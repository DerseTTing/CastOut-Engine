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


Point2D Rectangle::distanceToObject(Point2D point, float playerRadius){
  float sideXmin = positionCenter.getX() - width / 2;
    float sideXmax = positionCenter.getX() + width / 2;
    float sideYmin = positionCenter.getY() - length / 2;
    float sideYmax = positionCenter.getY() + length / 2;

    float closestDotX = max(sideXmin, min(point.getX(), sideXmax));
    float closestDotY = max(sideYmin, min(point.getY(), sideYmax));

    float dx = point.getX() - closestDotX;
    float dy = point.getY() - closestDotY;
    float distanceSquared = dx * dx + dy * dy;

    // СЛУЧАЙ 1: Центр игрока СНАРУЖИ прямоугольника (или на самой границе)
    if (distanceSquared > 0) {
        float distance = sqrt(distanceSquared);
        if (distance < playerRadius) {
            float overlap = playerRadius - distance;
            return Point2D(dx / distance, dy / distance) * overlap;
        }
    } 
    // СЛУЧАЙ 2: Центр игрока оказался ВНУТРИ прямоугольника
    else {
        // Ищем расстояния до всех 4-х граней
        float distToLeft   = point.getX() - sideXmin;
        float distToRight  = sideXmax - point.getX();
        float distToTop    = point.getY() - sideYmin;
        float distToBottom = sideYmax - point.getY();

        // Находим минимальное расстояние — это кратчайший путь наружу
        float minDist = min({distToLeft, distToRight, distToTop, distToBottom});

        if (minDist == distToLeft)   return Point2D(-(minDist + playerRadius), 0);
        if (minDist == distToRight)  return Point2D(minDist + playerRadius, 0);
        if (minDist == distToTop)    return Point2D(0, -(minDist + playerRadius));
        if (minDist == distToBottom) return Point2D(0, minDist + playerRadius);
    }

    return Point2D(0, 0);
}

float Rectangle::intersectRay(Point2D startPointRay, Point2D directionVector){
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
