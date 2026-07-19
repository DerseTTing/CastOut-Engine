#include "PathFinding.hpp"

PathFinding::PathFinding( Cell map[MAP_SIZE_Y][MAP_SIZE_X]){
    for(int y = 0; y < MAP_SIZE_Y; y++){
        for(int x = 0; x < MAP_SIZE_X; x++){
            this->map[y][x] = map[y][x];
        }
    }
    fillWalkableCell();
}

void PathFinding::fillWalkableCell(){
    walkableCell.clear();

    for(int y = 0; y < MAP_SIZE_Y; y++){
        for(int x = 0; x < MAP_SIZE_X; x++){
            if(ObjectTypeRules::isWalkable(map[y][x].objectType))
                walkableCell.push_back(Point2D(x, y));
        }
    }
}

Point2D PathFinding::getRandomWalkableCell(){
    int idCell = Random::getRandomNumber(0, walkableCell.size() - 1);
    return walkableCell[idCell];
}

Point2D PathFinding::getFarWalkableCell(Point2D positionStart){
    fillListStepToTarget(positionStart, nullptr);

    Point2D farCell = positionStart;
    int distanceFarCell = 0;
    for(int y = 0; y < MAP_SIZE_Y; y++){
        for(int x = 0; x < MAP_SIZE_X; x++){
            if(pathMap[y][x].distance > distanceFarCell){
                distanceFarCell = pathMap[y][x].distance;
                farCell = Point2D(x, y);

            }
        }
    }
    return farCell;
}

deque<Point2D> PathFinding::fillListStepToTarget(Point2D positionStartInCell, const Point2D* positionTargetInCell){
    resetPathMap();

    deque<Point2D> listStepToTarget;
    
    queue<Point2D> queue;

    queue.push(positionStartInCell);
    pathMap[(int)queue.front().getY()][(int)queue.front().getX()].lastPosition = Point2D(-1, -1);
    pathMap[(int)queue.front().getY()][(int)queue.front().getX()].visited = true;

    Point2D currentAnalysis;
    do
    {
        currentAnalysis = queue.front();
        
        int posX, posY;
        posX = (int)currentAnalysis.getX();
        posY = (int)currentAnalysis.getY() + 1;
        if(posY < MAP_SIZE_Y && !pathMap[posY][posX].visited && ObjectTypeRules::isWalkable(map[posY][posX].objectType)){
            queue.push(Point2D(posX, posY));
            pathMap[posY][posX].lastPosition = currentAnalysis;
            pathMap[posY][posX].visited = true;
            pathMap[posY][posX].distance = pathMap[(int)currentAnalysis.getY()][(int)currentAnalysis.getX()].distance + 1;
        }

        posX = (int)currentAnalysis.getX();
        posY = (int)currentAnalysis.getY() - 1;
        if(posY >= 0 && !pathMap[posY][posX].visited && ObjectTypeRules::isWalkable(map[posY][posX].objectType)){
            queue.push(Point2D(posX, posY));
            pathMap[posY][posX].lastPosition = currentAnalysis;
            pathMap[posY][posX].visited = true;
            pathMap[posY][posX].distance = pathMap[(int)currentAnalysis.getY()][(int)currentAnalysis.getX()].distance + 1;
        }

        posX = (int)currentAnalysis.getX() + 1;
        posY = (int)currentAnalysis.getY();
        if(posX < MAP_SIZE_X && !pathMap[posY][posX].visited && ObjectTypeRules::isWalkable(map[posY][posX].objectType)){
            queue.push(Point2D(posX, posY));
            pathMap[posY][posX].lastPosition = currentAnalysis;
            pathMap[posY][posX].visited = true;
            pathMap[posY][posX].distance = pathMap[(int)currentAnalysis.getY()][(int)currentAnalysis.getX()].distance + 1;
        }

        posX = (int)currentAnalysis.getX() - 1;
        posY = (int)currentAnalysis.getY();
        if(posX >= 0 && !pathMap[posY][posX].visited && ObjectTypeRules::isWalkable(map[posY][posX].objectType)){
            queue.push(Point2D(posX, posY));
            pathMap[posY][posX].lastPosition = currentAnalysis;
            pathMap[posY][posX].visited = true;
            pathMap[posY][posX].distance = pathMap[(int)currentAnalysis.getY()][(int)currentAnalysis.getX()].distance + 1;
        }

        queue.pop();

    }while((positionTargetInCell == nullptr || currentAnalysis != *positionTargetInCell) && !queue.empty());

    if(positionTargetInCell == nullptr || currentAnalysis != *positionTargetInCell) return listStepToTarget;

    if(pathMap[(int)currentAnalysis.getY()][(int)currentAnalysis.getX()].lastPosition == Point2D(-1, -1)) return listStepToTarget;

    do{
        listStepToTarget.push_front(currentAnalysis);
        currentAnalysis = pathMap[(int)currentAnalysis.getY()][(int)currentAnalysis.getX()].lastPosition;
    }while(currentAnalysis != positionStartInCell);

    return listStepToTarget;
}

void PathFinding::resetPathMap(){

    for(int y = 0; y < MAP_SIZE_Y; y++){
        for(int x = 0; x < MAP_SIZE_X; x++){
            pathMap[y][x].visited = false;
            pathMap[y][x].distance = 0;
        }
    }
}

void PathFinding::setMap(Cell map[MAP_SIZE_Y][MAP_SIZE_X]){
    
    for(int y = 0; y < MAP_SIZE_Y; y++){
        for(int x = 0; x < MAP_SIZE_X; x++){
            this->map[y][x] = map[y][x];
        }
    }
    fillWalkableCell();
}