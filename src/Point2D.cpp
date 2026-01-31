#include "../headers/Point2D.hpp"

Point2D::Point2D(){
    x = 0;
    y = 0;
}

Point2D::Point2D(float x, float y){
    this->x = x;
    this->y = y;
}


void Point2D::setX(float x){
    this->x = x;
}

void Point2D::setY(float y){
    this->y = y;
}

void Point2D::setPoint(float x, float y){
    this->x = x;
    this->y = y;
}


float Point2D::getX() const{
    return x;
}

float Point2D::getY() const{
    return y;
}


Point2D Point2D::operator+(const Point2D& point2) const{
    Point2D newPoint;
    newPoint.setX(getX() + point2.getX());
    newPoint.setY(getY() + point2.getY());
    return newPoint;
}

Point2D Point2D::operator-(const Point2D& point2) const{
    Point2D newPoint;
    newPoint.setX(getX() - point2.getX());
    newPoint.setY(getY() - point2.getY());
    return newPoint;
}

Point2D Point2D::operator*(float constNumber) const{
    Point2D newPoint;
    newPoint.setX(getX() * constNumber);
    newPoint.setY(getY() * constNumber);
    return newPoint;
}