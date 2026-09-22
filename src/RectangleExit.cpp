#include "../headers/RectangleExit.hpp"

RectangleExit::RectangleExit(const Point2D& positionCenter, float width, float length, float objectHeight, sf::Color color, ObjectType objectType)
:Rectangle(positionCenter, width, length, objectHeight, color, objectType){

}

void RectangleExit::drawOnMinimap(sf::Color color, sf::RenderWindow& window) const{
    sf::RectangleShape rectangle;

    float extraBefore = 0.0f;
    float extraAfter = 0.0f;

    if(sizeOnMinimap.extendBefore)
        extraBefore = MAP_CELL_SIZE;

    if(sizeOnMinimap.extendAfter)
        extraAfter = MAP_CELL_SIZE;

    float drawX = positionAngle.getX();
    float drawY = positionAngle.getY();
    float sizeX = width;
    float sizeY = length;

    if(sizeOnMinimap.extendAlongY){
        sizeY += extraBefore + extraAfter;
        drawY -= extraBefore;
    }
    else{
        sizeX += extraBefore + extraAfter;
        drawX -= extraBefore;
    }

    rectangle.setSize(sf::Vector2f(sizeX, sizeY));
    rectangle.setPosition(drawX, drawY);
    rectangle.setFillColor(color);
    window.draw(rectangle);
}

void RectangleExit::updateObject(const vector<Object2D*>& arrayObjects, float deltaTime){
    Player* player;

    for(int i = 0; i < arrayObjects.size();i++){
        if(arrayObjects[i]->getObjectType() == ObjectType::Player)
            player = static_cast<Player*>(arrayObjects[i]);
    }

    if(player->getCountCollectedKey() == KEY_COUNT)
        isOpen = true;

    if(isOpen && (player->getPositionCenter() - positionCenter).getLenghtVector() < RECTANGLE_EXIT_DISTANCE_TRIGGER_FINISH){
        finishGame = true;
    }

}

Point2D RectangleExit::distanceToObject(Point2D point, float playerRadius){
    if(isOpen)
        return Point2D(0, 0);
    return Rectangle::distanceToObject(point, playerRadius);
}

void RectangleExit::setFinishGame(bool finishGame){
    this->finishGame = finishGame;
}

void RectangleExit::setSizeOnMinimap(bool extendAlongY, bool extendBefore, bool extendAfter){
    sizeOnMinimap.extendAlongY = extendAlongY;
    sizeOnMinimap.extendBefore = extendBefore;
    sizeOnMinimap.extendAfter = extendAfter;
}

bool RectangleExit::getFinishGame(){
    return finishGame;
}