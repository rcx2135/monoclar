local lvgl = require("lvgl")
local tabs = require("monoclar.tabs")

local tab = tabs.create {
    id = "test2",
    name = "thrghjrfghb",
}
tabs.register(tab)
tabs.set_active(tab)

local count_label = tab.root:Label {
    text = "Hello",
    text_color = "#FFFFFF",
    align = lvgl.ALIGN.CENTER,
}


print("yooo")
local ticks = 0

local timer = lvgl.Timer {
    period = 1000,
    cb = function(timer)
        ticks = ticks + 1
        print(count_label.text)
        count_label:set { text = string.format("Updated text %d", ticks) }
        print("test " .. ticks)
    end,
    repeat_count = nil,
    paused = false,
}
