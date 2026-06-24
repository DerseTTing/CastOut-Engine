#pragma once
#include "Point2D.hpp"
#include "settings.hpp"
#include "CellMap.hpp"
#include "Random.hpp"


using namespace std;

class Room{
    private:
        Point2D positionAngle;
        int width;
        int length;

    public:
        void fullRoom(Cell (&map)[MAP_SIZE_Y][MAP_SIZE_X]);

        void fullLittleRoom(Cell (&map)[MAP_SIZE_Y][MAP_SIZE_X]);
        void fullMidleRoom(Cell (&map)[MAP_SIZE_Y][MAP_SIZE_X]);
        void fullHightRoom(Cell (&map)[MAP_SIZE_Y][MAP_SIZE_X]);
        
        void room_CentralCircle(float minSizeCentralCircleOfPartRoom, float maxSizeCentralCircleOfPartRoom, Cell (&map)[MAP_SIZE_Y][MAP_SIZE_X]);
        void room_CentralRectangle(int sizeRectangle, Cell (&map)[MAP_SIZE_Y][MAP_SIZE_X]);

        Point2D conversionPartToLocalPosition(Point2D partPosition);

        void generationCircleInRoom(Point2D localePositionCenterCircle, int size, Cell (&map)[MAP_SIZE_Y][MAP_SIZE_X]);
        void generationRectangleInRoom(Point2D localePositionAngleRectangle, int size, Cell (&map)[MAP_SIZE_Y][MAP_SIZE_X]);

        void setPositionAngle(const Point2D& positionAngle);
        void setWidth(int width);
        void setLenght(int length);

        Point2D& getPositionAngle();
        int getWidth();
        int getLenght();
};