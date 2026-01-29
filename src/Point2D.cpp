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


const float Point2D::getX() const{
    return x;
}

const float Point2D::getY() const{
    return y;
}

