#include "../headers/UI.hpp"

UI::UI(){
    if(font.loadFromFile("fonts/ArchiveUkr.ttf")){
        fpsText.setFont(font);
        fpsText.setCharacterSize(15);
        fpsText.setFillColor(sf::Color::Blue);
    }
}

void UI::drawBackground(sf::RenderWindow& window, float currentScreenWidth, float currentScreenHeight){
    ceiling.setPrimitiveType(sf::Quads);
    ceiling.resize(4);

    ceiling[0].position = sf::Vector2f(0, 0);
    ceiling[0].color = BACKGROUND_COLOR_NEAR_CEILING;
    
    ceiling[1].position = sf::Vector2f(currentScreenWidth, 0);
    ceiling[1].color = BACKGROUND_COLOR_NEAR_CEILING;
    
    ceiling[2].position = sf::Vector2f(currentScreenWidth, currentScreenHeight / 2.0f);
    ceiling[2].color = BACKGROUND_COLOR_FAR;
    
    ceiling[3].position = sf::Vector2f(0, currentScreenHeight / 2.0f);
    ceiling[3].color = BACKGROUND_COLOR_FAR;

    window.draw(ceiling);

    floor.setPrimitiveType(sf::Quads);
    floor.resize(4);
    
    floor[0].position = sf::Vector2f(0, currentScreenHeight / 2.0f);
    floor[0].color = BACKGROUND_COLOR_FAR;
    
    floor[1].position = sf::Vector2f(currentScreenWidth, currentScreenHeight / 2.0f);
    floor[1].color = BACKGROUND_COLOR_FAR;
    
    floor[2].position = sf::Vector2f(currentScreenWidth, currentScreenHeight);
    floor[2].color = BACKGROUND_COLOR_NEAR_FLOOR;
    
    floor[3].position = sf::Vector2f(0, currentScreenHeight);
    floor[3].color = BACKGROUND_COLOR_NEAR_FLOOR;

    window.draw(floor);
}

void UI::drawMinimap(sf::RenderWindow& window, const vector<Object2D*>& arrayObjects, const vector<RayHit>& rayCastResult, const Point2D& playerPosition) {
    minimapChangeView(window);

    minimapDrawBackground(window);

    minimapDrawPlayerRay(window, rayCastResult, playerPosition);

    for (int i = 0; i < arrayObjects.size(); i++) 
        arrayObjects[i]->drawOnMinimap(window);

    window.setView(oldView);

    minimapDrawOutline(window);
}

void UI::minimapChangeView(sf::RenderWindow& window){
    oldView = window.getView();
    minimapView.reset(sf::FloatRect(0, 0, MINIMAP_AREA_SIZE, MINIMAP_AREA_SIZE));
    float vWidth = (float)MINIMAP_SIZE / window.getSize().x;
    float vHeight = (float)MINIMAP_SIZE / window.getSize().y;
    minimapView.setViewport(sf::FloatRect(0.0f, 0.0f, vWidth, vHeight));
    window.setView(minimapView);
}

void UI::minimapDrawBackground(sf::RenderWindow& window){
    minimapBack.setSize(sf::Vector2f(MINIMAP_AREA_SIZE, MINIMAP_AREA_SIZE));
    minimapBack.setPosition(0, 0);
    minimapBack.setFillColor(MINIMAP_BACK_COLOR);
    window.draw(minimapBack);
}

void UI::minimapDrawPlayerRay(sf::RenderWindow& window, const vector<RayHit>& rayCastResult, const Point2D& playerPosition){
    minimapPlayerVisibilityArea.setPrimitiveType(sf::TriangleFan);
    minimapPlayerVisibilityArea.resize(rayCastResult.size()+1);

    minimapPlayerVisibilityArea[0].position = sf::Vector2f(playerPosition.getX(), playerPosition.getY());
    minimapPlayerVisibilityArea[0].color = MINIMAP_RAY_PLAYER_COLOR;

    for(int i = 0; i < rayCastResult.size(); i++){
        float posX = playerPosition.getX() + rayCastResult[i].distanceWithoutCorrection * cos(rayCastResult[i].angle * DEG_TO_RAD);
        float posY = playerPosition.getY() + rayCastResult[i].distanceWithoutCorrection * sin(rayCastResult[i].angle * DEG_TO_RAD);

        minimapPlayerVisibilityArea[i+1].position = sf::Vector2f(posX, posY);
        minimapPlayerVisibilityArea[i+1].color = MINIMAP_RAY_PLAYER_COLOR;
    }

    window.draw(minimapPlayerVisibilityArea);
}

void UI::minimapDrawOutline(sf::RenderWindow& window){
    outline.setSize(sf::Vector2f(MINIMAP_SIZE, MINIMAP_SIZE));
    outline.setPosition(0, 0);
    outline.setFillColor(sf::Color::Transparent);
    outline.setOutlineThickness(MINIMAP_OUTLINE_THICKNESS);
    outline.setOutlineColor(MINIMAP_OUTLINE_COLOR);
    window.draw(outline);
}

void UI::drawRayCastResult(sf::RenderWindow& window, const vector<RayHit>& rayCastResult, float currentScreenWidth, float currentScreenHeight){
    float distanceToProjection = (currentScreenHeight / 2.0f) / FOV_TANGENT;
    for(int i = 0; i < rayCastResult.size(); i++){
        if(rayCastResult[i].hit){
            float objectHeight = rayCastResult[i].closestObject->getObjectHeight();
            float distance = rayCastResult[i].distance;
            if (distance < 0.1f) distance = 0.1f;
            float heightStrip = objectHeight * distanceToProjection / distance;
            float stepStripX = currentScreenWidth / rayCastResult.size();
            
            //Вынести цвет отдельно
            sf::Color color = rayCastResult[i].closestObject->getColor();
            float fogFactor = 1.0f - expf(-distance * DENSITY);;

            color.r = color.r * (1 - fogFactor) + BACKGROUND_COLOR_FAR.r * fogFactor;
            color.b = color.b * (1 - fogFactor) + BACKGROUND_COLOR_FAR.b * fogFactor;
            color.g = color.g * (1 - fogFactor) + BACKGROUND_COLOR_FAR.g * fogFactor;

            strip.setSize(sf::Vector2f(stepStripX, heightStrip));
            strip.setPosition(i * stepStripX, (currentScreenHeight - heightStrip)/2);
            strip.setFillColor(color);
            window.draw(strip);
        }
    }
}

void UI::drawFPS(sf::RenderWindow& window, float currentFPS, float currentScreenWidth){
    fpsText.setPosition(currentScreenWidth - 62, 5);
    fpsText.setString("FPS: " + std::to_string(static_cast<int>(currentFPS)));
    window.draw(fpsText);
}