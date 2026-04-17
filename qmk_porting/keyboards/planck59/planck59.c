/*
Copyright 2022 Huckies <https://github.com/Huckies>
Copyright 2023 OctopusZ <https://github.com/OctopusZ>

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

#ifdef RGB_MATRIX_ENABLE
/* clang-format off */
led_config_t g_led_config = {
    // 第1层：LED 矩阵位置（对应 5行 x 13列）
    {
        // 第0行（数字行）：13个 LED
        { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12 },
        // 第1行（QWERTY行）：13个 LED
        { 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 },
        // 第2行（ASDF行）：13个 LED
        { 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38 },
        // 第3行（ZXCV行）：13个 LED
        { 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51 },
        // 第4行（底部行）：12个 LED
        { 52, 53, 54, 55, 56, 57,NO_LED, 58, 59, 60, 61, 62, 63 }
    },
    // 第2层：每个 LED 的物理坐标 (x, y) - 单位像素，按键盘实际尺寸调整
    {
        // 第0行（Y=0）
         { 0,0 },  { 23,0 },  { 46,0 },  { 69,0 },  { 92,0 },  { 115,0 },  { 138,0 }, { 161,0 },  { 184,0 },  { 207,0 },  { 230,0 },  { 253,0 },  { 254,0 },
        // 第1行（Y=21）
        { 0,21 },  { 23,21 }, { 46,21 }, { 69,21 }, { 92,21 }, { 115,21 }, { 138,21 },{ 161,21 }, { 184,21 }, { 207,21 }, { 230,21 }, { 253,21 }, { 254,21 },
        // 第2行（Y=42）
        { 0,42 },  { 23,42 }, { 46,42 }, { 69,42 }, { 92,42 }, { 115,42 }, { 138,42 },{ 161,42 }, { 184,42 }, { 207,42 }, { 230,42 }, { 253,42 }, { 254,42 },
        // 第3行（Y=63）
        { 0,63 },  { 23,63 }, { 46,63 }, { 69,63 }, { 92,63 }, { 115,63 }, { 138,63 },{ 161,63 }, { 184,63 }, { 207,63 }, { 230,63 }, { 253,63 }, { 254,63 },
        // 第4行（Y=84）
        { 0,84 },  { 23,84 }, { 46,84 }, { 69,84 }, { 92,84 }, { 115,84 },            { 161,84 }, { 184,84 }, { 207,84 }, { 230,84 }, { 253,84 }, { 254,84 },

          
    },
    // 第3层：每个 LED 的物理尺寸（宽,高）- 按键大小
    {
        // 第0行
        1,1,1,1,1,1,1,1,1,1,1,1,1,
        // 第1行
        1,1,1,1,1,1,1,1,1,1,1,1,1,
        // 第2行
        1,1,1,1,1,1,1,1,1,1,1,1,1,
        // 第3行
        1,1,1,1,1,1,1,1,1,1,1,1,1,
        // 第4行（底部行，如果你有加宽的空格键，可以在这里调整）
        1,1,1,1,1,1,  1,1,1,1,1,1

        
    }


};
/* clang-format on */
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max)
{
    if (led_min <= 0 && led_max > 0 && host_keyboard_led_state().num_lock) {
        RGB_MATRIX_INDICATOR_SET_COLOR(0, 0xFF, 0x00, 0x00);
    }

    return true;
}
#endif

#ifdef OLED_ENABLE
#define BONGO_TIMEOUT 75 // the amount of time it takes to return to idle
uint32_t bongo_timeout_timer = 0;

#define BONGO_FRAMES 20
#define ANIM_SIZE    636 // number of bytes in array, minimize for adequate firmware size, max is 1024
#define IDLE_FRAMES  1
#define IDLE_TIMEOUT 800 // the amount of time it takes to return to idle

#define TAP_FRAMES 2
#define KEYS_SIZE  MATRIX_ROWS *MATRIX_COLS // the number of keys stored in the array that tracks keypresses; how many keys are on the board?

enum anim_states {
    Idle,
    Prep,
    Tap
};
uint8_t anim_state = Idle;
uint32_t idle_timeout_timer = 0;

uint8_t current_tap_frame = 0;

struct pair_int_int {
    uint8_t first;
    uint8_t second;
};
struct pair_int_int pressed_keys[KEYS_SIZE];
struct pair_int_int pressed_keys_prev[KEYS_SIZE];
uint8_t pressed_keys_index = 0;



bool detect_key_down(void)
{
    // store the previous cycle's cache
    for (uint8_t i = 0; i < KEYS_SIZE; ++i) {
        pressed_keys_prev[i].first = pressed_keys[i].first;
        pressed_keys_prev[i].second = pressed_keys[i].second;
    }

    // fill cache with currently pressed keys
    pressed_keys_index = 0;
    for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
        for (uint8_t y = 0; y < MATRIX_COLS; y++) {
            // is this key is currently down?
            if (((matrix_get_row(x) & (1 << y)) > 0)) {
                pressed_keys[pressed_keys_index].first = x + 1; // adding 1 to the row/col so that we can use 0 as a null-check
                pressed_keys[pressed_keys_index].second = y + 1;
            } else {
                pressed_keys[pressed_keys_index].first = 0;
                pressed_keys[pressed_keys_index].second = 0;
            }
            pressed_keys_index++;
        }
    }

    // check for a new key down compared to last cycle
    for (uint8_t i = 0; i < KEYS_SIZE; i++) {
        if (pressed_keys[i].first && pressed_keys[i].second && !pressed_keys_prev[i].first && !pressed_keys_prev[i].second) {
            return true;
        }
    }
    return false;
}

void eval_anim_state(void)
{
    key_down = detect_key_down();

    switch (anim_state) {
        case Idle:
            if (key_down) // Idle to Tap
            {
                anim_state = Tap;
            }
            break;

        case Prep:
            if (key_down) // Prep to Tap
            {
                anim_state = Tap;
            } else if (timer_elapsed32(idle_timeout_timer) >= IDLE_TIMEOUT) // Prep to Idle
            {
                anim_state = Idle;
            }
            break;
        case Tap:
            if (!key_down) // Tap to Prep
            {
                anim_state = Prep;
                idle_timeout_timer = timer_read32();
            }
            break;

        default:
            break;
    }
}

static void draw_bongo()
{
    eval_anim_state();

    oled_set_cursor(0, 0);

    switch (anim_state) {
        case Idle:
            oled_set_cursor(0, 4);
            oled_write_raw_P(idle_minimal[0], ANIM_SIZE);

            break;

        case Prep:
            oled_set_cursor(0, 4);
            oled_write_raw_P(prep_minimal[0], ANIM_SIZE);

            break;

        case Tap:
            oled_set_cursor(0, 4);
            oled_write_raw_P(tap_minimal[current_tap_frame], ANIM_SIZE);
            current_tap_frame = (current_tap_frame + 1) % TAP_FRAMES;

            break;

        default:
            break;
    }
}

bool oled_task_user(void)
{
    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.caps_lock ? PSTR("CAPLK ") : PSTR("_____ "), false);
    oled_write_P(led_state.num_lock ? PSTR("NUMLK ") : PSTR("_____ "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCRLK") : PSTR("_____"), false);
    // Host Keyboard Layer Status
    oled_set_cursor(0, 2);
    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case 0:
            oled_write_P(PSTR("0\n"), false);
            break;
        case 1:
            oled_write_P(PSTR("1\n"), false);
            break;
        case 2:
            oled_write_P(PSTR("2\n"), false);
            break;
        case 3:
            oled_write_P(PSTR("3\n"), false);
            break;
        case 4:
            oled_write_P(PSTR("4\n"), false);
            break;
        case 5:
            oled_write_P(PSTR("5\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }
    if (timer_elapsed32(bongo_timeout_timer) >= BONGO_TIMEOUT) // Prep to Idle
    {
        bongo_timeout_timer = timer_read();
        draw_bongo();
    }
    return false;
}
#endif

int main()
{
    extern void protocol_setup();
    extern void protocol_pre_init();
    extern void protocol_post_init();
    extern void platform_run();

    platform_setup();

    protocol_setup();
#if !defined ESB_ENABLE || ESB_ENABLE != 2
    keyboard_setup();
#endif

    protocol_pre_init();
    keyboard_init();
    protocol_post_init();

    /* Main loop */
    for (;;) {
        platform_run();
        //! housekeeping_task() is handled by platform
    }
}
