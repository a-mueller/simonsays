#include <neotrellis.hpp>
#include <pico_tone.hpp>

#include "pico/stdlib.h"
#include <stdio.h>
#include <sys/stat.h>

#include "keypad_map.hpp"
#include "songs.hpp"

#define PICO_I2C i2c0
#define PICO_SDA 0
#define PICO_SCL 1

#define SOUND_OUT 27
#define SOUND_SWITCH 26

#define USER_BLOCKED_INDICATOR_LED 19
#define USER_TURN_INDICATOR_LED 18

// #define MODE_LED 2
// #define MODE_BTN 3
#define MODE_LED 5
#define MODE_BTN 4


#define RESET_LED 6
#define RESET_BTN 7

enum Mode {
    SONG_SELECTION,
    TURN_USER,
    TURN_COMPUTER,
    TURN_INCORRECT,
    TURN_FINISH
};

Mode mode = SONG_SELECTION;

NeoTrellis *trellisPtr;
Tone *myPlayerPtr;

int target_position_in_song = -1;
int current_position_in_song = -1;
int *currentSong;
int currentSongTotalLength;


void keypad_handler(uint8_t key, Keypad::Edge edge);

void init_buttons_and_leds();

void starutp_sequence();

void show_song_selection();

void show_user_failed_pad();

void show_user_success_pad();

void turn_keyboard_leds_off();

void game_turn_computer();


int main() {
    stdio_init_all();

    /* Init the trellis library */
    NeoTrellis trellis(PICO_I2C, PICO_SDA, PICO_SCL);
    trellis.init();
    trellis.keypad.set_callback(keypad_handler);
    trellisPtr = &trellis;


    /* Init the sound library */
    Tone myPlayer(SOUND_OUT, 50, 0, 20, 0, 20, 0, 10);
    // Tone myPlayer(SOUND_OUT);
    myPlayer.init(TONE_NON_BLOCKING, false);
    myPlayerPtr = &myPlayer;

    /* Setup the button and led pins */
    init_buttons_and_leds();


    /* Enable the sound output */
    gpio_put(SOUND_SWITCH, true);


    /* Show some startup sequence by flashing some lights */
    starutp_sequence();
    sleep_ms(50);
    gpio_put(MODE_LED, true);
    show_song_selection();
    trellis.pixels.show();

    while (true) {
        trellis.keypad.update();
        // trellis.pixels.show();

        if (gpio_get(MODE_BTN)) {
            // we want to switch mode
            if (mode != SONG_SELECTION) {
                mode = SONG_SELECTION;
                // turn the led on
                gpio_put(MODE_LED, true);
                show_song_selection();
            } else {
                mode = TURN_COMPUTER;
                target_position_in_song = 0;
                current_position_in_song = -1;
                // turn the led off
                gpio_put(MODE_LED, false);
                turn_keyboard_leds_off();
            }
            trellis.pixels.show();

            while (gpio_get(MODE_BTN)) {
                // wait till the button is up again
                sleep_ms(10);
            }
        } else {
            // game play
            if (mode == TURN_INCORRECT) {
                trellis.pixels.show();
                sleep_ms(50);
                // make a red X
                show_user_failed_pad();
                trellis.pixels.show();
                // play some error tone

                // reset game
                current_position_in_song = -1;
                target_position_in_song = 0;

                sleep_ms(1000);
                turn_keyboard_leds_off();
                trellis.pixels.show();

                mode = TURN_COMPUTER;
            } else if (mode == TURN_FINISH) {
                trellis.pixels.show();
                sleep_ms(50);
                // make a gree tick
                show_user_success_pad();
                trellis.pixels.show();
                // play some success tone

                // reset game
                current_position_in_song = -1;
                target_position_in_song = 0;

                sleep_ms(1000);
                turn_keyboard_leds_off();
                trellis.pixels.show();
            } else if (mode == TURN_COMPUTER) {
                if (current_position_in_song < target_position_in_song) {
                    game_turn_computer();
                } else {
                    mode = TURN_USER;
                    current_position_in_song = -1;
                }
            } else if (mode == TURN_USER) {
                trellis.pixels.show();
                if (current_position_in_song == target_position_in_song) {
                    mode = TURN_COMPUTER;
                    target_position_in_song++;
                    current_position_in_song = -1;
                    sleep_ms(500);
                }
            }

            sleep_ms(50);
        }


        // if (count++ % 2 == 0) {
        //     myPlayer.tone(NOTE_A4, 0.5);
        //
        // } else {
        //     myPlayer.tone(NOTE_C4, 0.5);
        // }
        // if (gpio_get(4)) {
        //     gpio_put(5, true);
        // } else {
        //     gpio_put(5, false);
        // }
        //
        // sleep_ms(1000);
    }
}

void init_buttons_and_leds() {
    // init the button
    gpio_init(USER_BLOCKED_INDICATOR_LED);
    gpio_set_dir(USER_BLOCKED_INDICATOR_LED, GPIO_OUT);

    gpio_init(USER_TURN_INDICATOR_LED);
    gpio_set_dir(USER_TURN_INDICATOR_LED, GPIO_OUT);

    gpio_init(MODE_LED);
    gpio_set_dir(MODE_LED, GPIO_OUT);

    gpio_init(MODE_BTN);
    gpio_set_dir(MODE_BTN, GPIO_IN);

    gpio_init(RESET_LED);
    gpio_set_dir(RESET_LED, GPIO_OUT);

    gpio_init(RESET_BTN);
    gpio_set_dir(RESET_BTN, GPIO_IN);

    gpio_init(SOUND_SWITCH);
    gpio_set_dir(SOUND_SWITCH, GPIO_OUT);
}

void starutp_sequence() {
    // switch on all the leds we have
    gpio_put(USER_BLOCKED_INDICATOR_LED, true);
    gpio_put(USER_TURN_INDICATOR_LED, true);
    gpio_put(MODE_LED, true);
    gpio_put(RESET_LED, true);

    // Test NeoPixels
    for (uint8_t i = 0; i < 16; i++) {
        trellisPtr->pixels.set(i, COLOR_WHITE);
        trellisPtr->pixels.show();
        sleep_ms(50);
    }
    for (uint8_t i = 0; i < 16; i++) {
        trellisPtr->pixels.set(i, COLOR_BLACK);
        trellisPtr->pixels.show();
        sleep_ms(50);
    }

    // switch off all the leds we have
    gpio_put(USER_BLOCKED_INDICATOR_LED, false);
    gpio_put(USER_TURN_INDICATOR_LED, false);
    gpio_put(MODE_LED, false);
    gpio_put(RESET_LED, false);
}


void keypad_handler(uint8_t key, Keypad::Edge edge) {
    if (edge == Keypad::Edge::RISING) {
        if (mode == TURN_USER) {
            int colour = keypad_pos_to_color.at(key);
            int correct_note = currentSong[(current_position_in_song + 1) * 2];
            if (note_to_kepad_pos.at(correct_note) == key) {
                myPlayerPtr->tone(correct_note);
            } else {
                myPlayerPtr->tone(kepad_pos_to_note.at(key));
            }

            trellisPtr->pixels.set(key, colour);
        } else if (mode == SONG_SELECTION) {
            if (key == 0) {
                currentSong = happyBirthday;
                currentSongTotalLength = HAPPY_BIRTHDAY_LENGTH;
                myPlayerPtr->play_melody(T_ADAGIO, 4, currentSong);
            } else if (key == 1) {
                currentSong = wheelsOnTheBus;
                currentSongTotalLength = WHEELS_ON_THE_BUS_LENGTH;
                myPlayerPtr->play_melody(T_ADAGIO, 5, currentSong);
            } else if (key == 2) {
                currentSong = fiveLittleDucks;
                currentSongTotalLength = FIVE_LITTLE_DUCKS_LENGTH;
                myPlayerPtr->play_melody(T_ADAGIO, FIVE_LITTLE_DUCKS_LENGTH, currentSong);
            }
        }
    } else {
        if (mode == TURN_USER) {
            trellisPtr->pixels.set(key, COLOR_BLACK);
            myPlayerPtr->stop();

            // check if that was actually correct
            int note = kepad_pos_to_note.at(key);
            int correct_note = currentSong[(current_position_in_song + 1) * 2];
            if (note != correct_note) {
                mode = TURN_INCORRECT;
            } else if ((current_position_in_song + 1) * 2 + 2 == currentSongTotalLength) {
                mode = TURN_FINISH;
            } else {
                current_position_in_song++;
            }
        } else if (mode == SONG_SELECTION) {
            myPlayerPtr->stop();
        }
    }
}

void show_song_selection() {
    trellisPtr->pixels.set(0, COLOR_WHITE);
    trellisPtr->pixels.set(1, COLOR_RED);
    trellisPtr->pixels.set(2, COLOR_GREEN);
    // trellisPtr->pixels.set(3, COLOR_BLUE);
    // trellisPtr->pixels.set(4, COLOR_ORANGE);

    // for (uint8_t i = 0; i < 5; i++) {
    //     trellisPtr -> pixels.set(i, COLOR_GREEN);
    // }
}

void turn_keyboard_leds_off() {
    for (uint8_t i = 0; i < 16; i++) {
        trellisPtr->pixels.set(i, COLOR_BLACK);
    }
}

void show_user_failed_pad() {
    trellisPtr->pixels.set(0, COLOR_RED);
    trellisPtr->pixels.set(3, COLOR_RED);
    trellisPtr->pixels.set(5, COLOR_RED);
    trellisPtr->pixels.set(6, COLOR_RED);
    trellisPtr->pixels.set(9, COLOR_RED);
    trellisPtr->pixels.set(10, COLOR_RED);
    trellisPtr->pixels.set(12, COLOR_RED);
    trellisPtr->pixels.set(15, COLOR_RED);
}

void show_user_success_pad() {
    trellisPtr->pixels.set(7, COLOR_GREEN);
    trellisPtr->pixels.set(8, COLOR_GREEN);
    trellisPtr->pixels.set(10, COLOR_GREEN);
    trellisPtr->pixels.set(13, COLOR_GREEN);
}


void game_turn_computer() {
    current_position_in_song++;

    const int note = currentSong[current_position_in_song * 2];
    const uint8_t key = note_to_kepad_pos.at(note);
    const int colour = keypad_pos_to_color.at(key);
    trellisPtr->pixels.set(key, colour);
    trellisPtr->pixels.show();
    myPlayerPtr->tone(note);
    const int duration = abs(currentSong[current_position_in_song * 2 + 1]);
    if (duration == 8) {
        sleep_ms(200);
    } else if (duration == 4) {
        sleep_ms(400);
    } else if (duration == 2) {
        sleep_ms(800);
    }

    myPlayerPtr->stop();
    trellisPtr->pixels.set(key, COLOR_BLACK);
    trellisPtr->pixels.show();
    sleep_ms(200);
}
