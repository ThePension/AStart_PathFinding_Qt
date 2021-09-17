#include "cell.h"

Cell::Cell(int x, int y, int w, int h, QWidget * parent) : QPushButton(parent)
{
    this->posX = x;
    this->posY = y;
    this->w = w;
    this->h = h;

    this->setGeometry(x * w, y * h, w, h);
    this->show();
}

void Cell::update()
{
    if(this->isWall){
        this->setStyleSheet("background-color: rgb(0, 0, 0); border: 1px solid black;");
    }else if(isClosed){
        this->setStyleSheet("background-color: rgb(255, 0, 0); border: 1px solid black;");
    }else if(isOpen){
        this->setStyleSheet("background-color: rgb(0, 255, 0); border: 1px solid black;");
    }else if(isOnPath){
        this->setStyleSheet("background-color: rgb(0, 0, 255); border: 1px solid black;");
    }else{
        this->setStyleSheet("border: 1px solid black;  background-color: rgb(50, 100, 150);");
    }
}

void Cell::mousePressEvent(QMouseEvent  * e)
{
    qDebug() << cost + heuristic;
    this->isWall = !this->isWall;
}
