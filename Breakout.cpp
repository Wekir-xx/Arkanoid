#include "Breakout.h"
#include <qpainter.h>
#include <QKeyEvent>
#include <QPainterPath>

Breakout::Breakout(QWidget* parent)
    : QWidget{parent}, paddle{}, bricks{}, ball{}, timerID{}
{
    this->setFixedSize(WIDTH, HEIGHT);
    QBrush brush;
    brush.setTextureImage (QImage(":/images/back.png"));
    QPalette palete;
    palete.setBrush (QPalette::Window, brush);
    this->setPalette(palete);
    newGame();
}

Breakout::~Breakout()
{
    delete paddle;
    for (auto& brick : bricks)
        delete brick;
    bricks.clear();
    delete ball;
}

void Breakout::newGame()
{
    if (paddle != nullptr)
        delete paddle;
    for (auto& brick : bricks)
        if(brick!=nullptr)
        {
            delete brick;
            brick = nullptr;
        }
    bricks.clear();
    if (ball != nullptr)
        delete ball;
    if (timerID != 0)
        killTimer(timerID);

    timerID = 0;
    score = 0;
    game_over = false;
    paused = false;
    new_game = true;

    paddle = new Item();
    QImage paddle_image(":/images/paddle.png");
    paddle_image = paddle_image.scaledToWidth (PADDLE_SIZE);
    paddle->setImage(paddle_image);
    paddle->setCords(QPoint(WIDTH / 2 - paddle_image.width() / 2, HEIGHT - PADDLE_Y_FROM_BOTTOM_BORDER));
    QImage brick_image(":/images/brickie.png");
    brick_image = brick_image.scaledToWidth (BRICKIE_SIZE);

    for (int h = 0; h < BRICKS_IN_HEIGHT; h++)
    {
        int y = BRICK_Y_FROM_TOP_BORDER + h * brick_image.height();
        for (int w = 0; w < BRICKS_IN_WIDTH; w++)
        {
            bricks.push_back(new Item());
            bricks.at(bricks.size() - 1)->setImage(brick_image);
            int x;
            if (BRICKS_IN_WIDTH % 2 == 0)
                x = this->width() / 2 - BRICKS_IN_WIDTH / 2 * brick_image.width() + w * brick_image.width();
            else
                x = this->width() / 2 - BRICKS_IN_WIDTH / 2 * brick_image.width() + w * brick_image.width() - brick_image.width() / 2;
            bricks.at(bricks.size() - 1)->setCords(QPoint(x, y));
        }
    }
    ball = new Item();
    ball->setImage(QImage(":/images/ball.png").scaledToWidth (BALL_SIZE));
    ball->setCords(QPoint(paddle->getCords().x() + paddle_image.width() / 2, paddle->getCords().y() - paddle_image.height() / 2));
    xdir = 1;
    ydir = -1;
}

void Breakout::startGame()
{
    if (timerID == 0)
    {
        timerID = startTimer(DELAY);
        paused = false;
        new_game = false;
    }
}

void Breakout::pauseGame()
{
    killTimer(timerID);
    timerID = 0;
    paused = true;
}

void Breakout::ballMove()
{
    ball->setCords(QPoint(ball->getCords().x() + xdir, ball->getCords().y() + ydir));
}

void Breakout::checkBallTouch()
{
    if (ball->getCords().x() + ball->getImage().width() > this->width())
        xdir = -1;
    if (ball->getCords().x() < 0)
        xdir = 1;
    if (ball->getCords().y() < 0)
        ydir = 1;
    if (ball->getCords().y() + ball->getImage().width() > this->height())
    {
        killTimer(timerID);
        game_over = true;
        return;
    }
    if ((ball->getCords().y() + ball->getImage().height() > paddle->getCords().y()) &&
        (ball->getCords().y() + ball->getImage().height() < paddle->getCords().y() + paddle->getImage().height()) &&
        ball->getCords().x() + ball->getImage().width() / 2 > paddle->getCords().x() &&
        ball->getCords().x() - ball->getImage().width() / 2 < paddle->getCords().x() + paddle->getImage().width())
    {
        ydir = -1;
        if (ball->getCords().x() + ball->getImage().width() / 2 > paddle->getCords().x() + paddle->getImage().width() / 2 - ball->getImage().width() / 2 &&
            ball->getCords().x() + ball->getImage().width() / 2 < paddle->getCords().x() + paddle->getImage().width() / 2 + ball->getImage().width() / 2)
        {
            xdir = 0;
        }
        else if (ball->getCords().x() + ball->getImage().width() / 2 > paddle->getCords().x() + paddle->getImage().width() / 2)
        {
            xdir = 1;
        }
        else
        {
            xdir = -1;
        }
    }
    for (size_t brick = 0; brick < bricks.size(); brick++)
    {
        if (ydir == -1 && ball->getCords().y() < bricks.at(brick)->getCords().y() + bricks.at(brick)->getImage().height() &&
            ball->getCords().y() > bricks.at(brick)->getCords().y() &&
            ball->getCords().x() + ball->getImage().width() > bricks.at(brick)->getCords().x() &&
            ball->getCords().x() < bricks.at(brick)->getCords().x() + bricks.at(brick)->getImage().width())
        {
            ydir = 1;
            score += 10;
            if (ball->getCords().x() + ball->getImage().width() / 2 < bricks.at(brick)->getCords().x() + bricks.at(brick)->getImage().width() / 2)
                xdir = -1;
            else
                xdir = 1;
            Item* temp = bricks.at(brick);
            bricks.remove(brick);
            delete temp;
        }
        else if (ydir == 1 && ball->getCords().y() + ball->getImage().height() > bricks.at(brick)->getCords().y() &&
                 ball->getCords().y() + ball->getImage().height() < bricks.at(brick)->getCords().y() + bricks.at(brick)->getImage().height() &&
                 ball->getCords().x() + ball->getImage().width() > bricks.at(brick)->getCords().x() &&
                 ball->getCords().x() < bricks.at(brick)->getCords().x() + bricks.at(brick)->getImage().width())
        {
            ydir = -1;
            score += 10;
            if (ball->getCords().x() + ball->getImage().width() / 2 < bricks.at(brick)->getCords().x() + bricks.at(brick)->getImage().width() / 2)
                xdir = -1;
            else
                xdir = 1;
            Item* temp = bricks.at(brick);
            bricks.remove(brick);
            delete temp;
        }
    }
}

void Breakout::paintGameField(QPainter* painter)
{
    painter->drawImage(paddle->getCords(), paddle->getImage());
    for (const auto& brick : bricks)
        painter->drawImage(brick->getCords(), brick->getImage());
    painter->drawImage(ball->getCords(), ball->getImage());
}

void Breakout::paintText(QPainter* painter, qreal x, qreal y, QString text, QFont font, QColor pen, QColor brush)
{
    QFontMetrics metrics(font);
    qreal text_width = metrics.horizontalAdvance(text);
    QPainterPath path;
    path.addText(x, y, font, text);
    painter->setPen(QPen(pen));
    painter->setBrush(QBrush(brush));
    painter->drawPath(path);
}

void Breakout::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter* painter = new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing);
    if (!game_over && bricks.size() > 0)
    {
        paintGameField(painter);
        QString text("Score: %1");
        text = text.arg(score);
        QFont font("Times New Roman", 14);
        QFontMetrics metrics(font);
        qreal text_width = metrics.horizontalAdvance(text);
        qreal text_height = metrics.height();
        paintText(painter, this->width() / 2 - text_width / 2, PADDLE_Y_FROM_BOTTOM_BORDER / 2, text, font, QColor("red"), QColor("red"));
        if (new_game)
        {
            QString text("To game begin click \"Space\"");
            QFont font("Times New Roman", 10);
            QFontMetrics metrics(font);
            qreal text_width = metrics.horizontalAdvance(text);
            qreal text_height = metrics.height();
            paintText(painter, this->width() / 2 - text_width / 2, this->height() - static_cast<qreal>(PADDLE_Y_FROM_BOTTOM_BORDER) / 2 + text_height, text, font, QColor("orange"), QColor("black"));
        }
    }
    else if (bricks.size() > 0)
    {
        QString text("Game Over");
        QString s_score("your score: %1");
        s_score = s_score.arg(score);
        QFont text_font("Times New Roman", 35);
        QFont score_font("Times New Roman", 14);
        QFontMetrics text_metrics(text_font);
        QFontMetrics score_metrics(score_font);
        qreal text_width = text_metrics.horizontalAdvance(text);
        qreal score_width = score_metrics.horizontalAdvance(s_score);
        qreal score_height = score_metrics.height();
        paintText(painter, this->width() / 2 - text_width / 2, this->height() / 2, text, text_font, QColor("red"), QColor("black"));
        paintText(painter, this->width() / 2 - score_width / 2, this->height() / 2 + score_height, s_score, score_font, QColor("red"), QColor("red"));
    }
    else
    {
        QString text("You win!");
        QString score("your score: %1");
        score = score.arg(score);
        QFont text_font("Times New Roman", 35);
        QFont score_font("Times New Roman", 14);
        QFontMetrics text_metrics(text_font);
        QFontMetrics score_metrics(score_font);
        qreal text_width = text_metrics.horizontalAdvance(text);
        qreal score_width = score_metrics.horizontalAdvance(score);
        qreal score_height = score_metrics.height();
        paintText(painter, this->width() / 2 - text_width / 2, this->height() / 2, text, text_font, QColor("black"), QColor("red"));
        paintText(painter, this->width() / 2 - score_width / 2, this->height() / 2 + score_height, score, score_font, QColor("red"), QColor("red"));
    }
    if (paused)
    {
        QString text("To game continue click \"Space\", or \"Escape\"");
        QFont font("Times New Roman", 10);
        QFontMetrics metrics(font);
        qreal text_width = metrics.horizontalAdvance(text);
        qreal text_height = metrics.height();
        paintText(painter, this->width() / 2 - text_width / 2, this->height() - static_cast<qreal>(PADDLE_Y_FROM_BOTTOM_BORDER) / 2 + text_height, text, font, QColor("orange"), QColor("black"));
    }
    delete painter;
}

void Breakout::timerEvent(QTimerEvent* event)
{
    Q_UNUSED(event);
    ballMove();
    checkBallTouch();
    this->repaint();
}

void Breakout::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    if (key == Qt::Key_Left)
    {
        if (paddle->getCords().x() > 0 && !new_game)
        {
            QPoint paddle_cords = paddle->getCords();
            paddle_cords.setX(paddle_cords.x() - PADDLE_STEP);
            paddle->setCords(paddle_cords);
        }
    }
    else if (key == Qt::Key_Right)
    {
        if (paddle->getCords().x() < WIDTH - paddle->getImage().width() && !new_game)
        {
            QPoint paddle_cords = paddle->getCords();
            paddle_cords.setX(paddle_cords.x() + PADDLE_STEP);
            paddle->setCords(paddle_cords);
        }
    }
    else if (key == Qt::Key_Space)
    {
        startGame();
    }
    else if (key == Qt::Key_Escape && !new_game)
    {
        if (timerID != 0)
            pauseGame();
        else
            startGame();
    }
    else if (key == Qt::Key_N)
        newGame();
    this->repaint();
}
