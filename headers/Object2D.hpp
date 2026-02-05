#pragma once
#include <SFML/Graphics.hpp>
#include "Point2D.hpp"
#include "ObjectType.hpp"
#include "RayHit.hpp"
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
        
        virtual Point2D distanceToObject(Point2D point, float playerRadius) = 0;
        virtual float intersectRay(Point2D startPointRay, Point2D directionVector) = 0;
        virtual void updateObject(const vector<Object2D*>& arrayObjects, float deltaTime) {};
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