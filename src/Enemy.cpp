#include "../headers/Enemy.hpp"

Enemy::Enemy(const Point2D& positionCenter, float radius, float objectHeight, sf::Color color, ObjectType objectType)
: Circle(positionCenter, radius, objectHeight, color, objectType) {
    speed = ENEMY_SPEED_SEARCH;
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

bool Enemy::isWalkableCell(int posX, int posY) const{
    ObjectType objT = map[posY][posX].objectType;
    return objT != ObjectType::CircleWall && objT != ObjectType::CircleWallGenerator && objT != ObjectType::RectangleWall;
}

void Enemy::fillWalkableCell(){
    for(int y = 0; y < MAP_SIZE_Y; y++){
        for(int x = 0; x < MAP_SIZE_X; x++){
            if(isWalkableCell(x, y))
                walkableCell.push_back(Point2D(x, y));
        }
    }
}

Point2D Enemy::getRandomWalkableCell(){
    int idCell = Random::getRandomNumber(0, walkableCell.size() - 1);
    return walkableCell[idCell];
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

void Enemy::fillListStepToTarget(Point2D positionTarget){
    listStepToTarget.clear();

    positionTarget = Point2D(floor(positionTarget.getX()), floor(positionTarget.getY()));
    Point2D positionEnemy = Point2D(floor(positionCenter.getX() / MAP_CELL_SIZE), floor(positionCenter.getY() / MAP_CELL_SIZE));

    struct CellPath
    {
        bool visited = false;
        Point2D lastPosition;
    };
    
    CellPath pathMap[MAP_SIZE_Y][MAP_SIZE_X];
    queue<Point2D> queue;

    queue.push(positionEnemy);
    pathMap[(int)queue.front().getY()][(int)queue.front().getX()].lastPosition = Point2D(-1, -1);
    pathMap[(int)queue.front().getY()][(int)queue.front().getX()].visited = true;

    Point2D currentAnalysis;
    do
    {
        currentAnalysis = queue.front();
        
        int posX, posY;
        posX = (int)currentAnalysis.getX();
        posY = (int)currentAnalysis.getY() + 1;
        if(posY < MAP_SIZE_Y && !pathMap[posY][posX].visited && isWalkableCell(posX, posY)){
            queue.push(Point2D(posX, posY));
            pathMap[posY][posX].lastPosition = currentAnalysis;
            pathMap[posY][posX].visited = true;
        }

        posX = (int)currentAnalysis.getX();
        posY = (int)currentAnalysis.getY() - 1;
        if(posY >= 0 && !pathMap[posY][posX].visited && isWalkableCell(posX, posY)){
            queue.push(Point2D(posX, posY));
            pathMap[posY][posX].lastPosition = currentAnalysis;
            pathMap[posY][posX].visited = true;
        }

        posX = (int)currentAnalysis.getX() + 1;
        posY = (int)currentAnalysis.getY();
        if(posX < MAP_SIZE_X && !pathMap[posY][posX].visited && isWalkableCell(posX, posY)){
            queue.push(Point2D(posX, posY));
            pathMap[posY][posX].lastPosition = currentAnalysis;
            pathMap[posY][posX].visited = true;
        }

        posX = (int)currentAnalysis.getX() - 1;
        posY = (int)currentAnalysis.getY();
        if(posX >= 0 && !pathMap[posY][posX].visited && isWalkableCell(posX, posY)){
            queue.push(Point2D(posX, posY));
            pathMap[posY][posX].lastPosition = currentAnalysis;
            pathMap[posY][posX].visited = true;
        }

        queue.pop();

    }while(currentAnalysis != positionTarget && !queue.empty());

    if(currentAnalysis != positionTarget) return;

    if(pathMap[(int)currentAnalysis.getY()][(int)currentAnalysis.getX()].lastPosition == Point2D(-1, -1)) return;

    do{
        listStepToTarget.push_front(currentAnalysis);
        currentAnalysis = pathMap[(int)currentAnalysis.getY()][(int)currentAnalysis.getX()].lastPosition;
    }while(currentAnalysis != positionEnemy);
}

void Enemy::foundNewRandomTarget(){
    speed = ENEMY_SPEED_SEARCH;

    int id = Random::getRandomNumber(0, walkableCell.size() - 1);
    Point2D target = walkableCell[id];
    fillListStepToTarget(target);
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

    Point2D newLastPlayerPosition = Point2D(floor(player->getPositionCenter().getX() / MAP_CELL_SIZE), floor(player->getPositionCenter().getY() / MAP_CELL_SIZE));
    if(lastPlayerPosition != newLastPlayerPosition){
        fillListStepToTarget(newLastPlayerPosition);
        lastPlayerPosition = newLastPlayerPosition;
    }

}

void Enemy::setMap(const Cell (*map)[MAP_SIZE_X]){
    this->map = map;
    fillWalkableCell();
    foundNewRandomTarget();
}

void Enemy::setPlayer(const Player* player){
    this->player = player;
}