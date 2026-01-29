#pragma once
#include <SFML/Graphics.hpp>
#include "Point2D.hpp"
#include "ObjectType.hpp"
class Object2D
{
    protected:
        Point2D positionCenter;
        sf::Color color;
        ObjectType objectType;

    public:
        virtual ~Object2D() = default;
        virtual bool isInside(Point2D point) = 0;

        virtual void setPositionCenter(const Point2D& positionCenter);
        void setColor(sf::Color color);

        const Point2D& getPositionCenter() const;
        const sf::Color& getColor() const;

        virtual void drawOnMinimap(sf::RenderWindow& window) = 0;

};