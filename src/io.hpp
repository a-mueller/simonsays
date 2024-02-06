//
// Created by abmue on 24/01/24.
//

#ifndef IO_HPP
#define IO_HPP
#include <neotrellis.hpp>

/**
 * Init buttons / leds / sound
 */
void init_io();

/**
 * Visual feedback to the user we are starting up
 */
void indicate_startup(NeoTrellis* trellis);

/**
 * Switch all leds off (the ones that are not on the NeoTrellis board)
 */
void all_leds_off();

/**
 * \brief Switch all leds of that are involved in mode (turns / song selection)
 */
void all_mode_leds_off();

/**
 * \brief indicates it is the user's turn to press buttons
 */
void show_user_turn();

/**
 * \brief inidictates it is the computers turn to press buttons
 */
void show_computer_turn();

/**
 * Show the song selection "screen"
 */
void show_song_selection_mode(NeoTrellis* trellis, bool show = true);

/**
 * Turn all trellis off (black)
 */
void all_trellis_off(NeoTrellis* trellis, bool show = true);

/**
 * Show the user that they failed via the trellis pad
 */
void show_user_failed_pad(NeoTrellis *trellis);

/**
 * Show the user that they completed the whole song via the trellis pad
 */
void show_user_success_pad(NeoTrellis *trellis);

/**
 * Makes sure the latest trellis state is flushed
 */
void show_trellis(NeoTrellis *trellis);

/**
 * \brief toggles the sound on/off, only works if the
 */
void switch_sound(bool on);

#endif //IO_HPP
