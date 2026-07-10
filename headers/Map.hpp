#pragma once
#include <vector>
#include "ObjectType.hpp"
#include "settings.hpp"
#include "Rectangle.hpp"
#include "Player.hpp"
#include "Random.hpp"
#include "Room.hpp"
#include "CellMap.hpp"
#include "Enemy.hpp"
#include "PathFinding.hpp"


class Map
{
    private:
        
        Cell map[MAP_SIZE_Y][MAP_SIZE_X];
        vector<Room> rooms;

        Player* player;
        Enemy* enemy;


        void generationWallsAround();
        void generationRooms();
        void cutRoom(Point2D startPosition, bool horizontally, int lengthCut);
        void fixingWallNearDoor(const Point2D& positionDoor);
        vector<int> getDoorPlaces(int lengthWall);
        void fullRooms();
        void generationPlayer();
        void generationEnemy();

        vector<Object2D*> createArrayObjects();

    public:
        vector<Object2D*> generationMap();
        
        const Cell (*getMap())[MAP_SIZE_X];

        void clearMap();

};
