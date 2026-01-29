#include "../headers/UI.hpp"

void UI::drawBackground(sf::RenderWindow& window){
    background1.setSize(sf::Vector2f(SCREEN_SIZE_X, SCREEN_SIZE_Y - MINIMAP_SIZE));
    background1.setPosition(0, MINIMAP_SIZE);
    background1.setFillColor(BACKGROUND_COLOR);
    window.draw(background1);

    background2.setSize(sf::Vector2f(SCREEN_SIZE_X - MINIMAP_SIZE, SCREEN_SIZE_Y));
    background2.setPosition(MINIMAP_SIZE, 0);
    background2.setFillColor(BACKGROUND_COLOR);
    window.draw(background2);
}

void UI::drawMinimap(sf::RenderWindow& window, const vector<Object2D*>& arrayObjects){
    minimapBack.setSize(sf::Vector2f(SCREEN_SIZE_X, SCREEN_SIZE_Y));
    minimapBack.setPosition(0, 0);
    minimapBack.setFillColor(MINIMAP_BACK_COLOR);
    window.draw(minimapBack);

    for(int i = 0; i < arrayObjects.size(); i++){
        arrayObjects[i]->drawOnMinimap(window);
    }
}