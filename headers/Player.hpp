#pragma once
#include "Circle.hpp"

class Player : public Circle
{
    private:
        float directionGaze;
        float speed;
        void updateMovement(float deltaTime);
        void updateRotation(float deltaTime);
    public:
        Player(const Point2D& positionCenter, float radius, float objectHeight, sf::Color color, ObjectType objectType);

        void updateObject(float deltaTime) override;

        //void drawOnMinimap(sf::RenderWindow& window) override;

        void setDirectionGaze(float directionGaze);
        void setSpeed(float speed);

        float getDirectionGaze();
        float getSpeed();
};