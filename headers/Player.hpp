#pragma once
#include "Circle.hpp"
#include "Rectangle.hpp"

class Player : public Circle
{
    private:
        float directionGaze;
        float speed;
        void updateMovement(const vector<Object2D*>& arrayObjects, float deltaTime);
        void updateRotation(float deltaTime);
    public:
        Player(const Point2D& positionCenter, float radius, float objectHeight, sf::Color color, ObjectType objectType);

        void updateObject(const vector<Object2D*>& arrayObjects, float deltaTime) override;
        Point2D createMoveVector();

        void setDirectionGaze(float directionGaze);
        void setSpeed(float speed);

        float getDirectionGaze();
        float getSpeed();
};