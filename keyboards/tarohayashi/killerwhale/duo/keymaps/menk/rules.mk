ENCODER_MAP_ENABLE = yes


SRC := $(filter-out keyboards/mechboards/common/display_oled.c,$(SRC))

SRC += lib/add_keycodes.c
SRC += lib/add_oled.c
SRC += lib/moominhouse.c
