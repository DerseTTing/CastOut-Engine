#include "../headers/Enemy.hpp"

Enemy::Enemy(const Point2D& positionCenter, float radius, float objectHeight, sf::Color color, ObjectType objectType)
: Circle(positionCenter, radius, objectHeight, color, objectType) {
    speed = ENEMY_SPEED_SEARCH;
}


Enemy::~Enemy(){
    delete(pathFinding);
}

void Enemy::updateObject(const vector<Object2D*>& arrayObjects, float deltaTime){
    if(enemySeePlayer(arrayObjects))
        foundChaseTarget(arrayObjects);
    else if(listStepToTarget.size() == 0)
        foundNewRandomTarget();


    if(listStepToTarget.size() != 0)
        goToNextStep(deltaTime);       
}

void Enemy::drawOnMinimap(sf::Color color, sf::RenderWindow& window) const{
    sf::CircleShape circle;
    
    float visualRadius = radius * MINIMAP_CHARACTER_SIZE_MULTIPLIER;
    circle.setRadius(visualRadius);

    float drawX = positionCenter.getX() - visualRadius;
    float drawY = positionCenter.getY() - visualRadius;
    circle.setPosition(drawX, drawY);

    circle.setFillColor(this->color);

    window.draw(circle);
}

void Enemy::goToNextStep(float deltaTime){
    Point2D target = Point2D((listStepToTarget.front().getX() + 0.5f) * MAP_CELL_SIZE, (listStepToTarget.front().getY() + 0.5f) * MAP_CELL_SIZE);
    Point2D vectorToTarget = target - positionCenter;
    float distanceToTarget = vectorToTarget.getLenghtVector();
    float step = speed * deltaTime;

    if(step >= distanceToTarget){
        setPositionCenter(target);
        listStepToTarget.pop_front();
    }
    else{
        vectorToTarget.normalizedVector();
        setPositionCenter(positionCenter + vectorToTarget * step);
    }

}

void Enemy::foundNewRandomTarget(){
    speed = ENEMY_SPEED_SEARCH;
    lastPlayerPosition = Point2D(-1, -1);
    enemyInChase = false;
    enemyInCarefullyMode = false;

    Point2D positionCenterInCell = Point2D(floor(positionCenter.getX() / MAP_CELL_SIZE), floor(positionCenter.getY() / MAP_CELL_SIZE));

    Point2D positionTargetInCell = pathFinding->getRandomWalkableCell();
    listStepToTarget = pathFinding->fillListStepToTarget(positionCenterInCell, &positionTargetInCell);
}

bool Enemy::enemySeePlayer(const vector<Object2D*>& arrayObjects) const{
    Point2D startPoint = positionCenter;
    Point2D directionVector = player->getPositionCenter() - positionCenter;
    directionVector.normalizedVector();

    float minDistance = MAX_FLOAT;
    Object2D* closestObject = nullptr;

    for(int i = 0; i < arrayObjects.size();i++){
        if(arrayObjects[i]->getObjectType() != ObjectType::Enemy){
            float dist = arrayObjects[i]->intersectRay(startPoint, directionVector);
            if(minDistance > dist && dist != -1.0f){
                minDistance = dist;
                closestObject = arrayObjects[i];
            }
        }
    }

    if(closestObject != nullptr && closestObject->getObjectType() == ObjectType::Player)
        return true;
    else
        return false;

}

void Enemy::foundChaseTarget(const vector<Object2D*>& arrayObjects){
    speed = ENEMY_SPEED_CHASE;
    enemyInChase = true;
    enemyInCarefullyMode = false;

    Point2D newLastPlayerPosition = Point2D(floor(player->getPositionCenter().getX() / MAP_CELL_SIZE), floor(player->getPositionCenter().getY() / MAP_CELL_SIZE));
    Point2D positionCenterInCell = Point2D(floor(positionCenter.getX() / MAP_CELL_SIZE), floor(positionCenter.getY() / MAP_CELL_SIZE));
    if(lastPlayerPosition != newLastPlayerPosition){
        listStepToTarget = pathFinding->fillListStepToTarget(positionCenterInCell, &newLastPlayerPosition);
        lastPlayerPosition = newLastPlayerPosition;
    }

}

void Enemy::foundCarefullyModeTarget(const Point2D& positionTarget){
    Point2D positionCenterInCell = Point2D(floor(positionCenter.getX() / MAP_CELL_SIZE), floor(positionCenter.getY() / MAP_CELL_SIZE));
    Point2D positionTargetInCell = Point2D(floor(positionTarget.getX() / MAP_CELL_SIZE), floor(positionTarget.getY() / MAP_CELL_SIZE));

    listStepToTarget = pathFinding->fillListStepToTarget(positionCenterInCell, &positionTargetInCell);

    enemyInCarefullyMode = true;
}

void Enemy::setPlayer(const Player* player){
    this->player = player;
}

void Enemy::setPathFinding(PathFinding* pathFinding){
    this->pathFinding = pathFinding;
}

bool Enemy::getEnemyInChase(){
    return enemyInChase;
}

bool Enemy::getEnemyInCarefullyMode(){
    return enemyInCarefullyMode;
}