#pragma once
#include "Circle.hpp"
#include "CellMap.hpp"
#include "Random.hpp"
#include "Player.hpp"
#include "PathFinding.hpp"
#include "settings.hpp"
#include <queue>
#include <deque>

class Enemy : public Circle
{
    private:
        float speed = ENEMY_SPEED_SEARCH;
        deque<Point2D> listStepToTarget;

        const Player* player;
        PathFinding* pathFinding;

        void goToNextStep(float deltaTime);

        void foundNewRandomTarget();

        bool enemySeePlayer(const vector<Object2D*>& arrayObjects) const;
        void foundChaseTarget(const vector<Object2D*>& arrayObjects);

        Point2D lastPlayerPosition;

        bool enemyInChase = false;
        bool enemyInCarefullyMode = false;

        void cheakDealsDamage();
        bool dealsDamage = false;

    public:
        Enemy(const Point2D& positionCenter, float radius, float objectHeight, sf::Color color, ObjectType objectType);
        ~Enemy();

        void updateObject(const vector<Object2D*>& arrayObjects, float deltaTime) override;

        void foundCarefullyModeTarget(const Point2D& positionTarget);

        void drawOnMinimap(sf::Color color, sf::RenderWindow& window) const override;

        void teleportToFarWalkableCell(const Point2D& positionPlayer);

        void setPlayer(const Player* player);
        void setPathFinding(PathFinding* pathFinding);
        void setDealsDamage(bool dealsDamage);

        bool getEnemyInChase() const;
        bool getEnemyInCarefullyMode() const;
        bool getDealsDamage() const;

};