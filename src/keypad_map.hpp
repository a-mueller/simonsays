//
// Created by abmue on 21/01/24.
//

#ifndef KEYPAD_MAP_HPP
#define KEYPAD_MAP_HPP
#include <cstdint>
#include <map>
#include <memory>
#include <neopixel.hpp>
#include <pitches.h>

const std::map<int, uint8_t> note_to_kepad_pos = {
    {NOTE_C4, 0},
    {NOTE_D4, 1},
    {NOTE_E4, 2},
    {NOTE_F4, 3},
    {NOTE_FS4, 3},
    {NOTE_G4, 4},
    {NOTE_A4, 5},
    {NOTE_B4, 6},
    {NOTE_C5, 7},
    {NOTE_D5, 8},
    {NOTE_E5, 9},
    {NOTE_F5, 10},
    {NOTE_G5, 11},
    {NOTE_A5, 12},
    {NOTE_B5, 13},
    {NOTE_C6, 14},
    {NOTE_D6, 15}
};

const std::map<uint8_t, int> kepad_pos_to_note = {
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

const std::map<uint8_t, int> keypad_pos_to_color = {
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
