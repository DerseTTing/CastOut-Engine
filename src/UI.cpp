#include "../headers/UI.hpp"

UI::UI(){
    if(font.loadFromFile("fonts/ArchiveUkr.ttf")){
        fpsText.setFont(font);
        fpsText.setCharacterSize(15);
        fpsText.setFillColor(sf::Color::Blue);

        counterKeyText.setFont(font);
        counterKeyText.setCharacterSize(COUNTER_KEY_TEXT_SIZE);
        counterKeyText.setFillColor(COUNTER_KEY_TEXT_COLOR);
    }

    rectangleForMinimap = new Rectangle(Point2D(1, 1), 1, 1, 1, WALL_RECTANGLE_COLOR, ObjectType::None);
    circleForMinimap = new Circle(Point2D(1, 1), 1, 1, WALL_CIRCLE_COLOR, ObjectType::None);
}

void UI::drawBackground(sf::RenderWindow& window){
    ceilingObj.setPrimitiveType(sf::Quads);
    ceilingObj.resize(4);

    ceilingObj[0].position = sf::Vector2f(0, 0);
    ceilingObj[0].color = BACKGROUND_COLOR_NEAR_CEILING;
    
    ceilingObj[1].position = sf::Vector2f(currentScreenWidth, 0);
    ceilingObj[1].color = BACKGROUND_COLOR_NEAR_CEILING;
    
    ceilingObj[2].position = sf::Vector2f(currentScreenWidth, currentScreenHeight / 2.0f);
    ceilingObj[2].color = BACKGROUND_COLOR_FAR;
    
    ceilingObj[3].position = sf::Vector2f(0, currentScreenHeight / 2.0f);
    ceilingObj[3].color = BACKGROUND_COLOR_FAR;

    window.draw(ceilingObj);

    floorObj.setPrimitiveType(sf::Quads);
    floorObj.resize(4);
    
    floorObj[0].position = sf::Vector2f(0, currentScreenHeight / 2.0f);
    floorObj[0].color = BACKGROUND_COLOR_FAR;
    
    floorObj[1].position = sf::Vector2f(currentScreenWidth, currentScreenHeight / 2.0f);
    floorObj[1].color = BACKGROUND_COLOR_FAR;
    
    floorObj[2].position = sf::Vector2f(currentScreenWidth, currentScreenHeight);
    floorObj[2].color = BACKGROUND_COLOR_NEAR_FLOOR;
    
    floorObj[3].position = sf::Vector2f(0, currentScreenHeight);
    floorObj[3].color = BACKGROUND_COLOR_NEAR_FLOOR;

    window.draw(floorObj);
}

void UI::drawMinimap(sf::RenderWindow& window, const vector<Object2D*>& arrayObjects, const vector<RayHit>& rayCastResult, const Object2D& player, const Cell (*map)[MAP_SIZE_X], bool showHiddenObjectsOnMinimap, bool areaVisionMinimap) {
    minimapChangeView(window);

    minimapDrawBackground(window);

    minimapDrawPlayerRay(window, rayCastResult, player);
    
    if(areaVisionMinimap)
        minimapDrawVisibleObject(window, arrayObjects, map, player);
    else
        minimapDrawAllObject(window, arrayObjects, showHiddenObjectsOnMinimap);

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

void UI::minimapDrawAllObject(sf::RenderWindow& window, const vector<Object2D*>& arrayObjects, bool showHiddenObjectsOnMinimap){
    sf::Color objectColor;

    for (int i = 0; i < arrayObjects.size(); i++){
        ObjectType type = arrayObjects[i]->getObjectType();

        if(ObjectTypeRules::isHiddenOnMinimapTypeObject(type) && !showHiddenObjectsOnMinimap){
            if(type == ObjectType::RectangleExit){
                arrayObjects[i]->drawOnMinimap(MINIMAP_WALL_COLOR, window);
            }
            continue;
        }

        switch(type){
            case ObjectType::Player:
                objectColor = PLAYER_COLOR;
                break;
            case ObjectType::Enemy:
                objectColor = ENEMY_COLOR;
                break;
            case ObjectType::RectangleExit:
                objectColor = MINIMAP_RECTANGLE_EXIT_COLOR;
                break;
            default:
                objectColor = MINIMAP_WALL_COLOR;
                break;
        }

        arrayObjects[i]->drawOnMinimap(objectColor, window);

    }
}

void UI::minimapDrawVisibleObject(sf::RenderWindow& window, const vector<Object2D*>& arrayObjects, const Cell (*map)[MAP_SIZE_X], const Object2D& player){
    Point2D playerPosition = Point2D(floor(player.getPositionCenter().getX() / MAP_CELL_SIZE), floor(player.getPositionCenter().getY() / MAP_CELL_SIZE));

    for(int y = 0; y < MAP_SIZE_Y; y++){
        for(int x = 0; x < MAP_SIZE_X; x++){
            if((map[y][x].objectType == ObjectType::RectangleWall && (playerPosition - Point2D(x, y)).getLenghtVector() < MINIMAP_BOUNDED_REGION_CELL) || map[y][x].objectType == ObjectType::RectangleWallAround){
                rectangleForMinimap->setWidth(1 * MAP_CELL_SIZE);
                rectangleForMinimap->setLength(1 * MAP_CELL_SIZE);
                rectangleForMinimap->setPositionCenter(Point2D((x + 0.5f) * MAP_CELL_SIZE, (y + 0.5f) * MAP_CELL_SIZE));
                rectangleForMinimap->drawOnMinimap(MINIMAP_WALL_COLOR, window);
            }

            if((map[y][x].objectType == ObjectType::CircleGenerator || map[y][x].objectType == ObjectType::CircleWallGenerator) && (playerPosition - Point2D(x, y)).getLenghtVector() < MINIMAP_BOUNDED_REGION_CELL){
                int sizeCircle = 1;
                while(map[y][x + sizeCircle].objectType == ObjectType::CircleWall || map[y][x + sizeCircle].objectType == ObjectType::Circle){
                    sizeCircle++;
                }
                circleForMinimap->setRadius(sizeCircle * MAP_CELL_SIZE / 2.0f);
                circleForMinimap->setPositionCenter(Point2D((x + sizeCircle/2.0f) * MAP_CELL_SIZE, (y + sizeCircle/2.0f) * MAP_CELL_SIZE));
                circleForMinimap->drawOnMinimap(MINIMAP_WALL_COLOR, window);
            }

            if(map[y][x].objectType == ObjectType::RectangleExit ){
                if((playerPosition - Point2D(x, y)).getLenghtVector() < MINIMAP_BOUNDED_REGION_CELL){
                    rectangleForMinimap->setWidth(1 * MAP_CELL_SIZE);
                    rectangleForMinimap->setLength(1 * MAP_CELL_SIZE);
                    rectangleForMinimap->setPositionCenter(Point2D((x + 0.5f) * MAP_CELL_SIZE, (y + 0.5f) * MAP_CELL_SIZE));
                    rectangleForMinimap->drawOnMinimap(MINIMAP_RECTANGLE_EXIT_COLOR, window);
                }
                else{
                    rectangleForMinimap->setWidth(1 * MAP_CELL_SIZE);
                    rectangleForMinimap->setLength(1 * MAP_CELL_SIZE);
                    rectangleForMinimap->setPositionCenter(Point2D((x + 0.5f) * MAP_CELL_SIZE, (y + 0.5f) * MAP_CELL_SIZE));
                    rectangleForMinimap->drawOnMinimap(MINIMAP_WALL_COLOR, window);
                }
            }

        }
    }

    circleForMinimap->setRadius(PLAYER_RADIUS * MINIMAP_CHARACTER_SIZE_MULTIPLIER);
    circleForMinimap->setPositionCenter(Point2D(player.getPositionCenter().getX(), player.getPositionCenter().getY()));
    circleForMinimap->drawOnMinimap(PLAYER_COLOR, window);

}

void UI::minimapDrawOutline(sf::RenderWindow& window){
    outline.setSize(sf::Vector2f(MINIMAP_SIZE, MINIMAP_SIZE));
    outline.setPosition(0, 0);
    outline.setFillColor(sf::Color::Transparent);
    outline.setOutlineThickness(MINIMAP_OUTLINE_THICKNESS);
    outline.setOutlineColor(MINIMAP_OUTLINE_COLOR);
    window.draw(outline);
}

void UI::drawRayCastResult(sf::RenderWindow& window, const vector<RayHit>& rayCastResult, const vector<EdgeHit>& findEdges){
    float distanceToProjection = (currentScreenHeight / 2.0f) / FOV_TANGENT;
    int indexEngeRay = 0;

    RayHit rayHit;
    float positionX;
    float width;

    for(int i = 0; i < rayCastResult.size(); i++){
        if(i != rayCastResult.size() - 1){
            if(findEdges.size() > indexEngeRay && i == findEdges[indexEngeRay].leftRayIndex){
                const EdgeHit& edge = findEdges[indexEngeRay];

                float edgeX = std::max(i * stepStripX, std::min(edge.positionScreenX, (i + 1) * stepStripX));

                // До края
                float positionX = i * stepStripX;
                float width = edgeX - positionX;
                drawStrip(window, edge.hitBeforeEdge, positionX, width, distanceToProjection);

                // После края
                positionX = edgeX;
                width = (i + 1) * stepStripX - edgeX;
                drawStrip(window, edge.hitAfterEdge, positionX, width, distanceToProjection);

                indexEngeRay++;
            }
            else{
                drawStrip(window, rayCastResult[i], i * stepStripX, stepStripX, distanceToProjection);
            
            
            }
        }
    }
}

void UI::drawStrip(sf::RenderWindow& window, const RayHit& rayHit, float positionX, float width, float distanceToProjection){
    if(rayHit.hit){
        float objectHeight = rayHit.closestObject->getObjectHeight();
        float distance = rayHit.distance;
        if (distance < 0.1f) distance = 0.1f;
        float heightStrip = objectHeight * distanceToProjection / distance;

        sf::Color color;
        color = rayHit.closestObject->getColor();

        if(!ObjectTypeRules::isDangerColorImmuneTypeObject(rayHit.closestObject->getObjectType())){
            if(enemyInChase) color = getMixingColorExpNear(color, DANGER_COLOR, factorNearEnemy, DANGER_DENSITY);
            else if(enemyInCarefullyMode) color = getMixingColorLin(color, CAREFULLY_COLOR);
        }

        color = getMixingColorExpFar(color, BACKGROUND_COLOR_FAR, distance, FOG_DENSITY);

        strip.setSize(sf::Vector2f(width, heightStrip));
        strip.setPosition(positionX, (currentScreenHeight - heightStrip)/2);
        strip.setFillColor(color);
        window.draw(strip);

        

    }
}

void UI::drawStaminaBar(sf::RenderWindow& window, float currentStamina, bool sprintIsAvailable){
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

void UI::drawCounterKey(sf::RenderWindow& window, const Player& player){
    counterKeyBack.setSize(sf::Vector2f(currentScreenWidth * COUNTER_KEY_BACKGROUND_SHARE_SIZE_X, currentScreenHeight * COUNTER_KEY_BACKGROUND_SHARE_SIZE_Y));
    counterKeyBack.setPosition(currentScreenWidth * COUNTER_KEY_BACKGROUND_SHARE_POSITION_X, currentScreenHeight * COUNTER_KEY_BACKGROUND_SHARE_POSITION_Y);
    counterKeyBack.setFillColor(COUNTER_KEY_BACKGROUND_COLOR);
    window.draw(counterKeyBack);

    counterKeyText.setPosition(currentScreenWidth * COUNTER_KEY_TEXT_SHARE_POSITION_X, currentScreenHeight * COUNTER_KEY_TEXT_SHARE_POSITION_Y);
    counterKeyText.setString("KEY: " + to_string(static_cast<int>(player.getCountCollectedKey())) + "/" + to_string(static_cast<int>(KEY_COUNT)));
    window.draw(counterKeyText);
}

void UI::drawAllSprites(sf::RenderWindow& window, const vector<Object2D*>& arrayObjects, const vector<RayHit>& rayCastResult){
    vector<BillboardSprite> spritesToDraw;
    Player* player;
    for(int idObj = 0; idObj < arrayObjects.size(); idObj++){
        if(arrayObjects[idObj]->getObjectType() == ObjectType::Player)
            player = static_cast<Player*>(arrayObjects[idObj]);
    }

    for(int idObj = 0; idObj < arrayObjects.size(); idObj++){
        if(arrayObjects[idObj]->getObjectType() == ObjectType::Enemy){
            BillboardSprite sprite = createEnemySprite(window, *static_cast<Enemy*>(arrayObjects[idObj]), *player, rayCastResult);
            if(sprite.visible)
                spritesToDraw.push_back(sprite);
        }
        else if(arrayObjects[idObj]->getObjectType() == ObjectType::Key){
            BillboardSprite sprite = createKeySprite(window, *static_cast<Key*>(arrayObjects[idObj]), *player, rayCastResult);
            if(sprite.visible)
                spritesToDraw.push_back(sprite);
        }
    }

    while(spritesToDraw.size() > 0){
        float maxDistance = -1;
        int maxDistanceIdSprite = 0;
        BillboardSprite* maxDistanceSprite;
        for(int idSprite = 0; idSprite < spritesToDraw.size(); idSprite++){
            if(spritesToDraw[idSprite].distance > maxDistance){
                maxDistance = spritesToDraw[idSprite].distance;
                maxDistanceIdSprite = idSprite;
                maxDistanceSprite = &spritesToDraw[idSprite];
            }
        }

        drawSprite(window, rayCastResult, *maxDistanceSprite);
        spritesToDraw.erase(spritesToDraw.begin() + maxDistanceIdSprite);

    }
}

UI::BillboardSprite UI::createEnemySprite(sf::RenderWindow& window, const Enemy& enemy, const Player& player, const vector<RayHit>& rayCastResult){
    Point2D positionEnemy = enemy.getPositionCenter();
    Point2D positionPlayer = player.getPositionCenter();
    Point2D vectorPlayerToEnemy = Point2D(positionEnemy.getX() - positionPlayer.getX(), positionEnemy.getY() - positionPlayer.getY());
    
    float relativeAngle = atan2(vectorPlayerToEnemy.getY(), vectorPlayerToEnemy.getX()) * RAD_TO_DEG - player.getDirectionGaze();
    while(relativeAngle < -180)
        relativeAngle += 360;
    while(relativeAngle > 180)
        relativeAngle -= 360;

    if(abs(relativeAngle) > PLAYER_VIEWING_ANGLE/2 + 45){
        BillboardSprite invisible;
        invisible.visible = false;
        return invisible;

    }

    float distanceToEnemy = vectorPlayerToEnemy.getLenghtVector();
    float distanceToProjection = (currentScreenHeight / 2) / FOV_TANGENT;
    float screenX = tan(relativeAngle * DEG_TO_RAD) / FOV_TANGENT;
    float centerEnemyOnScreenX = (screenX + 1.0f) / 2.0f * currentScreenWidth / stepStripX;

    float correctedDistanceToEnemy = distanceToEnemy * cos(relativeAngle * DEG_TO_RAD);
    
    float spriteHeight = ENEMY_HEIGHT * distanceToProjection / correctedDistanceToEnemy;
    float screenXRight = (ENEMY_RADIUS / correctedDistanceToEnemy) / FOV_TANGENT;
    float spriteRadius = screenXRight * currentScreenWidth / 2.0f / stepStripX;


    sf::Color color = ENEMY_COLOR;
    color = getMixingColorExpFar(color, BACKGROUND_COLOR_FAR, distanceToEnemy, ENEMY_FOG_DENSITY);

    BillboardSprite spriteEnemy;
    spriteEnemy.leftSide = centerEnemyOnScreenX - spriteRadius;
    spriteEnemy.rightSide = centerEnemyOnScreenX + spriteRadius;
    spriteEnemy.height = spriteHeight;
    spriteEnemy.distance = distanceToEnemy;
    spriteEnemy.color = color;

    return spriteEnemy;
}

UI::BillboardSprite UI::createKeySprite(sf::RenderWindow& window, const Key& key, const Player& player, const vector<RayHit>& rayCastResult){
    Point2D positionKey = key.getPositionCenter();
    Point2D positionPlayer = player.getPositionCenter();
    Point2D vectorPlayerToKey = Point2D(positionKey.getX() - positionPlayer.getX(), positionKey.getY() - positionPlayer.getY());
    
    float relativeAngle = atan2(vectorPlayerToKey.getY(), vectorPlayerToKey.getX()) * RAD_TO_DEG - player.getDirectionGaze();
    while(relativeAngle < -180)
        relativeAngle += 360;
    while(relativeAngle > 180)
        relativeAngle -= 360;

    if(abs(relativeAngle) > PLAYER_VIEWING_ANGLE/2 + 45){
        BillboardSprite invisible;
        invisible.visible = false;
        return invisible;

    }

    float distanceToKey = vectorPlayerToKey.getLenghtVector();
    float distanceToProjection = (currentScreenHeight / 2) / FOV_TANGENT;
    float screenX = tan(relativeAngle * DEG_TO_RAD) / FOV_TANGENT;
    float centerEnemyOnScreenX = (screenX + 1.0f) / 2.0f * currentScreenWidth / stepStripX;

    float correctedDistanceToKey = distanceToKey * cos(relativeAngle * DEG_TO_RAD);
    
    float spriteHeight = KEY_HEIGHT * distanceToProjection / correctedDistanceToKey;
    float screenXRight = (KEY_RADIUS / correctedDistanceToKey) / FOV_TANGENT;
    float spriteRadius = screenXRight * currentScreenWidth / 2.0f / stepStripX;


    sf::Color color = KEY_COLOR;
    color = getMixingColorExpFar(color, BACKGROUND_COLOR_FAR, distanceToKey, ENEMY_FOG_DENSITY);

    BillboardSprite spriteKey;
    spriteKey.leftSide = centerEnemyOnScreenX - spriteRadius;
    spriteKey.rightSide = centerEnemyOnScreenX + spriteRadius;
    spriteKey.height = spriteHeight;
    spriteKey.distance = distanceToKey;
    spriteKey.color = color;

    return spriteKey;
}

void UI::drawSprite(sf::RenderWindow& window, const vector<RayHit>& rayCastResult, const BillboardSprite& spriteInfo){
    for(int i = 0; i < rayCastResult.size(); i++){
        if(i >= floor(spriteInfo.leftSide) && i < ceil(spriteInfo.rightSide) && rayCastResult[i].distanceWithoutCorrection > spriteInfo.distance){
            float pixelLeft  = max((float)i, spriteInfo.leftSide) * stepStripX;
            float pixelRight = min((float)i + 1,spriteInfo.rightSide) * stepStripX;
            float enemyStripPixelShape = pixelRight - pixelLeft;

            enemyStrip.setSize(sf::Vector2f(enemyStripPixelShape, spriteInfo.height));
            enemyStrip.setPosition(pixelLeft, (currentScreenHeight - spriteInfo.height)/2);
            enemyStrip.setFillColor(spriteInfo.color);
            window.draw(enemyStrip);
        }

    }
}

sf::Color UI::getMixingColorExpNear(sf::Color color1, sf::Color color2, float factor, float density){
    float intensity = exp(-factor * density);

    sf::Uint8 r = static_cast<sf::Uint8>(clamp(color1.r + round((color2.r - color1.r) * intensity), 0.0f, 255.0f));
    sf::Uint8 g = static_cast<sf::Uint8>(clamp(color1.g + round((color2.g - color1.g) * intensity), 0.0f, 255.0f));
    sf::Uint8 b = static_cast<sf::Uint8>(clamp(color1.b + round((color2.b - color1.b) * intensity), 0.0f, 255.0f));

    return sf::Color(r, g, b);

}

sf::Color UI::getMixingColorExpFar(sf::Color color1, sf::Color color2, float factor, float density){
    float intensity = 1.0f - expf(-factor * density);

    sf::Uint8 r = static_cast<sf::Uint8>(clamp(color1.r * (1 - intensity) + color2.r * intensity, 0.0f, 255.0f));
    sf::Uint8 b = static_cast<sf::Uint8>(clamp(color1.b * (1 - intensity) + color2.b * intensity, 0.0f, 255.0f));
    sf::Uint8 g = static_cast<sf::Uint8>(clamp(color1.g * (1 - intensity) + color2.g * intensity, 0.0f, 255.0f));

    return sf::Color(r, g, b);

}

sf::Color UI::getMixingColorLin(sf::Color color1, sf::Color color2){
    sf::Uint8 r = static_cast<sf::Uint8>(clamp((color1.r + color2.r) / 2.0f, 0.0f, 255.0f));
    sf::Uint8 b = static_cast<sf::Uint8>(clamp((color1.b + color2.b) / 2.0f, 0.0f, 255.0f));
    sf::Uint8 g = static_cast<sf::Uint8>(clamp((color1.g + color2.g) / 2.0f, 0.0f, 255.0f));

    return sf::Color(r, g, b);
}

void UI::drawFPS(sf::RenderWindow& window, float currentFPS){
    fpsText.setPosition(currentScreenWidth - 62, 5);
    fpsText.setString("FPS: " + std::to_string(static_cast<int>(currentFPS)));
    window.draw(fpsText);
}

void UI::setEnemyInfo(const float factorNearEnemy, const bool enemyInChase, const bool enemyInCarefullyMode){
    if(factorNearEnemy < 0)
        this->factorNearEnemy = 0;
    else
        this->factorNearEnemy = factorNearEnemy;

    this->enemyInChase = enemyInChase;
    this->enemyInCarefullyMode = enemyInCarefullyMode;
}

void UI::setScreenInfo(float currentScreenWidth, float currentScreenHeight, float stepStripX){
    this->currentScreenWidth = currentScreenWidth;
    this->currentScreenHeight = currentScreenHeight;
    this->stepStripX = stepStripX;
}