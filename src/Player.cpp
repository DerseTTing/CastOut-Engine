#include "../headers/Player.hpp"

Player::Player(const Point2D& positionCenter, float radius, float objectHeight, sf::Color color, ObjectType objectType)
    :Circle(positionCenter, radius, objectHeight, color, objectType){
    speed = PLAYER_SPEED;
    directionGaze = PLAYER_VIEWING_ANGLE;
    stamina = PLAYER_MAX_STAMINA;
}

void Player::updateObject(const vector<Object2D*>& arrayObjects, float deltaTime){
    updateStamina(deltaTime);
    updateMovement(arrayObjects, deltaTime);
    updateRotation(mouseDelta, deltaTime);
}

void Player::updateMovement(const vector<Object2D*>& arrayObjects, float deltaTime){
    Point2D moveVector = createMoveVector();
    Point2D velocity = moveVector * (speed * deltaTime);
    Point2D targetPosition = positionCenter + velocity;

    if(!(moveVector.getX() == 0 && moveVector.getY() == 0)){
        for(int i = 0; i < arrayObjects.size(); i++){
            if(!ObjectTypeRules::isPlayerNotCoolisionTypeObject(arrayObjects[i]->getObjectType())){
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

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && sprintIsAvailable == true)
        speed = PLAYER_SPEED * PLAYER_SHIFT_SPEED_MULTIPLIER;
    else
        speed = PLAYER_SPEED;
    
    

    moveVector.normalizedVector();

    return moveVector;
}

void Player::updateStamina(float deltaTime){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && sprintIsAvailable == true)
        stamina -= PLAYER_DEGENERATION_STAMINA * deltaTime;
    else if(stamina < PLAYER_MAX_STAMINA)
        stamina += PLAYER_REGENERATION_STAMINA * deltaTime;
    
    if(stamina <= 0){
        stamina = 0;
        sprintIsAvailable = false;
    }

    if(stamina > PLAYER_MAX_STAMINA)
        stamina = PLAYER_MAX_STAMINA;

    if(stamina > PLAYER_PART_STAMINA_TO_SHIFT*PLAYER_MAX_STAMINA)
        sprintIsAvailable = true;
}



void Player::drawOnMinimap(sf::Color color, sf::RenderWindow& window) const{
    sf::CircleShape circle;
    
    float visualRadius = radius * MINIMAP_CHARACTER_SIZE_MULTIPLIER;
    circle.setRadius(visualRadius);

    float drawX = positionCenter.getX() - visualRadius;
    float drawY = positionCenter.getY() - visualRadius;
    circle.setPosition(drawX, drawY);

    circle.setFillColor(color);

    window.draw(circle);
}

void Player::updateRotation(float mouseDelta, float deltaTime){
     setDirectionGaze(getDirectionGaze() + mouseDelta);
}

void Player::setDirectionGaze(float directionGaze){
    this->directionGaze = directionGaze;
}

void Player::setSpeed(float speed){
    this->speed = speed;
}

void Player::setStamina(float stamina){
    this->stamina = stamina;
}

void Player::setMouseDelta(float mouseDelta){
    this->mouseDelta = mouseDelta;
}

void Player::setCountCollectedKey(int countCollectedKey){
    this->countCollectedKey = countCollectedKey;
}


float Player::getDirectionGaze() const{
    return directionGaze;
}

float Player::getSpeed() const{
    return speed;
}

float Player::getStamina() const{
    return stamina;
}

bool Player::getSprintIsAvailable() const{
    return sprintIsAvailable;
}

int Player::getCountCollectedKey() const{
    return countCollectedKey;
}