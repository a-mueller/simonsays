#include "game.hpp"

#include <cstdint>

#include "io.hpp"
#include "keypad_map.hpp"

void Game::init(int *song, const int songTotalLength) {
    this->song = song;
    this->songTotalLength = songTotalLength;
    this->reset();
}

void Game::reset() {
    this->currentPositionInSong = -1;
    this->targetPositionInSong = 0;
}

bool Game::reachedTargetPosition() const {
    return currentPositionInSong >= targetPositionInSong;
}

bool Game::reachedEnd() const {
    return currentPositionInSong * 2 + 1 >= songTotalLength;
}


void Game::swichTurn(const bool increaseTarget) {
    currentPositionInSong = -1;
    if (increaseTarget) {
        targetPositionInSong++;
    }
}

int Game::noteAtNextPosition() const {
    return song[(currentPositionInSong + 1) * 2];
}


void Game::advanceToNextNote() {
    currentPositionInSong++;
}


void Game::runComputerTurn(NeoTrellis *trellis, Tone *player) {
    advanceToNextNote();

    const int note = song[currentPositionInSong * 2];
    const uint8_t key = note_to_kepad_pos.at(note);
    const int colour = keypad_pos_to_color.at(key);
    trellis->pixels.set(key, colour);
    trellis->pixels.show();
    player->tone(note);
    const int duration = abs(song[currentPositionInSong * 2 + 1]);
    if (duration == 8) {
        sleep_ms(200);
    } else if (duration == 4) {
        sleep_ms(400);
    } else if (duration == 2) {
        sleep_ms(800);
    }

    player->stop();
    all_trellis_off(trellis);
    sleep_ms(200);
}
