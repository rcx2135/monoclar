local lvgl = require("lvgl")
local screen = require("monoclar.screen")



local overlay = screen.root:Object {
    w = 300,
    h = 80,
    align = lvgl.ALIGN.BOTTOM_MID,
    flex_flow = lvgl.FLEX_FLOW.ROW,
    scroll_dir = lvgl.DIR.HOR,
    scrollbar_mode = lvgl.SCROLLBAR_MODE.OFF,
    pad_all = 0,
    pad_column = 8,
}

for i = 0, 9 do
    local item = overlay:Button {
        w = 70,
        h = 60,
    }

    item:Label {
        text = string.format("Item %d", i),
        align = lvgl.ALIGN.CENTER,
    }
end
