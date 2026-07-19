#pragma once
#include "Point2D.hpp"
#include "Rectangle.hpp"
#include "Player.hpp"

class RectangleExit : public Rectangle{
    private:
        bool isOpen = false;
        bool finishGame = false;
    public:
        RectangleExit(const Point2D& positionCenter, float width, float length, float objectHeight, sf::Color color, ObjectType objectType);

        void drawOnMinimap(sf::Color color, sf::RenderWindow& window) const override;

        void updateObject(const vector<Object2D*>& arrayObjects, float deltaTime) override;

        Point2D distanceToObject(Point2D point, float playerRadius) override;

        void setFinishGame(bool finishGame);
        bool getFinishGame();



};


