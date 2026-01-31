#pragma once

class Point2D
{
    private:
    float x;
    float y;

    public:
    Point2D();
    Point2D(float x, float y);

    void setX(float x);
    void setY(float y);
    void setPoint(float x, float y);

    float getX() const;
    float getY() const;

    Point2D operator+(const Point2D& point2) const;
    Point2D operator-(const Point2D& point2) const;
    Point2D operator*(float constNumber) const;
};