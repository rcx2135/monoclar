local lvgl = require("lvgl")
  local tabs = require("monoclar.tabs")

  local tab = tabs.create {
      id = "hello",
      name = "Hello",
  }

tabs.register(tab)


  tab.root:Label {
      text = "Hello",
      text_color = "#FFFFFF",
      align = lvgl.ALIGN.CENTER,
  }
