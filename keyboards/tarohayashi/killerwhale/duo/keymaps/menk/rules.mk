VIA_ENABLE = yes
ENCODER_MAP_ENABLE = yes
CONSOLE_ENABLE = yes

FILTERED_SRC := $(filter-out %/add_keycodes.c %/add_oled.c, $(SRC))
SRC = $(FILTERED_SRC)

SRC += lib/common_killerwhale.c
SRC += lib/custom_keycodes.c
SRC += lib/add_rgblayers.c
SRC += lib/custom_oled.c
SRC += lib/moominhouse.c

