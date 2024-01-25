//
// Created by abmue on 24/01/24.
//

#ifndef CONFIG_HPP
#define CONFIG_HPP

#define PICO_I2C i2c0
#define PIN_SDA 0
#define PIN_SCL 1

#define PIN_SOUND_OUT 27
// if the Shutdown pin from the D-class amplifier board is connected, it should be pulled to ground by default (off)
// We will set this pin high before playing sound, so make sure there is nothing else on this pin in case you didn't connect it
#define PIN_SOUND_SWITCH 26

#define PIN_LED_USER_TURN 19
#define PIN_LED_COMPUTER_TURN 18

// #define MODE_LED 2
// #define MODE_BTN 3
#define PIN_BTN_LED_SONG_SELECT 5
#define PIN_BTN_SONG_SELECT 4

#define PIN_BTN_LED_SOUND_TOGGLE 6
#define PIN_BTN_SOUND_TOGGLE 7

// It seems that if you don't wait a while after setting the neopixels they behave strangely (especially the one at 0)
#define TRELLIS_UPDATE_DELAY_MS 20

#endif //CONFIG_HPP
