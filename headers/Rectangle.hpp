#pragma once
#include "Object2D.hpp"
#include "Point2D.hpp"
#include "settings.hpp"

class Rectangle : public Object2D
{
    private:
        float width;
        float length;

        void updateAngle() override final;

    public:
        Rectangle(const Point2D& positionCenter, float width, float length, float objectHeight, sf::Color color, ObjectType objectType);
        ~Rectangle() override = default;

        bool isInside(Point2D point) override;
        void drawOnMinimap(sf::RenderWindow& window) override;

        void setPositionCenter(const Point2D& positionCenter) override;
        void setWidth(float width);
        void setLength(float length);

        float getWidth() const;
        float getLength() const;
        const Point2D& getPositionAngle() const;

};