#pragma once
#include "Circle.hpp"
#include "CellMap.hpp"
#include "Random.hpp"
#include "Player.hpp"
#include "settings.hpp"
#include <queue>
#include <deque>

class Enemy : public Circle
{
    private:
        float speed;
        deque<Point2D> listStepToTarget;
        deque<Point2D> listStepToPriorityTarget;

        const Cell (*map)[MAP_SIZE_X];
        const Player* player;
        vector<Point2D> walkableCell;

        bool isWalkableCell(int posX, int posY) const;
        void fillWalkableCell();
        Point2D getRandomWalkableCell();

        void goToNextStep(float deltaTime);
        void fillListStepToTarget(Point2D positionTarget);

        void foundNewRandomTarget();

        bool enemySeePlayer(const vector<Object2D*>& arrayObjects) const;
        void foundChaseTarget(const vector<Object2D*>& arrayObjects);
        bool seePlayerFirstTime = true;
        Point2D lastPlayerPosition;


    public:
        Enemy(const Point2D& positionCenter, float radius, float objectHeight, sf::Color color, ObjectType objectType);

        void updateObject(const vector<Object2D*>& arrayObjects, float deltaTime) override;

        void drawOnMinimap(sf::Color color, sf::RenderWindow& window) const override;

        void setMap(const Cell (*map)[MAP_SIZE_X]);

        void setPlayer(const Player* player);


};