#ifndef GAME_HPP
#define GAME_HPP

#include <neotrellis.hpp>
#include <pico_tone.hpp>

class Game {
private:
    int targetPositionInSong = -1;
    int currentPositionInSong = -1;
    int *song = nullptr;
    int songTotalLength = -1;

public:
    Game() = default;

    void init(int *song, int songTotalLength);

    void reset();

    [[nodiscard]] bool reachedTargetPosition() const;

    [[nodiscard]] bool reachedEnd() const;

    void runComputerTurn(NeoTrellis *trellis, Tone *player);

    void swichTurn(bool increaseTarget);

    [[nodiscard]] int noteAtNextPosition() const;

    void advanceToNextNote();

};

#endif //GAME_HPP
