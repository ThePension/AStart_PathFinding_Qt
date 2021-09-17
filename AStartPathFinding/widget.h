#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>

#include "cell.h"

class Widget : public QGraphicsView
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QGraphicsScene * mainScene;

    int rows = 20;
    int cols = 20;

    int height = 601;
    int width = 601;

    Cell * lstCells[20][20];

    QTimer * mainTimer;

    int compareCells(Cell * c1, Cell * c2);

    QList<Cell *> openList;
    QList<Cell *> closedList;

    bool stopLooping = false;

public slots:
    void findShortestPath();
    void update();
};
#endif // WIDGET_H
