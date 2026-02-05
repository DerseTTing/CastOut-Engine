#include "../headers/Player.hpp"

Player::Player(const Point2D& positionCenter, float radius, float objectHeight, sf::Color color, ObjectType objectType)
    :Circle(positionCenter, radius, objectHeight, color, objectType){
    speed = PLAYER_SPEED;
    directionGaze = PLAYER_VIEWING_ANGLE;
}

void Player::updateObject(const vector<Object2D*>& arrayObjects, float deltaTime){
    updateMovement(arrayObjects, deltaTime);
    updateRotation(deltaTime);
}

void Player::updateMovement(const vector<Object2D*>& arrayObjects, float deltaTime){
    Point2D moveVector = createMoveVector();
    Point2D velocity = moveVector * (speed * deltaTime);
    Point2D targetPosition = positionCenter + velocity;

    if(!(moveVector.getX() == 0 && moveVector.getY() == 0)){
        for(int i = 0; i < arrayObjects.size(); i++){
            if(arrayObjects[i]->getObjectType() != ObjectType::Player){
                Point2D pushOut = arrayObjects[i]->distanceToObject(targetPosition, radius);
                if (pushOut.getX() != 0 || pushOut.getY() != 0) {
                    targetPosition = targetPosition + pushOut;
                }
                
            }
        }
    }
    setPositionCenter(targetPosition);
}

Point2D Player::createMoveVector(){
    Point2D moveVector = Point2D(0, 0);
    float directionGazeRadian = getDirectionGaze() * PI / 180.0f;
    Point2D directionVectorForward = Point2D(cos(directionGazeRadian), sin(directionGazeRadian));
    Point2D directionVectorRight = Point2D(-1 * directionVectorForward.getY(), directionVectorForward.getX());

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        moveVector = moveVector + directionVectorForward;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        moveVector = moveVector - directionVectorForward;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        moveVector = moveVector - directionVectorRight;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        moveVector = moveVector + directionVectorRight;

    moveVector.normalizedVector();

    return moveVector;
}

void Player::updateRotation(float deltaTime){
    float dRotation = 0.0f;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        dRotation -= 1.0f;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        dRotation += 1.0f;

    if(dRotation != 0){
        float rotation = getDirectionGaze() + (dRotation * PLAYER_SCREEN_ROTATION_SENSITIVITY * deltaTime);
        setDirectionGaze(rotation);
    }
}

void Player::setDirectionGaze(float directionGaze){
    this->directionGaze = directionGaze;
}

void Player::setSpeed(float speed){
    this->speed = speed;
}

float Player::getDirectionGaze(){
    return directionGaze;
}

float Player::getSpeed(){
    return speed;
}