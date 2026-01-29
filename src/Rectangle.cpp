#include "../headers/Rectangle.hpp"

Rectangle::Rectangle(const Point2D& positionCenter, float width, float height, sf::Color color){
    setPositionCenter(positionCenter);
    setWidth(width);
    setHeight(height);
    setColor(color);
    
    updateAngle();
}


bool Rectangle::isInside(Point2D point){

    return false;
}


void Rectangle::updateAngle(){
    float x = getPositionCenter().getX() - (width / 2.0f);
    float y = getPositionCenter().getY() - (height / 2.0f);
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

void Rectangle::setHeight(float height){
    this->height = height;
    updateAngle();
}



float Rectangle::getWidth() const{
    return width;
}

float Rectangle::getHeight() const{
    return height;
}

const Point2D& Rectangle::getPositionAngle() const{
    return positionAngle;
}

void Rectangle::drawOnMinimap(sf::RenderWindow& window){
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(width * MINIMAP_SCALE, height * MINIMAP_SCALE));
    rectangle.setPosition(positionAngle.getX() * MINIMAP_SCALE, positionAngle.getY() * MINIMAP_SCALE);
    rectangle.setFillColor(color);
    window.draw(rectangle);
}