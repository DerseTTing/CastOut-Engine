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

    const float getX() const;
    const float getY() const;

};