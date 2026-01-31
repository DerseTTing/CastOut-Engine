#include "../headers/UI.hpp"

void UI::drawBackground(sf::RenderWindow& window, float currentScreenWidth, float currentScreenHeight){
    background.setSize(sf::Vector2f(currentScreenWidth, currentScreenHeight));
    background.setPosition(0, 0);
    background.setFillColor(BACKGROUND_COLOR);
    window.draw(background);
}

void UI::drawMinimap(sf::RenderWindow& window, const vector<Object2D*>& arrayObjects) {
    oldView = window.getView();

    minimapView.reset(sf::FloatRect(0, 0, MINIMAP_AREA_SIZE, MINIMAP_AREA_SIZE));

    float vWidth = (float)MINIMAP_SIZE / window.getSize().x;
    float vHeight = (float)MINIMAP_SIZE / window.getSize().y;
    
    minimapView.setViewport(sf::FloatRect(0.0f, 0.0f, vWidth, vHeight));

    window.setView(minimapView);

    minimapBack.setSize(sf::Vector2f(MINIMAP_AREA_SIZE, MINIMAP_AREA_SIZE));
    minimapBack.setPosition(0, 0);
    minimapBack.setFillColor(MINIMAP_BACK_COLOR);
    window.draw(minimapBack);

    for (auto* obj : arrayObjects) {
        obj->drawOnMinimap(window);
    }

    window.setView(oldView);

    outline.setSize(sf::Vector2f(MINIMAP_SIZE, MINIMAP_SIZE));
    outline.setPosition(0, 0);
    outline.setFillColor(sf::Color::Transparent);
    outline.setOutlineThickness(MINIMAP_OUTLINE_THICKNESS);
    outline.setOutlineColor(MINIMAP_OUTLINE_COLOR);
    window.draw(outline);
}