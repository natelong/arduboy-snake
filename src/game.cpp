#include "game.h"

#include "Arduino.h"
#include "lib/key.h"
#include "lib/rnd.h"

const static Point BOUNDARY = Point(OLED_SCREEN_CX, OLED_SCREEN_CY);
const static Point ZERO     = Point(0, 0);

void Game::reset() {
    m_direction_h = 1;
    m_direction_v = 0;
    m_length = MIN_LENGTH;
    m_growCounter = 0;
    m_life = 0;

    for (u8 i = 0; i < MAX_LENGTH; i++) {
        m_snakeBuf[i] = Point(OLED_SCREEN_CX/2 - i, OLED_SCREEN_CY/2);
    }

    resetBall();
}

void Game::resetBall() {
    u8 x = (Rnd32() % (OLED_SCREEN_CX - BALL_PADDING)) + BALL_PADDING;
    u8 y = (Rnd32() % (OLED_SCREEN_CY - BALL_PADDING)) + BALL_PADDING;
    m_ball = Point(x, y);
}

void Game::update() {
    m_ticks++;

    if (m_nextState != GameState::None) {
        m_state = m_nextState;
        m_nextState = GameState::None;
    }

    switch (m_state) {
        case GameState::None:     _SystemError("Invalid state in Game::update"); break;
        case GameState::MainMenu: updateMainMenu(); break;
        case GameState::Playing:  updatePlaying(); break;
        case GameState::GameOver: updateGameOver(); break;
    }
}

void Game::updateMainMenu() {
    if (KeyGetTrg() != 0) {
        switchState(GameState::Playing);
    }
}

void Game::updatePlaying() {
    {
        Point head = m_snakeBuf[0];
        s16 bx = m_ball.x;
        s16 by = m_ball.y;
        s16 hx = head.x;
        s16 hy = head.y;

        s16 dx = bx - hx;
        s16 dy = by - hy;
        if (abs(dx) < 2 && abs(dy) < 2) {
            resetBall();
            if (m_length > BALL_SHRINK + MIN_LENGTH) {
                m_length -= BALL_SHRINK;
            } else {
                m_length = MIN_LENGTH;
            }
        }
    }

    if (m_growCounter == GROW_TIME) {
        m_life++;
        m_growCounter = 0;

        if (m_length < MAX_LENGTH) {
            m_length++;
            m_snakeBuf[m_length-1] = m_snakeBuf[m_length-2];
        }
    } else {
        m_growCounter++;
    }

    u8 trg = KeyGetTrg();
    if (trg & KEY_U && m_direction_v !=  1) {
        m_direction_h =  0;
        m_direction_v = -1;
    }
    if (trg & KEY_D && m_direction_v != -1) {
        m_direction_h = 0;
        m_direction_v = 1;
    }
    if (trg & KEY_L && m_direction_h !=  1) {
        m_direction_h = -1;
        m_direction_v =  0;
    }
    if (trg & KEY_R && m_direction_h != -1) {
        m_direction_h = 1;
        m_direction_v = 0;
    }

    if (m_ticks % 2 == 0) {
        for (u8 i = m_length-1; i > 0; i--) {
            m_snakeBuf[i] = m_snakeBuf[i-1];
        }
        m_snakeBuf[0].x += m_direction_h;
        m_snakeBuf[0].y += m_direction_v;
    }

    Point head = m_snakeBuf[0];
    if (head.x > BOUNDARY.x || head.x < ZERO.x || head.y > BOUNDARY.y || head.y < ZERO.y) {
        setState(GameState::GameOver);
    }

    for (u8 i = 1; i < m_length; i++) {
        if (head.x == m_snakeBuf[i].x && head.y == m_snakeBuf[i].y) {
            setState(GameState::GameOver);
        }
    }
}

void Game::updateGameOver() {
    if (KeyGetTrg() != 0) {
        reset();
        setState(GameState::Playing);
    }
}

void Game::draw() {
    switch (m_state) {
        case GameState::None:     _SystemError("Invalid state in Game::draw"); break;
        case GameState::MainMenu: drawMainMenu(); break;
        case GameState::Playing:  drawPlaying(); break;
        case GameState::GameOver: drawGameOver(); break;
    }
}

void Game::drawMainMenu() {
    //                "                     "
    OledDrawStr(7, 2, "SNAKE!");
    OledDrawStr(3, 4, "press any button");
    OledDrawStr(6, 5, "to play");
}

void Game::drawPlaying() {
    u8 x;
    u8 y;

    for (x = 0; x < OLED_SCREEN_CX; x++) { OledDrawDot(x, 0); }
    for (x = 0; x < OLED_SCREEN_CX; x++) { OledDrawDot(x, OLED_SCREEN_CY-1); }
    for (y = 0; y < OLED_SCREEN_CY; y++) { OledDrawDot(0, y); }
    for (y = 0; y < OLED_SCREEN_CY; y++) { OledDrawDot(OLED_SCREEN_CX-1, y); }

    // OledDrawStr(0, 0, "%d", m_life);
    for (u8 i = 0; i < m_length; i++) {
        Point p = m_snakeBuf[i];
        OledDrawDot(p.x, p.y);
    }

    OledDrawDot(m_ball.x,   m_ball.y);
    OledDrawDot(m_ball.x-1, m_ball.y);
    OledDrawDot(m_ball.x+1, m_ball.y);
    OledDrawDot(m_ball.x,   m_ball.y-1);
    OledDrawDot(m_ball.x,   m_ball.y+1);
}

void Game::drawGameOver() {
    drawPlaying();
    OledDrawStr(6, 2, "Game Over!");
    OledDrawStr(3, 4, "press any button");
    OledDrawStr(5, 5, "to play again");
}

void Game::setState(GameState newState) {
    m_state = newState;
}

void Game::switchState(GameState newState) {
    m_nextState = newState;
}