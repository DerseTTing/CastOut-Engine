#include "../headers/Room.hpp"

void Room::fullRoom(Cell (&map)[MAP_SIZE_Y][MAP_SIZE_X]){
    int sizeRoom = min(width, length);
    if(sizeRoom > ROOM_SIZE_EMPTY && sizeRoom <= ROOM_SIZE_LITTLE)
        fullLittleRoom(map);
    else if(sizeRoom <= ROOM_SIZE_MIDLE)
        fullMidleRoom(map);
    else
        fullHightRoom(map);
}

void Room::fullLittleRoom(Cell (&map)[MAP_SIZE_Y][MAP_SIZE_X]){
    float droppedChance = Random::getRandomNumber(0.0f, 1.0f);
    if(droppedChance < 1.0f){
        float minSizeCentralCircleOfPartRoom = 0.30f;
        float maxSizeCentralCircleOfPartRoom = 0.50f;
        room_CentralCircle(minSizeCentralCircleOfPartRoom, maxSizeCentralCircleOfPartRoom, map);
    }
}

void Room::fullMidleRoom(Cell (&map)[MAP_SIZE_Y][MAP_SIZE_X]){
    float droppedChance = Random::getRandomNumber(0.0f, 1.0f);
}

void Room::fullHightRoom(Cell (&map)[MAP_SIZE_Y][MAP_SIZE_X]){
    float droppedChance = Random::getRandomNumber(0.0f, 1.0f);
}

void Room::room_CentralCircle(float minSizeCentralCircleOfPartRoom, float maxSizeCentralCircleOfPartRoom, Cell (&map)[MAP_SIZE_Y][MAP_SIZE_X]){
    int minPassage = 2;
    Point2D partPositionCircle = Point2D(0.5f, 0.5f);
    int sizeRoom = min(width, length);
    maxSizeCentralCircleOfPartRoom = min(static_cast<float>(sizeRoom - PLAYER_RADIUS/MAP_CELL_SIZE - minPassage) / sizeRoom, maxSizeCentralCircleOfPartRoom);
    minSizeCentralCircleOfPartRoom = min(maxSizeCentralCircleOfPartRoom, minSizeCentralCircleOfPartRoom);
    
    Point2D localePositionCircle = conversionPartToLocalPosition(partPositionCircle);
    int sizeCircle = floor(Random::getRandomNumber(minSizeCentralCircleOfPartRoom, maxSizeCentralCircleOfPartRoom + 0.00001f) * sizeRoom);

    generationCircleInRoom(localePositionCircle, sizeCircle, map);
}

void Room::room_CentralRectangle(int sizeRectangle, Cell (&map)[MAP_SIZE_Y][MAP_SIZE_X]){

}

Point2D Room::conversionPartToLocalPosition(Point2D partPosition){
    Point2D localPosition = Point2D(width * partPosition.getX(), length * partPosition.getY());
    return localPosition;
}


void Room::generationCircleInRoom(Point2D localePositionCenterCircle, int size, Cell (&map)[MAP_SIZE_Y][MAP_SIZE_X]){
    int positionAngleCircleX = positionAngle.getX() + round(localePositionCenterCircle.getX() - size/2.0f);
    int positionAngleCircleY = positionAngle.getY() + round(localePositionCenterCircle.getY() - size/2.0f);

    float globalAngleCircleX = positionAngle.getX() + (localePositionCenterCircle.getX() - size/2.0f);
    float globalAngleCircleY = positionAngle.getY() + (localePositionCenterCircle.getY() - size/2.0f);

    for(int x = positionAngleCircleX; x < positionAngleCircleX + size; x++){
        for(int y = positionAngleCircleY; y < positionAngleCircleY + size; y++){
            Point2D distanceDotToCenter = Point2D(globalAngleCircleX + size/2.0f - x, globalAngleCircleY + size/2.0f - y);
            if(distanceDotToCenter.getLenghtVector() >= size/2 + ENEMY_RADIUS){
                if(x != positionAngleCircleX || y != positionAngleCircleY){
                    map[y][x].objectType = ObjectType::Circle;
                    map[y][x].createObject = false;
                }
                else{
                    map[y][x].objectType = ObjectType::CircleGenerator;
                    map[y][x].createObject = true;
                }
            }
            else{
                if(x != positionAngleCircleX || y != positionAngleCircleY){
                    map[y][x].objectType = ObjectType::CircleWall;
                    map[y][x].createObject = false;
                }
                else{
                    map[y][x].objectType = ObjectType::CircleWallGenerator;
                    map[y][x].createObject = true;
                }
            }
        }
    }
}

void Room::generationRectangleInRoom(Point2D localePositionAngleRectangle, int size, Cell (&map)[MAP_SIZE_Y][MAP_SIZE_X]){
    int positionAngleRectangleX = positionAngle.getX() + round(localePositionAngleRectangle.getX());
    int positionAngleRectangleY = positionAngle.getY() + round(localePositionAngleRectangle.getY());
    for(int x = positionAngleRectangleX; x < positionAngleRectangleX + size; x++){
        for(int y = positionAngleRectangleY; y < positionAngleRectangleY + size; y++){
            map[y][x].createObject = true;
            map[y][x].objectType = ObjectType::RectangleWall;

        }
    }
}

void Room::setPositionAngle(const Point2D& positionAngle){
    this->positionAngle = positionAngle;
}

void Room::setWidth(int width){
    this->width = width;
}

void Room::setLenght(int length){
    this->length = length;
}


Point2D& Room::getPositionAngle(){
    return positionAngle;
}

int Room::getWidth(){
    return width;
}

int Room::getLenght(){
    return length;
}