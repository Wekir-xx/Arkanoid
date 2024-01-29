#pragma once
#include <qpoint.h>
#include <qimage.h>

class Item
{
public:
    Item()
        : image(QImage()), cords(QPoint(0, 0))
    {}
public:
    QPoint getCords() { return cords; }
    void setCords(QPoint cords) { this->cords = cords; }
    QImage& getImage() { return image; }
    void setImage(QImage image) { this->image = image; }
private:
    QPoint cords;
    QImage image;
};
