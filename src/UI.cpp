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

void UI::drawMinimap(sf::RenderWindow& window, const vector<Object2D*>& arrayObjects, const vector<RayHit>& rayCastResult, const Object2D& player, bool showEnemyOnMinimap) {
    minimapChangeView(window);

    minimapDrawBackground(window);

    minimapDrawPlayerRay(window, rayCastResult, player);
    
    for (int i = 0; i < arrayObjects.size(); i++){
        if(arrayObjects[i]->getObjectType() == ObjectType::Enemy){
            if(showEnemyOnMinimap)
                arrayObjects[i]->drawOnMinimap(ENEMY_COLOR, window);
        }
        else if(arrayObjects[i]->getObjectType() == ObjectType::Player)
            arrayObjects[i]->drawOnMinimap(PLAYER_COLOR, window);
        else
            arrayObjects[i]->drawOnMinimap(MINIMAP_WALL_COLOR, window);
    }

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

void UI::minimapDrawPlayerRay(sf::RenderWindow& window, const vector<RayHit>& rayCastResult, const Object2D& player){
    Point2D playerPosition = player.getPositionCenter();
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
        if(rayCastResult[i].hit && rayCastResult[i].closestObject->getObjectType() != ObjectType::Enemy){
            float objectHeight = rayCastResult[i].closestObject->getObjectHeight();
            float distance = rayCastResult[i].distance;
            if (distance < 0.1f) distance = 0.1f;
            float heightStrip = objectHeight * distanceToProjection / distance;
            float stepStripX = currentScreenWidth / rayCastResult.size();
            
            //Вынести цвет отдельно
            sf::Color color = rayCastResult[i].closestObject->getColor();
            float fogFactor = 1.0f - expf(-distance * FOG_DENSITY);

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

void UI::drawStaminaBar(sf::RenderWindow& window, float currentStamina, float currentScreenWidth, float currentScreenHeight, bool sprintIsAvailable){
    staminaBarBack.setSize(sf::Vector2f(currentScreenWidth * (STAMINA_BAR_SIZE_SHARE_X + STAMINA_BAR_BACKGROUND_SHARE_INDENT), currentScreenHeight * (STAMINA_BAR_SIZE_SHARE_Y + STAMINA_BAR_BACKGROUND_SHARE_INDENT)));
    staminaBarBack.setPosition(currentScreenWidth * STAMINA_BAR_POSITION_SHARE_X, currentScreenHeight * STAMINA_BAR_POSITION_SHARE_Y);
    staminaBarBack.setFillColor(STAMINA_BAR_BACKGROUND_COLOR);
    window.draw(staminaBarBack);

    float biasPositionBar = STAMINA_BAR_SIZE_SHARE_Y * currentScreenHeight - currentScreenHeight * STAMINA_BAR_SIZE_SHARE_Y * (currentStamina / PLAYER_MAX_STAMINA);
    staminaBar.setSize(sf::Vector2f(currentScreenWidth * STAMINA_BAR_SIZE_SHARE_X, currentScreenHeight * STAMINA_BAR_SIZE_SHARE_Y * (currentStamina / PLAYER_MAX_STAMINA)));
    staminaBar.setPosition(currentScreenWidth * (STAMINA_BAR_POSITION_SHARE_X + STAMINA_BAR_BACKGROUND_SHARE_INDENT/2), currentScreenHeight * (STAMINA_BAR_POSITION_SHARE_Y + STAMINA_BAR_BACKGROUND_SHARE_INDENT/2) + biasPositionBar);
    if(currentStamina / PLAYER_MAX_STAMINA > PLAYER_PART_STAMINA_TO_SHIFT || sprintIsAvailable)
        staminaBar.setFillColor(STAMINA_BAR_COLOR);
    else
        staminaBar.setFillColor(STAMINA_BAR_COLOR_DEFICIENCY);
    window.draw(staminaBar);
}

void UI::drawEnemy(sf::RenderWindow& window, const Enemy& enemy, const Player& player, const vector<RayHit>& rayCastResult, float currentScreenWidth, float currentScreenHeight){
    Point2D positionEnemy = enemy.getPositionCenter();
    Point2D positionPlayer = player.getPositionCenter();
    Point2D vectorPlayerToEnemy = Point2D(positionEnemy.getX() - positionPlayer.getX(), positionEnemy.getY() - positionPlayer.getY());
    
    float relativeAngle = atan2(vectorPlayerToEnemy.getY(), vectorPlayerToEnemy.getX()) * RAD_TO_DEG - player.getDirectionGaze();
    while(relativeAngle < -180)
        relativeAngle += 360;
    while(relativeAngle > 180)
        relativeAngle -= 360;

    if(abs(relativeAngle) > PLAYER_VIEWING_ANGLE/2 + 45)
        return;

    float distanceToEnemy = vectorPlayerToEnemy.getLenghtVector();
    float distanceToProjection = (currentScreenHeight / 2) / FOV_TANGENT;
    float screenX = tan(relativeAngle * DEG_TO_RAD) / FOV_TANGENT;
    int centerEnemyOnScreenX = round((screenX + 1.0f) / 2.0f * currentScreenWidth);

    float halfAngle = atan2f(ENEMY_RADIUS, distanceToEnemy);
    float spriteHeight = ENEMY_HEIGHT * distanceToProjection / distanceToEnemy;
    float screenXRight = (ENEMY_RADIUS / distanceToEnemy) / FOV_TANGENT;
    int spriteRadius = round(screenXRight * currentScreenWidth / 2.0f);

    float stepStripX = currentScreenWidth / rayCastResult.size();

    float fogFactor = 1.0f - expf(-distanceToEnemy * ENEMY_FOG_DENSITY);
    sf::Color color = ENEMY_COLOR;
    color.r = color.r * (1 - fogFactor) + BACKGROUND_COLOR_FAR.r * fogFactor;
    color.g = color.g * (1 - fogFactor) + BACKGROUND_COLOR_FAR.g * fogFactor;
    color.b = color.b * (1 - fogFactor) + BACKGROUND_COLOR_FAR.b * fogFactor;

    for(int i = 0; i < rayCastResult.size(); i++){
        float rayAngle = rayCastResult[i].angle;
        float correctedDistanceToEnemy = distanceToEnemy * cos((rayAngle - player.getDirectionGaze()) * DEG_TO_RAD);
        if(i > centerEnemyOnScreenX - spriteRadius && i < centerEnemyOnScreenX + spriteRadius && rayCastResult[i].distanceWithoutCorrection > correctedDistanceToEnemy){
            enemyStrip.setSize(sf::Vector2f(stepStripX, spriteHeight));
            enemyStrip.setPosition(i * stepStripX, (currentScreenHeight - spriteHeight)/2);
            enemyStrip.setFillColor(color);
            window.draw(enemyStrip);
        }
    }


}

void UI::drawFPS(sf::RenderWindow& window, float currentFPS, float currentScreenWidth){
    fpsText.setPosition(currentScreenWidth - 62, 5);
    fpsText.setString("FPS: " + std::to_string(static_cast<int>(currentFPS)));
    window.draw(fpsText);
}