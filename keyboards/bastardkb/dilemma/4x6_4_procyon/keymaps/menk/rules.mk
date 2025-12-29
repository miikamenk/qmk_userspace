VIA_ENABLE = yes
ENCODER_MAP_ENABLE = no

QUANTUM_PAINTER_ENABLE = yes
QUANTUM_PAINTER_DRIVERS = ili9341_spi
QUANTUM_PAINTER_CONCURRENT_ANIMATIONS = 16
QUANTUM_PAINTER_LVGL_INTEGRATION = yes
OS_DETECTION_ENABLE = yes
WPM_ENABLE = yes
SPLIT_TRANSPORT_SYNC = yes

BACKLIGHT_ENABLE = yes

src += gfx/common.qgf.c
src += gfx/font_oled.qff.c
src += gfx/fonts.qff.c
src += gfx/dark_souls.qgf.c
src += gfx/mew.qgf.c
