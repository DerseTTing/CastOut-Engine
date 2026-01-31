#pragma once
#include "Object2D.hpp"
#include "Point2D.hpp"
#include "settings.hpp"

class Circle : public Object2D
{
    protected: 
        float radius;
    
        void updateAngle() override;

    public:
        Circle(const Point2D& positionCenter, float radius, float height, sf::Color color, ObjectType objectType);

        bool isInside(Point2D point) override;
        float intersectRay(Point2D startPointRay, float angle, Point2D directionVector) override;
        void drawOnMinimap(sf::RenderWindow& window) override;

        void setPositionCenter(const Point2D& positionCenter) override;
        void setRadius(float radius);

        float getRadius();
};