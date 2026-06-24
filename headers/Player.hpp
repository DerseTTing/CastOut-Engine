#pragma once
#include "Circle.hpp"
#include "Rectangle.hpp"

class Player : public Circle
{
    private:
        float directionGaze;
        float speed;
        float stamina;
        bool sprintIsAvailable = true;


        void updateMovement(const vector<Object2D*>& arrayObjects, float deltaTime);
        void updateRotation(float deltaAngleRotation, float deltaTime);
        void updateStamina(float deltaTime);
        bool shiftPressed();
    public:
        Player(const Point2D& positionCenter, float radius, float objectHeight, sf::Color color, ObjectType objectType);

        void updatePlayer(const vector<Object2D*>& arrayObjects, float deltaAngleRotation, float deltaTime);
        Point2D createMoveVector();

        void drawOnMinimap(sf::Color color, sf::RenderWindow& window) const override;

        void setDirectionGaze(float directionGaze);
        void setSpeed(float speed);
        void setStamina(float stamina);
        
        float getDirectionGaze();
        float getSpeed();
        float getStamina();
        bool getSprintIsAvailable();
};