#pragma once

#include "lib/common.h"
#include "lib/oled.h"


const static u8  MIN_LENGTH   = 5;
const static u8  MAX_LENGTH   = 255;
const static u16 GROW_TIME    = 10;
const static u8  BALL_SHRINK  = 15;
const static u8  BALL_PADDING = 5;

enum class GameState {
    None,
    MainMenu,
    Playing,
    GameOver
};

struct Point {
    u8 x;
    u8 y;
    Point(): x(0), y(0) {}
    Point(u8 x, u8 y): x(x), y(y) {}
};

class Game {
private:
    GameState m_state;
    GameState m_nextState;
    u32       m_ticks;
    s8        m_direction_h;
    s8        m_direction_v;
    Point     m_snakeBuf[MAX_LENGTH];
    u8        m_length;
    u16       m_growCounter;
    Point     m_ball;
    u32       m_life;

    void reset();
    void resetBall();

    void updateLoading();
    void updateMainMenu();
    void updatePlaying();
    void updateGameOver();

    void drawLoading();
    void drawMainMenu();
    void drawPlaying();
    void drawGameOver();

public:
    Game():
            m_state(GameState::MainMenu),
            m_nextState(GameState::None),
            m_ticks(0) {
        reset();
    }

    void update();
    void draw();

    void setState(GameState);
    void switchState(GameState);
};