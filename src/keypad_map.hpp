#ifndef KEYPAD_MAP_HPP
#define KEYPAD_MAP_HPP
#include <cstdint>
#include <map>
#include <memory>
#include <neopixel.hpp>
#include <pitches.h>

/**
 * \brief Given a note what trellis key is it.
 * Given we don't have too many, we map the S versions to the same keys, so multiple notes map to the same key
 */
const std::map<int, uint8_t> NOTE_TO_KEYPAD_POS = {
    {NOTE_C4, 0},
    {NOTE_CS4, 0},
    {NOTE_D4, 1},
    {NOTE_DS4, 1},
    {NOTE_E4, 2},
    {NOTE_F4, 3},
    {NOTE_FS4, 3},
    {NOTE_G4, 4},
    {NOTE_GS4, 4},
    {NOTE_A4, 5},
    {NOTE_AS4, 5},
    {NOTE_B4, 6},
    {NOTE_C5, 7},
    {NOTE_CS5, 7},
    {NOTE_D5, 8},
    {NOTE_DS5, 8},
    {NOTE_E5, 9},
    {NOTE_F5, 10},
    {NOTE_FS5, 10},
    {NOTE_G5, 11},
    {NOTE_GS5, 11},
    {NOTE_A5, 12},
    {NOTE_AS5, 12},
    {NOTE_B5, 13},
    {NOTE_C6, 14},
    {NOTE_CS6, 14},
    {NOTE_D6, 15},
    {NOTE_DS6, 15}
};

/**
 * \brief This is relevant when the user presses a button that is wrong. We don't really know which note to play
 * so we play the default one for that key
 */
const std::map<uint8_t, int> KEYPAD_POS_TO_DEFAULT_NOTE = {
    { 0, NOTE_C4},
    {1, NOTE_D4},
    { 2,NOTE_E4},
    { 3,NOTE_F4},
    { 4,NOTE_G4},
    { 5,NOTE_A4},
    { 6,NOTE_B4},
    { 7,NOTE_C5},
    { 8,NOTE_D5},
    { 9,NOTE_E5},
    { 10,NOTE_F5},
    { 11,NOTE_G5},
    { 12,NOTE_A5},
    { 13,NOTE_B5},
    { 14,NOTE_C6},
    { 15,NOTE_D6},
};

/**
 * \brief Maps the trellis key to a color. All Ranges repeat the colors, so C mid and C high will have the same color
 */
const std::map<uint8_t, int> KEYPAD_POS_TO_COLOUR = {
    {0, COLOR_RED},
    {1, COLOR_BLUE},
    {2, COLOR_ORANGE},
    {3, COLOR_PINK},
    {4, COLOR_WHITE},
    {5, COLOR_GREEN},
    {6, COLOR_PURPLE},
    {7, COLOR_RED},
    {8, COLOR_BLUE},
    {9, COLOR_ORANGE},
    {10, COLOR_PINK},
    {11, COLOR_WHITE},
    {12, COLOR_GREEN},
    {13, COLOR_PURPLE},
    {14, COLOR_RED},
    {15, COLOR_BLUE}
};

#endif //KEYPAD_MAP_HPP
