#ifndef MOOMINHOUSE_H
#    include <stdbool.h>
#    include <stdint.h>
#    include "quantum.h"
#    define ENABLE_DAY_NIGHT   // Uses ~120 bytes: toggles background brightness + sun position
#    define ENABLE_MOON_PHASES // Uses ~96 bytes: cycles through moon phases (works with ENABLE_MOON)
#    define ENABLE_SMOKE       // Uses ~420 bytes: particle-ish smoke from island/volcano chimney
#    define ENABLE_CLOUDS      // Uses ~512 bytes: drifting clouds (variable width sprites)
#    define ENABLE_RAIN        // Uses ~540 bytes: raindrops + splash animation

/**
 * Day / Night cycle
 *
 * Simple cycle that changes global brightness and optionally shifts sun position.
 * Prefer values that are multiples of FRAME/NUMBER_OF_FRAMES for smoothness.
 */
#    define DAY_NIGHT_CYCLE_FRAMES 2400
#    define BLACK_FRAMES 24
#    define DAY_BRIGHTNESS_MIN 20  // 0-255 (ambient multiplier during night)
#    define DAY_BRIGHTNESS_MAX 255 // 0-255 (ambient multiplier during day)
#    define SUN_LINE 2             // line where the sun is drawn (if implemented)
#    define SUN_COLUMN 80          // approximate column for sun path center

/**
 * Moon phase parameters
 *
 * Requires ENABLE_MOON and ENABLE_MOON_PHASES to show animated phases.
 */
#    define MOON_PHASES 8 // number of moon phase sprites (common: 8)
#    ifndef STATIC_MOON
#        define MOON_PHASE_CHANGE_EVERY_N_FRAMES (NUMBER_OF_FRAMES * 6) // change phase every N frames
#    endif
#    define MOON_PHASE_START 0 // starting phase index (0..MOON_PHASES-1)

/**
 * Smoke (particle-ish)
 *
 * Lightweight smoke: small number of vertical drifting sprites built from bytes.
 */
#    define SMOKE_MAX_PARTICLES 6     // max simultaneous smoke puffs
#    define SMOKE_EMIT_DELAY 8        // frames between new puffs (modulus)
#    define SMOKE_LIFETIME_FRAMES 60  // how many animation frames a puff lives
#    define SMOKE_RISE_SPEED_FRAMES 1 // pixels per frame (can be fractional in logic)
#    define SMOKE_DRIFT_AMOUNT 1      // horizontal drift, -n...n
#    define SMOKE_FADE_START_FRAME 30 // when to start fading alpha/brightness for a puff

/**
 * Clouds
 */
#    define CLOUD_MAX 3                // number of cloud sprites on-screen
#    define CLOUD_MIN_SPEED_FRAMES 120 // frames to move one pixel (slower)
#    define CLOUD_MAX_SPEED_FRAMES 40  // frames to move one pixel (faster)
#    define CLOUD_WRAP_AROUND 1        // 1 = when cloud leaves screen, re-enter from other side
#    define CLOUD_RENDER_LINE 3        // vertical offset lines where clouds are drawn

/**
 * Rain
 *
 * Simple column-based raindrops; inexpensive if kept small.
 */
#    define RAIN_DENSITY 12          // heuristic density (0..127) - larger = more drops
#    define RAIN_MAX_DROPS 40        // hard cap on active drops
#    define RAIN_DROP_SPEED_FRAMES 3 // frames per vertical move for drop
#    define RAIN_SPLASH_LIFETIME 6   // frames for splash animation

/**
 * Fog / overlay wash
 */
#    define FOG_ENABLED_ALPHA 120      // 0..255 overlay alpha for fog layer (tweak for subtlety)
#    define FOG_RISE_SPEED_FRAMES 180  // frames between slight fog shifts
#    define FOG_HORIZONTAL_VARIATION 8 // pixels of horizontal perlin-ish shift

void render_moominhouse(void);

#    ifdef ENABLE_DAY_NIGHT
void update_day_night(uint16_t frame_count);
#    endif

#    ifdef ENABLE_SMOKE
void spawn_smoke_if_needed(uint16_t frame_count, uint8_t x, uint8_t y);
#    endif

#    ifdef ENABLE_MOON_PHASES
void draw_moon_phase(uint8_t phase_index);
#    endif
#endif
