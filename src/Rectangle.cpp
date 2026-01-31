#include "../headers/Rectangle.hpp"

Rectangle::Rectangle(const Point2D& positionCenter, float width, float length, float objectHeight, sf::Color color, ObjectType objectType){
    setPositionCenter(positionCenter);
    setWidth(width);
    setLength(length);
    setObjectHeight(objectHeight);
    setColor(color);
    setObjectType(objectType);
    
    updateAngle();
}


bool Rectangle::isInside(Point2D point){

    return false;
}

void Rectangle::drawOnMinimap(sf::RenderWindow& window){
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(width, length));
    rectangle.setPosition(positionAngle.getX(), positionAngle.getY());
    rectangle.setFillColor(color);
    window.draw(rectangle);
}


void Rectangle::updateAngle(){
    float x = getPositionCenter().getX() - (width / 2.0f);
    float y = getPositionCenter().getY() - (length / 2.0f);
    positionAngle = Point2D(x, y);
}


void Rectangle::setPositionCenter(const Point2D& positionCenter){
    Object2D::setPositionCenter(positionCenter);
    updateAngle();
}

void Rectangle::setWidth(float width){
    this->width = width;
    updateAngle();
}

void Rectangle::setLength(float length){
    this->length = length;
    updateAngle();
}



float Rectangle::getWidth() const{
    return width;
}

float Rectangle::getLength() const{
    return length;
}

const Point2D& Rectangle::getPositionAngle() const{
    return positionAngle;
}
