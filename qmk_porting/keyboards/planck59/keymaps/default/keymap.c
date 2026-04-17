/*
Copyright 2022 Huckies <https://github.com/Huckies>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include QMK_KEYBOARD_H
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_all(/* 0: qwerty */
                     KC_ESC,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSLS, KC_BSPC,
                     KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,
               LT(2, KC_CAPS),  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, LT(2, KC_ENT),
                     KC_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_UP,   KC_SLSH, KC_RSFT ,
                     KC_LCTL,  KC_LGUI, KC_LALT, KC_SPC,  KC_SPC,  MO(1),   MO(3),   KC_SPC,  KC_LEFT, KC_DOWN, KC_RGHT, KC_RCTL
                  ),

    [1] = LAYOUT_all(/* 1: fn */
                     KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,      
                     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  
                     TG(3),   _______, _______, TG(0),   _______, _______, _______, _______, _______, _______, _______, _______, _______, 
                     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                     _______, _______, _______, _______, _______, _______, _______,_______, _______, _______, _______, _______ 
                  ),
    [2] = LAYOUT_all(/* 2: fn */
                     KC_SLEP, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_PSCR, 
                     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RM_TOGG, 
                     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
                     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                     KC_HOME, KC_END, KC_PAGE_UP, KC_PAGE_DOWN, _______, _______, _______, _______, RM_PREV, RM_VALD, RM_VALU,RM_NEXT
                  ),
    [3] = LAYOUT_all(/* 3: fn */
                     KC_ESC,  KC_PMNS, KC_PPLS, KC_PSLS, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_BSPC,
                     KC_P7,   KC_P8,   KC_P9,   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,   
                     KC_P4,   KC_P5,   KC_P6,   KC_PAST, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L, 
                     KC_P1,   KC_P2,   KC_P3,   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_P,    KC_BSLS, 
                     KC_BSPC, KC_PDOT, KC_P0,   KC_LCTL, KC_LGUI, MO(1),   KC_LALT,   KC_SPC,  KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
                  ),

};


#ifdef ENCODER_MAP_ENABLE

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [2] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [3] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
};

#endif