#include "../headers/Circle.hpp"

Circle::Circle(const Point2D& positionCenter, float radius, float objectHeight, sf::Color color, ObjectType objectType){
    setPositionCenter(positionCenter);
    setRadius(radius);
    setObjectHeight(objectHeight);
    setColor(color);
    setObjectType(objectType);
    
    updateAngle();
}

Point2D Circle::distanceToObject(Point2D point, float playerRadius){
    float vectorX = point.getX() - positionCenter.getX();
    float vectorY = point.getY() - positionCenter.getY();

    float lengthVector = sqrt(vectorX * vectorX + vectorY * vectorY);
    
    float totalRadius = radius + playerRadius;

    if (lengthVector < totalRadius) {
        Point2D direction = Point2D(vectorX, vectorY);
        
        if (lengthVector == 0)
            return Point2D(0, 1) * totalRadius;

        direction.normalizedVector();
        float overlap = totalRadius - lengthVector;
        return direction * overlap;
    }

    return Point2D(0, 0);
}

float Circle::intersectRay(Point2D startPointRay, Point2D directionVector){
    float posX_toCircleVector = positionCenter.getX() - startPointRay.getX();
    float posY_toCircleVector = positionCenter.getY() - startPointRay.getY();
    Point2D toCircleVector = {posX_toCircleVector, posY_toCircleVector};

    float distToCenterSquared = toCircleVector.getX() * toCircleVector.getX() + toCircleVector.getY() * toCircleVector.getY();
    bool startInsideCircle = distToCenterSquared < radius * radius;

    float tca = directionVector.getX() * toCircleVector.getX() + directionVector.getY() * toCircleVector.getY();
    if(tca < 0 && !startInsideCircle) return -1.0f; // круг целиком позади луча, и мы не внутри него

    float perpendicularSquared = distToCenterSquared - tca * tca;
    if(perpendicularSquared > radius * radius) return -1.0f;

    float thc = sqrt(radius * radius - perpendicularSquared);
    float t1 = tca - thc; // ближняя точка (вход)
    float t2 = tca + thc; // дальняя точка (выход)

    if(t1 >= 0) return t1;
    if(t2 >= 0) return t2; // мы стартуем внутри круга — возвращаем точку выхода
    return -1.0f;
}

void Circle::drawOnMinimap(sf::Color color, sf::RenderWindow& window) const{
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