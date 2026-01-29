#pragma once
#include "Object2D.hpp"
#include "Point2D.hpp"
#include "settings.hpp"

class Rectangle : public Object2D
{
    private:
        Point2D positionAngle;
        float width;
        float height;

        void updateAngle();

    public:
        Rectangle(const Point2D& positionCenter, float width, float height, sf::Color color);
        ~Rectangle() override = default;

        bool isInside(Point2D point) override;

        void setPositionCenter(const Point2D& positionCenter) override;
        void setWidth(float width);
        void setHeight(float height);

        float getWidth() const;
        float getHeight() const;
        const Point2D& getPositionAngle() const;

        void drawOnMinimap(sf::RenderWindow& window) override;

};