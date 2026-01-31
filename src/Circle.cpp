#include "../headers/Circle.hpp"

Circle::Circle(const Point2D& positionCenter, float radius, float objectHeight, sf::Color color, ObjectType objectType){
    setPositionCenter(positionCenter);
    setRadius(radius);
    setObjectHeight(objectHeight);
    setColor(color);
    setObjectType(objectType);
    
    updateAngle();
}

bool Circle::isInside(Point2D point){

}

float Circle::intersectRay(Point2D startPointRay, float angle, Point2D directionVector){
    float posX_directionVector = directionVector.getX();
    float posY_directionVector = directionVector.getY();

    float posX_toCircleVector = positionCenter.getX() - startPointRay.getX();
    float posY_toCircleVector = positionCenter.getY() - startPointRay.getY();
    Point2D toCircleVector = {posX_toCircleVector, posY_toCircleVector};

    float tca = directionVector.getX() * toCircleVector.getX() + directionVector.getY() * toCircleVector.getY();
    
    if(tca < 0) return -1.0f;

    float perpendicularSquared = (toCircleVector.getX() * toCircleVector.getX() + toCircleVector.getY() * toCircleVector.getY()) - (tca*tca); 

    if (perpendicularSquared > radius * radius) return -1.0f;

    float distanceReturn = sqrt(radius*radius - perpendicularSquared);
    float distToCircle = tca - distanceReturn;

    return distToCircle;
}

void Circle::drawOnMinimap(sf::RenderWindow& window){
    sf::CircleShape circle;
    
    circle.setRadius(radius);
    circle.setPosition(positionAngle.getX(), positionAngle.getY());
    circle.setFillColor(color);

    window.draw(circle);
}

void Circle::updateAngle(){
    float x = getPositionCenter().getX() - radius;
    float y = getPositionCenter().getY() - radius;
    positionAngle = Point2D(x, y);
}

void Circle::setPositionCenter(const Point2D& positionCenter){
    Object2D::setPositionCenter(positionCenter);

    updateAngle();
}

void Circle::setRadius(float radius){
    this->radius = radius;

    updateAngle();
}

float Circle::getRadius(){
    return radius;
}