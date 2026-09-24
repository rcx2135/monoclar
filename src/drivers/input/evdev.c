#include "../input.h"

#include <errno.h>
#include <fcntl.h>
#include <libevdev/libevdev.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

struct input {
  int fd;
  struct libevdev *device;
  input_event_fn callback;
  void *userdata;
  float dx;
  float dy;
  bool active;
  bool syncing;
};

static void emit(input_t *input, input_event_type_t type, uint64_t time_us) {
  input_event_t event = {.type = type, .time_us = time_us};

  if (type == INPUT_MOVE) {
    event.dx = input->dx;
    event.dy = input->dy;
    input->dx = 0;
    input->dy = 0;
  }

  input->callback(&event, input->userdata);
}

static void flush_move(input_t *input, uint64_t time_us) {
  if (input->active && (input->dx != 0 || input->dy != 0)) {
    emit(input, INPUT_MOVE, time_us);
  }
}

static void cancel(input_t *input, uint64_t time_us) {
  bool active = input->active;
  input->active = false;
  input->dx = 0;
  input->dy = 0;

  if (active) {
    emit(input, INPUT_CANCEL, time_us);
  }
}

input_t *input_create(const char *path, input_event_fn callback,
                      void *userdata) {
  if (!path || !callback) {
    errno = EINVAL;
    return NULL;
  }

  int fd = open(path, O_RDONLY | O_NONBLOCK | O_CLOEXEC);
  if (fd < 0) {
    return NULL;
  }

  input_t *input = calloc(1, sizeof(input_t));
  if (!input) {
    close(fd);
    errno = ENOMEM;
    return NULL;
  }

  input->fd = fd;
  input->callback = callback;
  input->userdata = userdata;

  int result = libevdev_new_from_fd(fd, &input->device);

  if (result == 0 &&
      (!libevdev_has_event_code(input->device, EV_REL, REL_X) ||
       !libevdev_has_event_code(input->device, EV_REL, REL_Y) ||
       !libevdev_has_event_code(input->device, EV_KEY, BTN_LEFT))) {
    result = -ENODEV;
  }

  if (result == 0) {
    result = libevdev_set_clock_id(input->device, CLOCK_MONOTONIC);
  }

  if (result < 0) {
    input_destroy(input);
    errno = -result;
    return NULL;
  }

  return input;
}

int input_poll(input_t *input) {
  if (!input) {
    return -EINVAL;
  }

  for (unsigned int i = 0; i < 256; ++i) {
    struct input_event raw;
    unsigned int flags =
        input->syncing ? LIBEVDEV_READ_FLAG_SYNC : LIBEVDEV_READ_FLAG_NORMAL;

    int result = libevdev_next_event(input->device, flags, &raw);

    if (result == -EAGAIN) {
      if (input->syncing) {
        input->syncing = false;
        continue;
      }
      return 0;
    }

    if (result == -EINTR) {
      continue;
    }

    if (result < 0) {
      struct timespec now = {0};
      clock_gettime(CLOCK_MONOTONIC, &now);
      cancel(input,
             (uint64_t)now.tv_sec * 1000000 + (uint64_t)now.tv_nsec / 1000);
      return result;
    }

    if (input->syncing) {
      continue;
    }

    uint64_t time_us =
        (uint64_t)raw.time.tv_sec * 1000000 + (uint64_t)raw.time.tv_usec;

    if (result == LIBEVDEV_READ_STATUS_SYNC) {
      input->syncing = true;
      cancel(input, time_us);
    } else if (raw.type == EV_KEY && raw.code == BTN_LEFT) {
      if (raw.value == 1 && !input->active) {
        input->active = true;
        emit(input, INPUT_BEGIN, time_us);
      } else if (raw.value == 0 && input->active) {
        flush_move(input, time_us);
        input->active = false;
        emit(input, INPUT_END, time_us);
      }
    } else if (raw.type == EV_REL && input->active) {
      if (raw.code == REL_X) {
        input->dx += raw.value;
      } else if (raw.code == REL_Y) {
        input->dy += raw.value;
      }
    } else if (raw.type == EV_SYN && raw.code == SYN_REPORT) {
      flush_move(input, time_us);
    }
  }

  return 0;
}

void input_destroy(input_t *input) {
  if (!input) {
    return;
  }

  if (input->device) {
    libevdev_free(input->device);
  }

  close(input->fd);
  free(input);
}
