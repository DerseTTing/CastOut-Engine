#pragma once
#include "Circle.hpp"
#include "Player.hpp"
#include "Enemy.hpp"

class Key : public Circle{
    private:

    public:
        Key(const Point2D& positionCenter, float radius, float objectHeight, sf::Color color, ObjectType objectType);

        ~Key();

        void drawOnMinimap(sf::Color color, sf::RenderWindow& window) const override;

        void updateObject(const vector<Object2D*>& arrayObjects, float deltaTime) override;
};