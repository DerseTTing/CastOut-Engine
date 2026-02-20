#pragma once
#include <vector>
#include "ObjectType.hpp"
#include "settings.hpp"
#include "Rectangle.hpp"
#include "Player.hpp"
#include "Random.hpp"


class Map
{
    private:
        struct Cell {
            ObjectType objectType = ObjectType::None;
            bool indestructible = false;
            bool createObject = false;
        };

        struct Room {
            Point2D positionAngle;
            int width;
            int length;
        };
        
        Cell map[MAP_SIZE_Y][MAP_SIZE_X];
        vector<Room> rooms;


        void generationWallsAround();
        void generationRooms();
        void cutRoom(Point2D startPosition, bool horizontally, int lengthCut);
        void fixingWallNearDoor(const Point2D& positionDoor, bool horizontally);
        vector<int> getDoorPlaces(int lengthWall);
        void generationPlayer();

        vector<Object2D*> createArrayObjects();

    public:
        vector<Object2D*> generationMap();
        void clearMap();

};