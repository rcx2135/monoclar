local lvgl = require("lvgl")
local screen = require("monoclar.screen")
local tabs = require("monoclar.tabs")

local period = 40
local speed = 20
local offset = 0
local populated = false

local overlay = screen.root:Object {
    w = lvgl.PCT(100),
    h = 56,
    align = lvgl.ALIGN.TOP_MID,
    bg_color = "#000000",
    bg_opa = lvgl.OPA(100),
    border_width = 0,
    radius = 0,
    pad_all = 0,
    scrollable = false,
}


local label = overlay:Label {
    text = "OVERLAY TEST",
    text_color = "#FF0000",
    align = lvgl.ALIGN.CENTER,

}
