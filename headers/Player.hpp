#pragma once
#include "Circle.hpp"
#include "Rectangle.hpp"

class Player : public Circle
{
    private:
        int health = PLAYER_HEALTH;
        float directionGaze = PLAYER_VIEWING_ANGLE;
        float speed = PLAYER_SPEED;
        float stamina = PLAYER_MAX_STAMINA;
        float staminaMax = PLAYER_MAX_STAMINA;
        float mouseDelta;
        bool sprintIsAvailable = true;

        int countCollectedKey = 0;

        bool isRotationToEnemy = false;
        bool isScreamToEnemy = false;
        float currentTimeScreamToEnemy = 0.0f;
        float angleStepRotationToEnemy;
        float targetDirectionGazeToEnemy;

        void updateMovement(const vector<Object2D*>& arrayObjects, float deltaTime);
        void updateRotation(float deltaAngleRotation, float deltaTime);
        void updateStamina(float deltaTime);

    public:
        Player(const Point2D& positionCenter, float radius, float objectHeight, sf::Color color, ObjectType objectType);

        void updateObject(const vector<Object2D*>& arrayObjects, float deltaTime) override;
        Point2D createMoveVector();

        void startRotationToEnemy(const Point2D& positionEnemy);

        void drawOnMinimap(sf::Color color, sf::RenderWindow& window) const override;

        void setDirectionGaze(float directionGaze);
        void setSpeed(float speed);
        void setStamina(float stamina);
        void setMouseDelta(float mouseDelta);
        void setCountCollectedKey(int countCollectedKey);
        
        int getHealth() const;
        float getDirectionGaze() const;
        float getSpeed() const;
        float getStamina() const;
        bool getSprintIsAvailable() const;
        int getCountCollectedKey() const;
        bool getIsCatchToEnemy() const;
};