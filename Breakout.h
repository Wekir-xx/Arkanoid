#pragma once
#include <qwidget.h>
#include "Item.h"

class Breakout :
                 public QWidget
{
public:
    Breakout(QWidget* parent = nullptr);
    ~Breakout();
private:
    static constexpr int WIDTH = 300;
    static constexpr int HEIGHT = 400;
    static constexpr int PADDLE_STEP = 12;
    static constexpr int BRICKS_IN_WIDTH = 6;
    static constexpr int BRICKS_IN_HEIGHT = 5;
    static constexpr int PADDLE_Y_FROM_BOTTOM_BORDER = 50;
    static constexpr int BRICK_Y_FROM_TOP_BORDER = 50;
    static constexpr int DELAY = 10;
    static constexpr int BRICKIE_SIZE = 40;
    static constexpr int BALL_SIZE = 20;
    static constexpr int PADDLE_SIZE = 40;

    void newGame();
    void startGame();
    void pauseGame();
    void ballMove();
    void checkBallTouch();
    void paintGameField(QPainter* painter);
    void paintText(QPainter* painter, qreal x, qreal y, QString text, QFont font, QColor pen, QColor brush);

    void paintEvent(QPaintEvent* event);
    void timerEvent(QTimerEvent* event);
    void keyPressEvent(QKeyEvent* event);

    Item* paddle;
    QList<Item*> bricks;
    Item* ball;
    int xdir;
    int ydir;
    int timerID;
    int score;
    bool game_over;
    bool paused;
    bool new_game;
};
