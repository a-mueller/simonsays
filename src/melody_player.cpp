//
// Created by abmue on 05/02/24.
//

#include "melody_player.hpp"

#include <cmath>
#include <pico/time.h>


void MelodyPlayer::init(int *song, int songTotalLength) {
    this->song = song;
    this->songTotalLength = songTotalLength;
    reset();
}

void MelodyPlayer::reset() {
    this->currentPositionInSong = -1;
    this->playing = false;
}

void MelodyPlayer::start() {
    this->nextTone = absolute_time_t();
    this->playing = true;
}

void MelodyPlayer::stop(Tone *player) {
    this->playing = false;
    player -> stop();
}

void MelodyPlayer::loop(Tone *player) {
    if (playing) {
        absolute_time_t curTime = get_absolute_time();
        if (absolute_time_diff_us(curTime, nextTone) <= 0) {
            currentPositionInSong++;
            if (currentPositionInSong >= songTotalLength) {
                currentPositionInSong = 0; // restart from the beginning
            }

            const int note = song[currentPositionInSong * 2];
            player->tone(note);

            const int toneDuration = abs(song[currentPositionInSong * 2 + 1]);
            if (toneDuration == 8) {
                nextTone = delayed_by_ms(curTime, 200);
            } else if (toneDuration == 4) {
                nextTone = delayed_by_ms(curTime, 400);
            } else if (toneDuration == 2) {
                nextTone = delayed_by_ms(curTime, 800);
            }
        }

    }
}

bool MelodyPlayer::reachedEnd() const {
    return false;
}






