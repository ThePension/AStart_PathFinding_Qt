#ifndef CELL_H
#define CELL_H

#include <QPushButton>
#include <QKeyEvent>
#include <QDebug>

class Cell : public QPushButton
{
public:
    Cell(int x, int y, int w, int h, QWidget * parent = 0);
    Cell * previousCell = nullptr;
    int posX, posY;
    double cost = 0, heuristic = 0;
    bool isWall = false, isClosed = false, isOpen = false, isOnPath = false;
    int w, h;
    void update();
protected:
    void mousePressEvent(QMouseEvent  * e);
};

#endif // CELL_H
