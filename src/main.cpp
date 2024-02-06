#include <neotrellis.hpp>
// #include <pico_tone.hpp>

#include "pico/stdlib.h"
#include <stdio.h>
#include <sys/stat.h>

#include "io.hpp"
#include "config.hpp"
#include "game.hpp"
#include "keypad_map.hpp"
#include "melody_player.hpp"
#include "songs.hpp"


enum Mode {
    SONG_SELECTION,
    TURN_USER,
    TURN_COMPUTER,
    TURN_INCORRECT,
    TURN_FINISH
};

enum SoundState {
    ON, OFF
};

Mode mode = SONG_SELECTION;
SoundState soundState = ON;

Game game;
MelodyPlayer melodyPlayer;

NeoTrellis *trellisPtr;
Tone *myPlayerPtr;


void keypad_handler(uint8_t key, Keypad::Edge edge);


int main() {
    stdio_init_all();

    /* Init the trellis library */
    NeoTrellis trellis(PICO_I2C, PIN_SDA, PIN_SCL);
    trellis.init();
    trellis.keypad.set_callback(keypad_handler);
    trellisPtr = &trellis;

    /* Init the sound library */
    Tone myPlayer(PIN_SOUND_OUT, 50, 0, 20, 0, 20, 0, 10);
    myPlayer.init(TONE_NON_BLOCKING, false);
    myPlayerPtr = &myPlayer;

    /* Setup the button and led pins */
    init_io();

    /* Indicate to the user that we are starting up */
    indicate_startup(&trellis);


    /* Make sure we have a song loaded in case the user doesn't select one */
    game.init(happyBirthday, HAPPY_BIRTHDAY_LENGTH);

    /* Start in song selection mode */
    show_song_selection_mode(&trellis);

    while (true) {
        trellis.keypad.update();

        if (gpio_get(PIN_BTN_SONG_SELECT)) {
            // we want to switch mode
            if (mode != SONG_SELECTION) {
                mode = SONG_SELECTION;
                // turn the led on
                show_song_selection_mode(&trellis);
            } else {
                melodyPlayer.stop(&myPlayer);
                melodyPlayer.reset();

                mode = TURN_COMPUTER;
                game.reset();
                // turn the leds and trellis off
                show_computer_turn();
                all_trellis_off(&trellis);
            }

            // wait till the button is up again before we go on with the loop
            while (gpio_get(PIN_BTN_SONG_SELECT)) {
                sleep_ms(10);
            }
        } else if (gpio_get(PIN_BTN_SOUND_TOGGLE)) {
            if (soundState == ON) {
                switch_sound(false);
                soundState = OFF;
            } else {
                switch_sound(true);
                soundState = ON;
            }
            // wait till the button is up again before we go on with the loop
            while (gpio_get(PIN_BTN_SOUND_TOGGLE)) {
                sleep_ms(10);
            }
        } else {
            // game play
            if (mode == TURN_INCORRECT) {
                show_trellis(&trellis);

                // make a red X
                show_user_failed_pad(&trellis);

                // reset game
                game.reset();

                sleep_ms(1000);
                all_trellis_off(&trellis);

                mode = TURN_COMPUTER;
                show_computer_turn();
            } else if (mode == TURN_FINISH) {
                show_trellis(&trellis);

                // make a green tick
                show_user_success_pad(&trellis);

                // reset game
                game.reset();
                mode = TURN_COMPUTER;

                sleep_ms(2000);
                all_trellis_off(&trellis);
                show_computer_turn();
            } else if (mode == TURN_COMPUTER) {
                if (!game.reachedTargetPosition()) {
                    game.runComputerTurn(&trellis, &myPlayer);
                } else {
                    mode = TURN_USER;
                    show_user_turn();
                    game.swichTurn(false);
                }
            } else if (mode == TURN_USER) {
                show_trellis(&trellis);

                if (game.reachedTargetPosition()) {
                    mode = TURN_COMPUTER;
                    game.swichTurn(true);
                    show_computer_turn();
                    sleep_ms(500);
                }
            } else if (mode == SONG_SELECTION) {
                show_trellis(&trellis);
                melodyPlayer.loop(&myPlayer);
            }

            sleep_ms(50);
        }
    }
}


void keypad_handler(uint8_t key, Keypad::Edge edge) {
    if (edge == Keypad::Edge::RISING) {
        if (mode == TURN_USER) {
            const int colour = KEYPAD_POS_TO_COLOUR.at(key);
            const int correctNote = game.noteAtNextPosition();

            if (NOTE_TO_KEYPAD_POS.at(correctNote) == key) {
                myPlayerPtr->tone(correctNote);
            } else {
                myPlayerPtr->tone(KEYPAD_POS_TO_DEFAULT_NOTE.at(key));
            }

            trellisPtr->pixels.set(key, colour);
        } else if (mode == SONG_SELECTION) {
            all_trellis_off(trellisPtr, false);
            trellisPtr->pixels.set(key, COLOR_WHITE);
            if (key == 0) {
                game.init(happyBirthday, HAPPY_BIRTHDAY_LENGTH);
                melodyPlayer.init(happyBirthday, HAPPY_BIRTHDAY_LENGTH);
                melodyPlayer.start();
            } else if (key == 1) {
                game.init(wheelsOnTheBus, WHEELS_ON_THE_BUS_LENGTH);
                melodyPlayer.init(wheelsOnTheBus, WHEELS_ON_THE_BUS_LENGTH);
                melodyPlayer.start();
            } else if (key == 2) {
                game.init(fiveLittleDucks, FIVE_LITTLE_DUCKS_LENGTH);
                melodyPlayer.init(fiveLittleDucks, FIVE_LITTLE_DUCKS_LENGTH);
                melodyPlayer.start();
            } else if (key == 3) {
                game.init(auldLangSyne, AULD_LANG_SYNE_LENGTH);
                melodyPlayer.init(auldLangSyne, AULD_LANG_SYNE_LENGTH);
                melodyPlayer.start();
            }
        }
    } else {
        if (mode == TURN_USER) {
            trellisPtr->pixels.set(key, COLOR_BLACK);
            myPlayerPtr->stop();

            // check if that was actually correct
            const int correct_note = game.noteAtNextPosition();
            const uint8_t correct_key = NOTE_TO_KEYPAD_POS.at(correct_note);

            if (correct_key != key) {
                mode = TURN_INCORRECT;
            } else {
                game.advanceToNextNote();
                if (game.reachedEnd()) {
                    mode = TURN_FINISH;
                }
            }
        } else if (mode == SONG_SELECTION) {
            show_song_selection_mode(trellisPtr, false);
            melodyPlayer.stop(myPlayerPtr);
            // myPlayerPtr->stop();
        }
    }
}
