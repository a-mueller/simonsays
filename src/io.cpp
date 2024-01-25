#include <hardware/gpio.h>
#include "io.hpp"

#include <neotrellis.hpp>

#include "config.hpp"

void init_io() {
    gpio_init(PIN_LED_USER_TURN);
    gpio_set_dir(PIN_LED_USER_TURN, GPIO_OUT);

    gpio_init(PIN_LED_COMPUTER_TURN);
    gpio_set_dir(PIN_LED_COMPUTER_TURN, GPIO_OUT);

    gpio_init(PIN_BTN_LED_SONG_SELECT);
    gpio_set_dir(PIN_BTN_LED_SONG_SELECT, GPIO_OUT);

    gpio_init(PIN_BTN_SONG_SELECT);
    gpio_set_dir(PIN_BTN_SONG_SELECT, GPIO_IN);

    gpio_init(PIN_BTN_LED_RESET_SONG);
    gpio_set_dir(PIN_BTN_LED_RESET_SONG, GPIO_OUT);

    gpio_init(PIN_BTN_RESET_SONG);
    gpio_set_dir(PIN_BTN_RESET_SONG, GPIO_IN);

    gpio_init(PIN_SOUND_SWITCH);
    gpio_set_dir(PIN_SOUND_SWITCH, GPIO_OUT);

    /* Enable the sound output */
    gpio_put(PIN_SOUND_SWITCH, true);
}


void indicate_startup(NeoTrellis* trellis) {
    // switch on all the leds we have
    gpio_put(PIN_LED_USER_TURN, true);
    gpio_put(PIN_LED_COMPUTER_TURN, true);
    gpio_put(PIN_BTN_LED_SONG_SELECT, true);
    gpio_put(PIN_BTN_LED_RESET_SONG, true);

    // Test NeoPixels
    for (uint8_t i = 0; i < 16; i++) {
        trellis->pixels.set(i, COLOR_WHITE);
        trellis->pixels.show();
        sleep_ms(50);
    }
    for (uint8_t i = 0; i < 16; i++) {
        trellis->pixels.set(i, COLOR_BLACK);
        trellis->pixels.show();
        sleep_ms(50);
    }

    // switch off all the leds we have
    all_leds_off();
    sleep_ms(100);
}

void all_leds_off() {
    gpio_put(PIN_LED_USER_TURN, false);
    gpio_put(PIN_LED_COMPUTER_TURN, false);
    gpio_put(PIN_BTN_LED_SONG_SELECT, false);
    gpio_put(PIN_BTN_LED_RESET_SONG, false);
}


void show_song_selection_mode(NeoTrellis *trellis) {
    all_leds_off();
    gpio_put(PIN_BTN_LED_SONG_SELECT, true);

    trellis->pixels.set(0, COLOR_WHITE);
    trellis->pixels.set(1, COLOR_RED);
    trellis->pixels.set(2, COLOR_GREEN);

    trellis->pixels.show();
    sleep_ms(TRELLIS_UPDATE_DELAY_MS);
}

void all_trellis_off(NeoTrellis *trellis) {
    for (uint8_t i = 0; i < 16; i++) {
        trellis->pixels.set(i, COLOR_BLACK);
    }
    trellis->pixels.show();
    sleep_ms(TRELLIS_UPDATE_DELAY_MS);
}


void show_user_failed_pad(NeoTrellis *trellis) {
    trellis->pixels.set(0, COLOR_RED);
    trellis->pixels.set(3, COLOR_RED);
    trellis->pixels.set(5, COLOR_RED);
    trellis->pixels.set(6, COLOR_RED);
    trellis->pixels.set(9, COLOR_RED);
    trellis->pixels.set(10, COLOR_RED);
    trellis->pixels.set(12, COLOR_RED);
    trellis->pixels.set(15, COLOR_RED);
    trellis->pixels.show();
    sleep_ms(TRELLIS_UPDATE_DELAY_MS);
}

void show_user_success_pad(NeoTrellis *trellis) {
    trellis->pixels.set(7, COLOR_GREEN);
    trellis->pixels.set(8, COLOR_GREEN);
    trellis->pixels.set(10, COLOR_GREEN);
    trellis->pixels.set(13, COLOR_GREEN);
    trellis->pixels.show();
    sleep_ms(TRELLIS_UPDATE_DELAY_MS);
}

void show_trellis(NeoTrellis *trellis) {
    trellis -> pixels.show();
    sleep_ms(TRELLIS_UPDATE_DELAY_MS);
}

