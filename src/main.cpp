#include <neotrellis.hpp>
// #include <pico_tone.hpp>

#include "pico/stdlib.h"
#include <stdio.h>
#include <sys/stat.h>

#include "io.hpp"
#include "config.hpp"
#include "game.hpp"
#include "keypad_map.hpp"
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
                mode = TURN_COMPUTER;
                game.reset();
                // turn the leds and trellis off
                all_leds_off();
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
            } else if (mode == TURN_FINISH) {
                show_trellis(&trellis);

                // make a green tick
                show_user_success_pad(&trellis);

                // reset game
                game.reset();
                mode = TURN_COMPUTER;

                sleep_ms(1000);
                all_trellis_off(&trellis);
            } else if (mode == TURN_COMPUTER) {
                if (!game.reachedTargetPosition()) {
                    game.runComputerTurn(&trellis, &myPlayer);
                } else {
                    mode = TURN_USER;
                    game.swichTurn(false);
                }
            } else if (mode == TURN_USER) {
                show_trellis(&trellis);

                if (game.reachedTargetPosition()) {
                    mode = TURN_COMPUTER;
                    game.swichTurn(true);
                    sleep_ms(500);
                }
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
            if (key == 0) {
                game.init(happyBirthday, HAPPY_BIRTHDAY_LENGTH);
                myPlayerPtr->play_melody(T_ADAGIO, 4, happyBirthday);
            } else if (key == 1) {
                game.init(wheelsOnTheBus, WHEELS_ON_THE_BUS_LENGTH);
                myPlayerPtr->play_melody(T_ADAGIO, 5, wheelsOnTheBus);
            } else if (key == 2) {
                game.init(fiveLittleDucks, FIVE_LITTLE_DUCKS_LENGTH);
                myPlayerPtr->play_melody(T_ADAGIO, FIVE_LITTLE_DUCKS_LENGTH, fiveLittleDucks);
            }
        }
    } else {
        if (mode == TURN_USER) {
            trellisPtr->pixels.set(key, COLOR_BLACK);
            myPlayerPtr->stop();

            // check if that was actually correct
            const int note = KEYPAD_POS_TO_DEFAULT_NOTE.at(key);
            const int correct_note = game.noteAtNextPosition();
            if (note != correct_note) {
                mode = TURN_INCORRECT;
            } else {
                game.advanceToNextNote();
                if (game.reachedEnd()) {
                    mode = TURN_FINISH;
                }
            }
        } else if (mode == SONG_SELECTION) {
            myPlayerPtr->stop();
        }
    }
}
