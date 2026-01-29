#include "../headers/Object2D.hpp"

void Object2D::setPositionCenter(const Point2D& positionCenter){
    this->positionCenter = positionCenter;
}

void Object2D::setColor(sf::Color color){
    this->color = color;
}

const Point2D& Object2D::getPositionCenter() const{
    return positionCenter;
}

const sf::Color& Object2D::getColor() const{
    return color;
}