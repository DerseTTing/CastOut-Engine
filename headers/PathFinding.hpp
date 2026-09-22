#pragma once
#include "Point2D.hpp"
#include "CellMap.hpp"
#include "Random.hpp"
#include "settings.hpp"
#include <vector>
#include <queue>
#include <deque>

using namespace std;

class PathFinding{
    private:
        Cell map[MAP_SIZE_Y][MAP_SIZE_X];
        vector<Point2D> walkableCell;
        

        struct CellPath
        {
            bool visited = false;
            Point2D lastPosition;
            int distance = 0;
        };

        CellPath pathMap[MAP_SIZE_Y][MAP_SIZE_X];

        void fillWalkableCell();

        void resetPathMap();

    public:
        PathFinding(Cell map[MAP_SIZE_Y][MAP_SIZE_X]);

        deque<Point2D> fillListStepToTarget(Point2D positionStartInCell, const Point2D* positionTargetInCell);

        Point2D getRandomWalkableCell();
        Point2D getFarWalkableCell(Point2D positionStartInCell);

        void setMap(Cell map[MAP_SIZE_Y][MAP_SIZE_X]);
};