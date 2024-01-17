#include <neotrellis.hpp>
#include <pico_tone.hpp>

#include "pico/stdlib.h"
#include <stdio.h>
#include <sys/stat.h>

#define PICO_I2C i2c0
#define PICO_SDA 0
#define PICO_SCL 1

NeoTrellis* trellisPtr;
void keypad_handler(uint8_t key, Keypad::Edge edge);


int main() {
    stdio_init_all();

    NeoTrellis trellis(PICO_I2C, PICO_SDA, PICO_SCL);
    trellisPtr = &trellis;

    Tone myPlayer(17,50,0,20,0,20,0,10);
    myPlayer.init(TONE_NON_BLOCKING, false);
    myPlayer.tone(NOTE_A4, 0.5);

    sleep_ms(500);

    trellis.init();


    // Test NeoPixels
    for (uint8_t i = 0; i < 16; i++) {
        trellis.pixels.set(i, COLOR_WHITE);
        trellis.pixels.show();
        sleep_ms(50);
    }
    for (uint8_t i = 0; i < 16; i++) {
        trellis.pixels.set(i, COLOR_BLACK);
        trellis.pixels.show();
        sleep_ms(50);
    }

    trellis.keypad.set_callback(keypad_handler);
    while (true) {
        trellis.keypad.update();
        trellis.pixels.show();
        sleep_ms(20);
    }

    while (true) {
        printf("Running program");
        sleep_ms(2000);
    }
}


void keypad_handler(uint8_t key, Keypad::Edge edge) {
    printf("Key pressed");
    if (edge == Keypad::Edge::RISING) {
        trellisPtr->pixels.set(key, COLOR_WHITE);
    } else {
        trellisPtr->pixels.set(key, COLOR_BLACK);
    }
}
