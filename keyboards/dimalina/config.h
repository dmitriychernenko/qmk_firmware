// Copyright 2023 D Chernenko (@dmitriychernenko)
// SPDX-License-Identifier: GPL-2.0-or-later


#pragma once

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT

#define WS2812_PIO_USE_PIO1
#define WS2812_DI_PIN GP22
#define RGBLED_NUM 15

#define RGBLIGHT_DEFAULT_VAL 127

#define RGBLIGHT_LAYERS
#define RGBLIGHT_LAYERS_RETAIN_VAL

#define RGBLIGHT_EFFECT_BREATHING
#define RGBLIGHT_EFFECT_CHRISTMAS
#define RGBLIGHT_EFFECT_KNIGHT
#define RGBLIGHT_EFFECT_RAINBOW_MOOD
#define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#define RGBLIGHT_EFFECT_RGB_TEST
#define RGBLIGHT_EFFECT_SNAKE
#define RGBLIGHT_EFFECT_STATIC_GRADIENT
#define RGBLIGHT_EFFECT_TWINKLE

//#define RGBLIGHT_DEFAULT_MODE RGBLIGHT_MODE_STATIC_LIGHT //RGBLIGHT_MODE_BREATHING
#define RGBLIGHT_DEFAULT_MODE RGBLIGHT_MODE_BREATHING

#define TAPPING_TERM 200
#define TAPPING_TOGGLE 2 // Toggle layer when tapped N times


#define randadd 53
#define randmul 181
#define randmod 167