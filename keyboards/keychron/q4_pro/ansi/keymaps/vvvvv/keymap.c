/* Copyright 2025 vvvvv
 * Copyright 2023 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

enum layers { LAYER_0, LAYER_1, LAYER_2, LAYER_3, LAYER_4 };

// bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
//     switch (keycode) {
//         case LCTL_T(KC_ESC):
//             // Immediately select the hold action when another key is tapped.
//             return true;
//         default:
//             // Do not select the hold action when another key is tapped.
//             return false;
//     }
// }

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LCTL_T(KC_ESC): // Prioritize CTRL if another key is pressed
            return true;
        default:
            return false;
    }
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LCTL_T(KC_ESC): // Prioritize CTRL if another key is pressed
            return 0;
        default:
            return QUICK_TAP_TERM;
    }
}

// Define a type for as many tap dance states as you need
// typedef enum { TD_NONE, TD_UNKNOWN, TD_SINGLE_TAP, TD_SINGLE_HOLD, TD_DOUBLE_HOLD, TD_DOUBLE_TAP } td_state_t;
typedef enum { TD_NONE, TD_UNKNOWN, TD_SINGLE_TAP, TD_SINGLE_HOLD, TD_DOUBLE_TAP, TD_DOUBLE_HOLD, TD_TRIPPLE_TAP } td_state_t;

typedef struct {
    bool       is_press_action;
    td_state_t state;
} td_tap_t;

enum {
    LSHIFT_LAYER_2,
};

// Declare the functions to be used with your tap dance key(s)

// Function associated with all tap dances
td_state_t cur_dance(tap_dance_state_t *state);

// Functions associated with individual tap dances
void shift_layer_finished(tap_dance_state_t *state, void *user_data);
void shift_layer_reset(tap_dance_state_t *state, void *user_data);

// Determine the current tap dance state
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->pressed) {
            return TD_SINGLE_HOLD;
        } else {
            return TD_SINGLE_TAP;
        }
    } else if (state->count == 2) {
        if (state->pressed) {
            return TD_DOUBLE_HOLD;
        } else {
            return TD_DOUBLE_TAP;
        }
    } else if (state->count == 3) {
        if (!state->pressed) {
            return TD_TRIPPLE_TAP;
        }
    } else {
        return TD_UNKNOWN;
    }
    return TD_UNKNOWN;
}

// Initialize tap structure associated with example tap dance key
static td_tap_t shift_layer_tap = {.is_press_action = true, .state = TD_NONE};

// Functions that control what our tap dance key does
void shift_layer_finished(tap_dance_state_t *state, void *user_data) {
    shift_layer_tap.state = cur_dance(state);
    switch (shift_layer_tap.state) {
        case TD_SINGLE_TAP:
        case TD_SINGLE_HOLD:
            register_code(KC_LEFT_SHIFT);
            break;
        // case TD_DOUBLE_TAP:
        case TD_DOUBLE_HOLD:
            layer_on(LAYER_2);
            break;
        case TD_TRIPPLE_TAP:
            layer_invert(LAYER_2);
            break;
        //     // Check to see if the layer is already set
        //     if (layer_state_is(LAYER_2)) {
        //         // If already set, then switch it off
        //         layer_off(LAYER_2);
        //     } else {
        //         // If not already set, then switch the layer on
        //         layer_on(LAYER_2);
        //     }
        //     break;
        // case TD_DOUBLE_HOLD:
        //     // Check to see if the layer is already set
        //     if (layer_state_is(LAYER_2)) {
        //         // If already set, then switch it off
        //         layer_off(LAYER_2);
        //         register_code(KC_LEFT_SHIFT);
        //     } else {
        //         // If not already set, then switch the layer on
        //         layer_on(LAYER_2);
        //     }
        //     break;
        default:
            break;
    }
}

void shift_layer_reset(tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    switch (shift_layer_tap.state) {
        case TD_SINGLE_TAP:
        case TD_SINGLE_HOLD:
            unregister_code(KC_LEFT_SHIFT);
            break;
        // case TD_DOUBLE_TAP:
        case TD_DOUBLE_HOLD:
            layer_off(LAYER_2);
            break;
        default:
            break;
    }
    shift_layer_tap.state = TD_NONE;

    // if (shift_layer_tap.state == TD_SINGLE_TAP || shift_layer_tap.state == TD_SINGLE_HOLD) {
    //     unregister_code(KC_LEFT_SHIFT);
    // }
    //
    // shift_layer_tap.state = TD_NONE;
}

// Associate our tap dance key with its functionality
tap_dance_action_t tap_dance_actions[] = {[LSHIFT_LAYER_2] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, shift_layer_finished, shift_layer_reset)};

// Set a long-ish tapping term for tap-dance keys
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // case LCTL_T(KC_ESC):
        //     return 120;
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            return 275;
        default:
            return TAPPING_TERM;
    }
}

// clang-format off

// key rows:
// 14
// 14
// 13
// 12
// 8

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [LAYER_0] = LAYOUT_61_ansi(
        KC_ESC,         KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,      KC_MINS,      KC_EQL,       KC_BSPC,
        KC_TAB,         KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,      KC_LBRC,      KC_RBRC,      KC_BSLS,
        LCTL_T(KC_ESC), KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,   KC_QUOT,                    KC_ENT,
        TD(LSHIFT_LAYER_2),       KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,    KC_SLSH,                    KC_RSFT,
        KC_HYPR,       KC_LOPTN,  KC_LCMMD,                               KC_SPC,                                 KC_RCMMD,  MO(LAYER_3),  MO(LAYER_4),  KC_MEH),

    [LAYER_1] = LAYOUT_61_ansi(
        KC_ESC,         KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,      KC_MINS,      KC_EQL,       KC_BSPC,
        KC_TAB,         KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,      KC_LBRC,      KC_RBRC,      KC_BSLS,
        KC_CAPS,        KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,   KC_QUOT,                    KC_ENT,
        KC_LSFT,                  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,    KC_SLSH,                    KC_RSFT,
        KC_LCTL,        KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,   MO(LAYER_3),  MO(LAYER_4),  KC_RCTL),

    [LAYER_2] = LAYOUT_61_ansi(
        KC_GRV,         _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,      _______,      KC_DEL,
        _______,        _______,  _______,  _______,  _______,  _______,  _______,  KC_PGUP,  _______,  _______,  _______,   KC_HOME,      KC_END,       _______,
        _______,        _______,  _______,  KC_PGDN,  _______,  _______,  KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  _______,   _______,                    _______,
        _______,        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                               _______,
        _______,        _______,  _______,                                _______,                                _______,   _______,      _______,      _______),

    [LAYER_3] = LAYOUT_61_ansi(
        KC_GRV,        KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,   KC_VOLD,       KC_VOLU,      _______,
        _______,       BT_HST1,  BT_HST2,  BT_HST3,  _______,  _______,  _______,  _______,  _______,  _______,  KC_INS,    KC_PGUP,       KC_HOME,      _______,
        RGB_TOG,       RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  KC_UP,    KC_SNAP,  KC_PGDN,   KC_END,                      _______,
        _______,                 RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  NK_TOGG,  KC_LEFT,  KC_DOWN,  KC_RIGHT,  KC_DEL,                      _______,
        _______,       _______,  _______,                                _______,                                _______,   _______,       _______,      _______),

    [LAYER_4] = LAYOUT_61_ansi(
        KC_TILD,      KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,     KC_F10,    KC_F11,        KC_F12,       _______,
        _______,      _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,   _______,       _______,      _______,
        _______,      _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,   _______,                     _______,
        _______,      _______,  _______,  _______,  _______,  BAT_LVL,  _______,  _______,  _______,  _______,   _______,                                _______,
        _______,      _______,  _______,                                _______,                                 _______,   _______,       _______,      _______),
};
