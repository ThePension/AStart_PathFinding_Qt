#include "widget.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>

Widget::Widget(QWidget *parent)
    : QGraphicsView(parent)
{
    // Set view properties
    this->setFixedSize(width, height);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Initialize main scene
    mainScene = new QGraphicsScene(this);
    mainScene->setSceneRect(0, 0, width, height);
    this->setScene(mainScene);

    int w = width / cols;
    int h = height / rows;

    srand(time(NULL));
    // Creation 2d array of cells
    for(int x = 0; x < rows; x++){
        for(int y = 0; y < cols; y++){
            lstCells[x][y] = new Cell(x, y, w, h, this);

            int r = rand() % 10 + 1;
            if(r < 4) lstCells[x][y]->isWall = true;

            lstCells[x][y]->update();

            // lstCells[x][y]->isWall = true;
            mainScene->addWidget(lstCells[x][y]);
        }
    }

    // start cell
    lstCells[0][0]->cost = 0;
    lstCells[0][0]->heuristic = 0;
    lstCells[0][0]->isWall = false;
    lstCells[rows - 1][cols - 1]->isWall = false;

    mainTimer = new QTimer(this);
    mainTimer->start(1000 / 240);

    // connect(mainTimer, &QTimer::timeout, this, &Widget::update);
    connect(mainTimer, &QTimer::timeout, this, &Widget::findShortestPath);

}

Widget::~Widget()
{
}

int Widget::compareCells(Cell *c1, Cell *c2)
{
    if(c1->heuristic < c2->heuristic) return 1;
    else if(c1->heuristic == c2->heuristic) return 0;
    else return -1;
}

void Widget::findShortestPath()
{
    if(stopLooping) return;
    Cell * startCell = lstCells[0][0];
    Cell * endCell = lstCells[rows - 1][cols - 1];
    openList.push_back(startCell);
    if(!openList.isEmpty()){
        Cell * currentCell;
        int winner = 0;
        for(int i = 1; i < openList.length(); i++){
             if(openList.at(i)->cost + openList.at(i)->heuristic < openList.at(winner)->cost + openList.at(winner)->heuristic && openList.at(i) != startCell) {
                winner = i;
              }
        }
        currentCell = openList.takeAt(winner);
        closedList.push_back(currentCell);
        // closedList.push_back(currentCell);
        if(currentCell->posX == endCell->posX && currentCell->posY == endCell->posY){
            // Reconstituer le chemin
            Cell * temp = currentCell;
            while(temp->previousCell != nullptr){
                temp->isOnPath = true;
                temp->isClosed = false;
                temp->isOpen = false;
                temp->update();
                temp = temp->previousCell;
            }
            // Vider les listes
            openList.clear();
            closedList.clear();

            // Terminer
            stopLooping = true;
            return;
        }
        for(int i = -1; i < 2; i++){ // For each neighbor
            for(int j = -1; j < 2; j++){
                if(!(j == 0 && i == 0)){
                    if(currentCell->posX + i >= 0 && currentCell->posX + i < cols && currentCell->posY + j >= 0 && currentCell->posY + j < rows){
                        Cell * neighborCell = lstCells[currentCell->posX + i][currentCell->posY + j];
                        if(!closedList.contains(neighborCell) && !neighborCell->isWall){
                            double tempG = currentCell->cost + sqrt(pow(endCell->posX - neighborCell->posX, 2) + pow(endCell->posY - neighborCell->posY, 2));
                            bool newPath = false;
                            if(openList.contains(neighborCell)){
                                if(tempG < neighborCell->heuristic){
                                    neighborCell->cost = tempG;
                                    newPath = true;
                                }
                            }else{
                                neighborCell->cost = tempG;
                                newPath = true;
                                openList.push_back(neighborCell);
                            }

                            // Yes, it's a better path
                            if (newPath) {
                              neighborCell->heuristic = sqrt(pow(endCell->posX - neighborCell->posX, 2) + pow(endCell->posY - neighborCell->posY, 2));
                              neighborCell->previousCell = currentCell;
                            }
                            /*if(openList.contains(neighborCell) && neighborCell->cost < currentCell->cost){
                                neighborCell->cost = currentCell->cost + 1;
                                neighborCell->heuristic = neighborCell->cost + sqrt(pow(endCell->posX - neighborCell->posX, 2) + pow(endCell->posY - neighborCell->posY, 2));
                                openList.push_back(neighborCell);
                                neighborCell->previousCell = currentCell;
                            }*/
                        }
                    }
                }
            }
        }

        for(int x = 0; x < cols; x++){
            for(int y = 0; y < rows; y++){
                if(closedList.contains(lstCells[x][y])){
                    lstCells[x][y]->isClosed = true;
                    lstCells[x][y]->update();
                }else if(openList.contains(lstCells[x][y])){
                    lstCells[x][y]->isOpen = true;
                    lstCells[x][y]->update();
                }
            }
        }

        // Reconstituer le chemin
        Cell * temp = currentCell;
        while(temp->previousCell != nullptr){
            temp->isOnPath = true;
            temp->isClosed = false;
            temp->isOpen = false;
            temp->update();
            temp = temp->previousCell;
        }

    }else{
        qDebug() << "No solution !";
    }
}

void Widget::update()
{
    // Update cells
    for(int x = 0; x < rows; x++){
        for(int y = 0; y < cols; y++){
            lstCells[x][y]->update();
        }
    }
}

