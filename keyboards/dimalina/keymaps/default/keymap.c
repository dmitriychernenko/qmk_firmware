// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#include "timer.h"
#include "hardware/regs/rosc.h"
#include "hardware/regs/addressmap.h"


#define _LAYER_BASE 0
#define _LAYER_LOWER 1
#define _LAYER_HIGHER 2

const rgblight_segment_t PROGMEM my_capslock_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {6, 2, HSV_RED},       // Light 4 LEDs, starting with LED 6
    {12, 2, HSV_RED}       // Light 4 LEDs, starting with LED 12
);

// Light LEDs 9 & 10 in cyan when keyboard layer 1 is active
const rgblight_segment_t PROGMEM my_layer1_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 15, HSV_WHITE}
);
// Light LEDs 11 & 12 in purple when keyboard layer 2 is active
const rgblight_segment_t PROGMEM my_layer2_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 15, HSV_PURPLE}
);
// Light LEDs 13 & 14 in green when keyboard layer 3 is active
const rgblight_segment_t PROGMEM my_layer3_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 15, HSV_GREEN}
);

// Now define the array of layers. Later layers take precedence
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    my_capslock_layer,
    my_layer1_layer,    // Overrides caps lock layer
    my_layer2_layer,    // Overrides other layers
    my_layer3_layer     // Overrides other layers
);

void keyboard_post_init_user(void) {
    // Enable the LED layers
    rgblight_layers = my_rgb_layers;
}

bool led_update_user(led_t led_state) {
    rgblight_set_layer_state(0, led_state.caps_lock);
    return true;
}

layer_state_t default_layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(1, layer_state_cmp(state, _LAYER_BASE));
    return state;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(2, layer_state_cmp(state, _LAYER_LOWER));
    rgblight_set_layer_state(3, layer_state_cmp(state, _LAYER_HIGHER));
    return state;
}

enum custom_keycodes {
    ARIA_HIDDEN_TRUE = SAFE_RANGE,
    ARIA_HIDDEN_FALSE,
    IS_INVALID,
    CLASS,
    
    VS_SURROUND_WITH,

    FORM_LABEL,
    FORM_CHECK_LABEL,
    FORM_CONTROL,
    FORM_CHECK_INPUT,

    RANDOM_EMAIL,

    BOOTLOADER_OR_ESC,

    MACRO_1,
    MACRO_2,
    MACRO_3,
    MACRO_4
};

static uint16_t random_value = 157;

uint32_t rnd(void){
    int k, random=0;
    volatile uint32_t *rnd_reg=(uint32_t *)(ROSC_BASE + ROSC_RANDOMBIT_OFFSET);
    
    for(k=0; k<32; k++){
    
        random = random << 1;
        random=random + (0x00000001 & (*rnd_reg));
    }

    return random;
}

uint16_t bootloader_or_esc_timer;
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint8_t clockbyte = rnd() % 256;
    uint8_t rval;

    if (record->event.pressed){
        switch (keycode) {
            case BOOTLOADER_OR_ESC:
                bootloader_or_esc_timer = timer_read();
            break;
        }

        return true; // pressed
    }

    switch (keycode) {
        case ARIA_HIDDEN_TRUE:
            SEND_STRING(" aria-hidden=\"true\"");
        break;

        case ARIA_HIDDEN_FALSE:
            SEND_STRING(" aria-hidden=\"false\"");
        break;

        case IS_INVALID:
            SEND_STRING("'is-invalid'");
        break;

        case FORM_LABEL:
            SEND_STRING("form-label");
        break;
        case FORM_CHECK_LABEL:
            SEND_STRING("form-check-label");
        break;
        case FORM_CONTROL:
            SEND_STRING("form-control");
        break;
        case FORM_CHECK_INPUT:
            SEND_STRING("form-check-input");
        break;
        case CLASS:
            SEND_STRING(" class=\"\"" SS_TAP(X_LEFT));
        break;
        case MACRO_1:
            tap_code(KC_ENTER);
            SEND_STRING("element.attr(\"aria-sort\", \"ascending\");");
        break;
        
        case MACRO_2:
            tap_code(KC_ENTER);
            SEND_STRING("element.attr(\"aria-sort\", \"descending\");");
        break;
        
        case MACRO_3:
            tap_code(KC_ENTER);
            SEND_STRING("$(this).removeAttr(\"aria-sort\");");
        break;
        
        case MACRO_4:
            tap_code(KC_ENTER);
            SEND_STRING("element.attr(\"aria-sort\", \"ascending\");");
        break;

        case VS_SURROUND_WITH:
            SEND_STRING(SS_LCTL("ks"));
            tap_code(KC_DOWN);
            tap_code(KC_DOWN);
            wait_ms(250);
            tap_code(KC_ENTER);
        break;

        case RANDOM_EMAIL:
            for (int k = 0; k < 10; k++){
                random_value = ((random_value + randadd) * randmul) % randmod;
                rval = (random_value ^ clockbyte) % 26;
                tap_code(KC_A + rval);
            }

            SEND_STRING("@example.com");
        break;

        case BOOTLOADER_OR_ESC:
            if (timer_elapsed(bootloader_or_esc_timer) < 2500)
            {
                tap_code(KC_ESC);
            } else
            {
                reset_keyboard();
            }
        break;
    }

    return true;
};

/*
layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case _LAYER_LOWER:
            rgblight_setrgb(RGB_GREEN);
            break;
        case _LAYER_HIGHER:
            rgblight_setrgb(RGB_RED);
            break;
        default: // for any other layers, or the default layer
            rgblight_setrgb (RGB_WHITE);
            break;
    }
  return state;
}
*/
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┬───┬───┐
     * │ A │ B │ C │ D │ E │
     * ├───┼───┼───┼───┼───┤
     * │ A │ B │ C │ D │ E │
     * ├───┼───┼───┼───┼───┤
     * │ A │ B │ C │ D │ E │
     * ├───┼───┼───┼───┼───┤
     * │ A │ B │ C │ D │ E │
     * ├───┼───┼───┼───┼───┤
     * │ A │ B │ C │ D │ E │
     * └───┴───┴───┴───┴───┘
     */
    [_LAYER_BASE] = LAYOUT_ortho_7x5(
        BOOTLOADER_OR_ESC,              KC_B,           ARIA_HIDDEN_TRUE,   ARIA_HIDDEN_FALSE,    KC_C,             KC_D,                RANDOM_EMAIL,
        VS_SURROUND_WITH,                           KC_B,           KC_A,               KC_B,                 KC_C,             KC_D,                KC_NO,
        KC_A,                           KC_B,           FORM_LABEL,         FORM_CHECK_LABEL,     FORM_CONTROL,     FORM_CHECK_INPUT,    CLASS,
        KC_A,                           KC_B,           RGB_MODE_FORWARD,         KC_AUDIO_VOL_UP,      RGB_MODE_REVERSE,             KC_NO,                KC_NO,
        LCTL(KC_V),                     LCTL(KC_C),     TT(_LAYER_LOWER),         KC_AUDIO_VOL_DOWN,    TT(_LAYER_HIGHER),             KC_NO,                KC_CAPS_LOCK
    ),
    [_LAYER_LOWER] = LAYOUT_ortho_7x5(
        KC_TRANSPARENT,                 KC_B,           ARIA_HIDDEN_TRUE,       ARIA_HIDDEN_FALSE,          KC_C,                       KC_D,                       RANDOM_EMAIL,
        MACRO_1,                        MACRO_2,        MACRO_3,                MACRO_4,                    KC_C,                       KC_D,                       KC_NO,
        KC_A,                           KC_B,           FORM_LABEL,             FORM_CHECK_LABEL,           FORM_CONTROL,               FORM_CHECK_INPUT,           CLASS,
        KC_A,                           KC_B,           KC_MEDIA_PLAY_PAUSE,    KC_AUDIO_VOL_UP,            KC_MEDIA_NEXT_TRACK,        KC_NO,                      KC_NO,
        LCTL(KC_V),                     LCTL(KC_C),     KC_TRNS,                KC_AUDIO_VOL_DOWN,          TT(_LAYER_HIGHER),          KC_NO,                      KC_NO
    ),
    [_LAYER_HIGHER] = LAYOUT_ortho_7x5(
        KC_TRANSPARENT,               KC_NO,           KC_NO,         KC_NO,             KC_NO,             KC_NO,                           KC_NO,
        RGB_MODE_PLAIN,               RGB_MODE_BREATHE, RGB_MODE_RAINBOW,         RGB_MODE_SWIRL,             RGB_MODE_SNAKE,             RGB_MODE_KNIGHT,                           RGB_TOG,
        RGB_MODE_XMAS,                           RGB_MODE_GRADIENT,           RGB_MODE_RGBTEST,         RGB_MODE_TWINKLE,             KC_NO,             KC_NO,                           RGB_MODE_PLAIN,
        KC_NO,                           KC_NO,           RGB_MODE_FORWARD,         RGB_VAI,             RGB_MODE_REVERSE,             RGB_MODE_GRADIENT,               RGB_MODE_KNIGHT,
        KC_NO,                           KC_NO,           TT(_LAYER_LOWER),         RGB_VAD,             KC_TRNS,             RGB_MODE_RGBTEST,                RGB_MODE_XMAS
    )
};