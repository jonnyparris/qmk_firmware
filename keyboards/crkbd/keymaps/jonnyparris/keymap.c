/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

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
#include <stdio.h>

enum layers {
  _COLEMAK,
  _NUMS,
  _SYMS,
  _QWERTY
};

// TODO - maybe
// Custom keycodes for layer keys
// Dual function escape with left command
// #define KC_LGESC LGUI_T(KC_ESC)

bool is_cmd_tab_active = false;
uint16_t cmd_tab_timer = 0;

#define SPC_FN1 LT(1, KC_SPC)
#define SPC_FN2 LT(2, KC_SPC)
#define SPC_FN3 LT(3, KC_SPC)

enum custom_keycodes {
    CMD_TAB = SAFE_RANGE,
    MACRO_0,
    MACRO_1,
    MACRO_2,
    MACRO_3,
    MACRO_4,
    MACRO_5,
    MACRO_6,
    MACRO_7,
    MACRO_8,
    MACRO_9,
    MACRO_10,
    MACRO_11,
    MACRO_12,
    MACRO_13,
    MACRO_14,
    MACRO_15,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_COLEMAK] = LAYOUT_split_3x6_3(
        CMD_TAB,    KC_Q,       KC_W,       KC_F,       KC_P,       KC_B,            KC_J,      KC_L,       KC_U,       KC_Y,       KC_SCLN,    RGB_TOG,
        KC_CAPS,    KC_A,       KC_R,       KC_S,       KC_T,       KC_G,            KC_M,      KC_N,       KC_E,       KC_I,       KC_O,       KC_QUOT,
        KC_LCTL,    KC_Z,       KC_X,       KC_C,       KC_D,       KC_V,            KC_K,      KC_H,       KC_COMM,    KC_DOT,     KC_SLSH,    DF(3),
                        MT(MOD_LSFT,KC_ESC), SPC_FN1, MT(MOD_LGUI,KC_TAB),           KC_SFTENT, MT(MOD_RALT,KC_BSPC),  TT(2)),
	[_NUMS] = LAYOUT_split_3x6_3(
        QK_BOOT,    KC_1,       KC_2,       KC_3,       KC_4,       KC_5,            KC_6,      KC_7,       KC_8,       KC_9,       KC_0,       KC_TRNS,
        KC_TRNS,    KC_MPRV,    KC_MPLY,    KC_MNXT,    MACRO_8,    MACRO_1,         KC_LEFT,   KC_DOWN,    KC_UP,      KC_RGHT,    KC_NO,      QK_BOOT,
        KC_TRNS,    KC_MUTE,    KC_VOLD,    KC_VOLU,    MACRO_7,    MACRO_2,         MACRO_0,   MACRO_4,    MACRO_3,    MACRO_5,    MACRO_6,    KC_NO,
                                            KC_TRNS,    KC_TRNS,    KC_TRNS,         KC_RGUI,   KC_TRNS,    KC_TRNS),
	[_SYMS] = LAYOUT_split_3x6_3(
        KC_TRNS,    KC_EXLM,    KC_AT,      KC_HASH,    KC_DLR,     KC_PERC,         KC_CIRC,   KC_AMPR,    KC_ASTR,    KC_LPRN,    KC_RPRN,    QK_BOOT,
        KC_TRNS,    KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,           KC_MINS,   KC_EQL,     KC_LBRC,    KC_RBRC,    KC_BSLS,    KC_GRV,
        KC_TRNS,    KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,           KC_UNDS,   KC_PLUS,    KC_LCBR,    KC_RCBR,    KC_PIPE,    KC_TILD,
                                            KC_TRNS,    KC_TRNS,    KC_TRNS,         KC_TRNS,   KC_TRNS,    KC_TRNS),
	[_QWERTY] = LAYOUT_split_3x6_3(
        CMD_TAB,    KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,            KC_Y,      KC_U,       KC_I,       KC_O,       KC_P,       RGB_TOG,
        KC_LCAP,    KC_A,       KC_S,       KC_D,       KC_F,       KC_G,            KC_H,      KC_J,       KC_K,       KC_L,       KC_SCLN,    KC_QUOT,
        KC_LCTL,    KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,            KC_N,      KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,    QK_DEF_LAYER,
                        MT(MOD_LSFT,KC_ESC), SPC_FN1, MT(MOD_LGUI,KC_TAB),           KC_SFTENT, MT(MOD_RALT,KC_BSPC),   KC_RALT)
};

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

void render_space(void) {
    oled_write_P(PSTR("     "), false);
}

void render_mod_status_gui_alt(uint8_t modifiers) {
    static const char PROGMEM gui_off_1[] = {0x85, 0x86, 0};
    static const char PROGMEM gui_off_2[] = {0xa5, 0xa6, 0};
    static const char PROGMEM gui_on_1[] = {0x8d, 0x8e, 0};
    static const char PROGMEM gui_on_2[] = {0xad, 0xae, 0};

    static const char PROGMEM alt_off_1[] = {0x87, 0x88, 0};
    static const char PROGMEM alt_off_2[] = {0xa7, 0xa8, 0};
    static const char PROGMEM alt_on_1[] = {0x8f, 0x90, 0};
    static const char PROGMEM alt_on_2[] = {0xaf, 0xb0, 0};

    // fillers between the modifier icons bleed into the icon frames
    static const char PROGMEM off_off_1[] = {0xc5, 0};
    static const char PROGMEM off_off_2[] = {0xc6, 0};
    static const char PROGMEM on_off_1[] = {0xc7, 0};
    static const char PROGMEM on_off_2[] = {0xc8, 0};
    static const char PROGMEM off_on_1[] = {0xc9, 0};
    static const char PROGMEM off_on_2[] = {0xca, 0};
    static const char PROGMEM on_on_1[] = {0xcb, 0};
    static const char PROGMEM on_on_2[] = {0xcc, 0};

    if(modifiers & MOD_MASK_GUI) {
        oled_write_P(gui_on_1, false);
    } else {
        oled_write_P(gui_off_1, false);
    }

    if ((modifiers & MOD_MASK_GUI) && (modifiers & MOD_MASK_ALT)) {
        oled_write_P(on_on_1, false);
    } else if(modifiers & MOD_MASK_GUI) {
        oled_write_P(on_off_1, false);
    } else if(modifiers & MOD_MASK_ALT) {
        oled_write_P(off_on_1, false);
    } else {
        oled_write_P(off_off_1, false);
    }

    if(modifiers & MOD_MASK_ALT) {
        oled_write_P(alt_on_1, false);
    } else {
        oled_write_P(alt_off_1, false);
    }

    if(modifiers & MOD_MASK_GUI) {
        oled_write_P(gui_on_2, false);
    } else {
        oled_write_P(gui_off_2, false);
    }

    if (modifiers & MOD_MASK_GUI & MOD_MASK_ALT) {
        oled_write_P(on_on_2, false);
    } else if(modifiers & MOD_MASK_GUI) {
        oled_write_P(on_off_2, false);
    } else if(modifiers & MOD_MASK_ALT) {
        oled_write_P(off_on_2, false);
    } else {
        oled_write_P(off_off_2, false);
    }

    if(modifiers & MOD_MASK_ALT) {
        oled_write_P(alt_on_2, false);
    } else {
        oled_write_P(alt_off_2, false);
    }
}

void render_mod_status_ctrl_shift(uint8_t modifiers) {
    static const char PROGMEM ctrl_off_1[] = {0x89, 0x8a, 0};
    static const char PROGMEM ctrl_off_2[] = {0xa9, 0xaa, 0};
    static const char PROGMEM ctrl_on_1[] = {0x91, 0x92, 0};
    static const char PROGMEM ctrl_on_2[] = {0xb1, 0xb2, 0};

    static const char PROGMEM shift_off_1[] = {0x8b, 0x8c, 0};
    static const char PROGMEM shift_off_2[] = {0xab, 0xac, 0};
    static const char PROGMEM shift_on_1[] = {0xcd, 0xce, 0};
    static const char PROGMEM shift_on_2[] = {0xcf, 0xd0, 0};

    // fillers between the modifier icons bleed into the icon frames
    static const char PROGMEM off_off_1[] = {0xc5, 0};
    static const char PROGMEM off_off_2[] = {0xc6, 0};
    static const char PROGMEM on_off_1[] = {0xc7, 0};
    static const char PROGMEM on_off_2[] = {0xc8, 0};
    static const char PROGMEM off_on_1[] = {0xc9, 0};
    static const char PROGMEM off_on_2[] = {0xca, 0};
    static const char PROGMEM on_on_1[] = {0xcb, 0};
    static const char PROGMEM on_on_2[] = {0xcc, 0};

    if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(ctrl_on_1, false);
    } else {
        oled_write_P(ctrl_off_1, false);
    }

    if ((modifiers & MOD_MASK_CTRL) && (modifiers & MOD_MASK_SHIFT)) {
        oled_write_P(on_on_1, false);
    } else if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(on_off_1, false);
    } else if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(off_on_1, false);
    } else {
        oled_write_P(off_off_1, false);
    }

    if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(shift_on_1, false);
    } else {
        oled_write_P(shift_off_1, false);
    }

    if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(ctrl_on_2, false);
    } else {
        oled_write_P(ctrl_off_2, false);
    }

    if (modifiers & MOD_MASK_CTRL & MOD_MASK_SHIFT) {
        oled_write_P(on_on_2, false);
    } else if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(on_off_2, false);
    } else if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(off_on_2, false);
    } else {
        oled_write_P(off_off_2, false);
    }

    if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(shift_on_2, false);
    } else {
        oled_write_P(shift_off_2, false);
    }
}


void render_layer_icon(void) {
    static const char PROGMEM middle_layer[] = {
        0x20, 0x94, 0x95, 0x96, 0x20,
        0x20, 0xb4, 0xb5, 0xb6, 0x20,
        0x20, 0xd4, 0xd5, 0xd6, 0x20, 0};
    static const char PROGMEM upper_layer[] = {
        0x20, 0x97, 0x98, 0x99, 0x20,
        0x20, 0xb7, 0xb8, 0xb9, 0x20,
        0x20, 0xd7, 0xd8, 0xd9, 0x20, 0};
    static const char PROGMEM bottom_layer[] = {
        0x20, 0x9a, 0x9b, 0x9c, 0x20,
        0x20, 0xba, 0xbb, 0xbc, 0x20,
        0x20, 0xda, 0xdb, 0xdc, 0x20, 0};
    static const char PROGMEM colemak_layer[] = {
        0x20, 0x9d, 0x9e, 0x9f, 0x20,
        0x20, 0xbd, 0xbe, 0xbf, 0x20,
        0x20, 0xdd, 0xde, 0xdf, 0x20, 0};
    if(layer_state_is(_COLEMAK)) {
        oled_write_P(colemak_layer, false);
    } else if(layer_state_is(_NUMS)) {
        oled_write_P(middle_layer, false);
    } else if(layer_state_is(_SYMS)) {
        oled_write_P(upper_layer, false);
    } else {
        oled_write_P(bottom_layer, false);
    }
}

#define L_BASE 0
#define L_NUMS 2
#define L_SYMS 4
#define L_ALT 8

void render_layer_name(void) {
    switch (layer_state) {
        case L_BASE:
            oled_write_ln_P(PSTR("BASE"), false);
            break;
        case L_NUMS:
            oled_write_ln_P(PSTR("NUMS"), false);
            break;
        case L_SYMS:
            oled_write_ln_P(PSTR("SYMS"), false);
            break;
        case L_ALT:
        case L_ALT|L_NUMS:
        case L_ALT|L_SYMS:
        case L_ALT|L_NUMS|L_SYMS:
            oled_write_ln_P(PSTR("QWERTY"), false);
            break;
    }
}

// char keylog_str[24] = {};

// const char code_to_name[60] = {
//     ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
//     'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
//     'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
//     '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
//     'R', 'E', 'B', 'T', '_', '-', '=', '[', ']', '\\',
//     '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};

// void set_keylog(uint16_t keycode, keyrecord_t *record) {
//   char name = ' ';
//     if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
//         (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) { keycode = keycode & 0xFF; }
//   if (keycode < 60) {
//     name = code_to_name[keycode];
//   }

//   // update keylog
//   snprintf(keylog_str, sizeof(keylog_str), "%dx%d, k%2d : %c",
//            record->event.key.row, record->event.key.col,
//            keycode, name);
// }

// void render_keylog(void) {
//     oled_write(keylog_str, false);
// }

void render_logo(void) {
    static const char PROGMEM corne_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0};
    oled_write_P(corne_logo, false);
    oled_write_P(PSTR("corne"), false);
}

bool oled_task_user(void) {
    render_logo();
    render_space();
    render_layer_icon();
    render_space();
    render_layer_name();
    render_space();
    // render_keylog();
    render_mod_status_gui_alt(get_mods()|get_oneshot_mods());
    render_mod_status_ctrl_shift(get_mods()|get_oneshot_mods());
    return false;
}
#endif // OLED_ENABLE


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MACRO_0:
            SEND_STRING(SS_DOWN(X_LGUI)SS_DOWN(X_LALT)SS_DOWN(X_LCTL)SS_TAP(X_LEFT)SS_UP(X_LCTL)SS_UP(X_LALT)SS_UP(X_LGUI));
            return false;
        case MACRO_1:
            SEND_STRING(SS_DOWN(X_RGUI)SS_DOWN(X_TAB)SS_TAP(X_TAB)SS_UP(X_TAB)SS_UP(X_RGUI));
            return false;
        case MACRO_2:
            SEND_STRING(SS_DOWN(X_RGUI)SS_TAP(X_GRV)SS_UP(X_RGUI));
            return false;
        case MACRO_3:
            SEND_STRING(SS_DOWN(X_LGUI)SS_DOWN(X_LALT)SS_DOWN(X_LCTL)SS_TAP(X_RGHT)SS_UP(X_LCTL)SS_UP(X_LALT)SS_UP(X_LGUI));
            return false;
        case MACRO_4:
            SEND_STRING(SS_DOWN(X_LGUI)SS_DOWN(X_LALT)SS_DOWN(X_LCTL)SS_TAP(X_ENT)SS_UP(X_LCTL)SS_UP(X_LALT)SS_UP(X_LGUI));
            return false;
        case MACRO_5:
            SEND_STRING(SS_DOWN(X_LGUI)SS_DOWN(X_LALT)SS_DOWN(X_LCTL)SS_TAP(X_C)SS_UP(X_LCTL)SS_UP(X_LALT)SS_UP(X_LGUI));
            return false;
        case MACRO_6:
            SEND_STRING(SS_DOWN(X_LGUI)SS_DOWN(X_LALT)SS_DOWN(X_LCTL)SS_TAP(X_UP)SS_UP(X_LCTL)SS_UP(X_LALT)SS_UP(X_LGUI));
            return false;
        case MACRO_7:
            SEND_STRING(SS_DOWN(X_LCTL)SS_TAP(X_L)SS_UP(X_LCTL));
            return false;
        case MACRO_8:
            SEND_STRING(SS_DOWN(X_LCTL)SS_TAP(X_W)SS_UP(X_LCTL));
            return false;
        case CMD_TAB:
            if (record->event.pressed) {
                if (!is_cmd_tab_active) {
                    is_cmd_tab_active = true;
                    register_code(KC_LGUI);
                }
                cmd_tab_timer = timer_read();
                register_code(KC_TAB);
            } else {
                unregister_code(KC_TAB);
            break;
            }
    }
    // set_keylog(keycode, record);
    return true;
};

void matrix_scan_user(void) { // The very important timer.
  if (is_cmd_tab_active) {
    if (timer_elapsed(cmd_tab_timer) > 1000) {
      unregister_code(KC_LGUI);
      is_cmd_tab_active = false;
    }
  }
}

int RGB_current_mode;

// Setting ADJUST layer RGB back to default
// void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
//   if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
//     layer_on(layer3);
//   } else {
//     layer_off(layer3);
//   }
// }

#ifdef RGB_MATRIX_ENABLE

//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;

void suspend_power_down_keymap(void) {
    rgb_matrix_set_suspend_state(true);
}

void suspend_wakeup_init_keymap(void) {
    rgb_matrix_set_suspend_state(false);
}

#endif

void matrix_init_user(void) {
    #ifdef RGBLIGHT_ENABLE
      RGB_current_mode = rgblight_config.mode;
    #endif
}
