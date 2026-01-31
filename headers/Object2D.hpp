#pragma once
#include <SFML/Graphics.hpp>
#include "Point2D.hpp"
#include "ObjectType.hpp"
#include <vector>
#include <cmath>
using namespace std;

class Object2D
{
    protected:
        Point2D positionCenter;
        Point2D positionAngle;
        sf::Color color;
        ObjectType objectType;
        float objectHeight;

        virtual void updateAngle() = 0;

    public:
        virtual ~Object2D() = default;
        
        virtual bool isInside(Point2D point) = 0;
        virtual float intersectRay(Point2D startPointRay, float angle, Point2D directionVector) = 0;
        virtual void updateObject(float deltaTime) {};
        virtual void drawOnMinimap(sf::RenderWindow& window) = 0;

        virtual void setPositionCenter(const Point2D& positionCenter);
        void setColor(sf::Color color);
        void setObjectType(ObjectType objectType);
        void setObjectHeight(float objectHeight);

        const Point2D& getPositionCenter() const;
        const sf::Color& getColor() const;
        ObjectType getObjectType();
        float getObjectHeight();


};