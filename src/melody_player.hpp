//
// Created by abmue on 05/02/24.
//

#ifndef MELODY_PLAYER_HPP
#define MELODY_PLAYER_HPP
#include <pico_tone.hpp>
#include <pico/types.h>


class MelodyPlayer {
private:
    bool playing = false;
    int currentPositionInSong = -1;
    int *song = nullptr;
    int songTotalLength = -1;
    absolute_time_t nextTone;

public:
    MelodyPlayer() = default;

    void init(int *song, int songTotalLength);
    void start();
    void stop(Tone *player);

    void reset();

    [[nodiscard]] bool reachedEnd() const;

    void loop(Tone *player);

};

#endif //MELODY_PLAYER_HPP
