#include "../headers/RectangleExit.hpp"

RectangleExit::RectangleExit(const Point2D& positionCenter, float width, float length, float objectHeight, sf::Color color, ObjectType objectType)
:Rectangle(positionCenter, width, length, objectHeight, color, objectType){

}

void RectangleExit::drawOnMinimap(sf::Color color, sf::RenderWindow& window) const{
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(width, length));
    rectangle.setPosition(positionAngle.getX(), positionAngle.getY());
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

bool RectangleExit::getFinishGame(){
    return finishGame;
}