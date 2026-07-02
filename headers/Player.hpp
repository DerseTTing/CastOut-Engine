#pragma once
#include "Circle.hpp"
#include "Rectangle.hpp"

class Player : public Circle
{
    private:
        float directionGaze;
        float speed;
        float stamina;
        float mouseDelta;
        bool sprintIsAvailable = true;


        void updateMovement(const vector<Object2D*>& arrayObjects, float deltaTime);
        void updateRotation(float deltaAngleRotation, float deltaTime);
        void updateStamina(float deltaTime);
    public:
        Player(const Point2D& positionCenter, float radius, float objectHeight, sf::Color color, ObjectType objectType);

        void updateObject(const vector<Object2D*>& arrayObjects, float deltaTime) override;
        Point2D createMoveVector();

        void drawOnMinimap(sf::Color color, sf::RenderWindow& window) const override;

        void setDirectionGaze(float directionGaze);
        void setSpeed(float speed);
        void setStamina(float stamina);
        void setMouseDelta(float mouseDelta);
        
        float getDirectionGaze() const;
        float getSpeed() const;
        float getStamina() const;
        bool getSprintIsAvailable() const;
};