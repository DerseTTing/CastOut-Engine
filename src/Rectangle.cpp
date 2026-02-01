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

float Rectangle::intersectRay(Point2D startPointRay, float angle, Point2D directionVector){
    float minX = positionCenter.getX() - width/2;
    float maxX = positionCenter.getX() + width/2;
    float minY = positionCenter.getY() - length/2;
    float maxY = positionCenter.getY() + length/2;

    float directionVectorX = directionVector.getX();
    float directionVectorY = directionVector.getY();

    if(directionVectorX == 0) directionVectorX = LITLE_FLOAT;
    if(directionVectorY == 0) directionVectorY = LITLE_FLOAT;

    float timeX1 = (minX - startPointRay.getX()) / directionVectorX;
    float timeX2 = (maxX - startPointRay.getX()) / directionVectorX;
    float timeY1 = (minY - startPointRay.getY()) / directionVectorY;
    float timeY2 = (maxY - startPointRay.getY()) / directionVectorY;

    float timeEnterX = min(timeX1, timeX2);
    float timeExitX = max(timeX1, timeX2);
    float timeEnterY = min(timeY1, timeY2);
    float timeExitY = max(timeY1, timeY2);

    float timeEnter = max(timeEnterX, timeEnterY);
    float timeExit = min(timeExitX, timeExitY);

    if(timeEnter > timeExit || timeExit < 0) return -1.0f;
    if (timeEnter < 0) return 0.0f;
    return timeEnter;
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
