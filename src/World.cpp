#include "../headers/World.hpp"

World::World()
    : window(sf::VideoMode(SCREEN_SIZE_X, SCREEN_SIZE_Y), "CastOut"){
    view = window.getDefaultView();

    resizedEvent();

    window.setMouseCursorVisible(false);
    window.setMouseCursorGrabbed(true);

    generationMap();
}

World::~World() {
    for (int i = 0; i < arrayObjects.size(); i++) {
        delete arrayObjects[i];
    }
}

void World::runWorld(){
    while(window.isOpen()){
        while (window.pollEvent(event)){
            processingEvent();
        }

        if(worldIsRunning){
            deltaTime = clock.restart().asSeconds();

            updateObjects();

            castRay(player->getPositionCenter(), player->getDirectionGaze());

            updateFPS();

            drawFrame();
        }
    }
}

void World::restartWorld(){
    for (int i = 0; i < arrayObjects.size(); i++) {
            delete arrayObjects[i];
    }
    arrayObjects.clear();
    mapObj.clearMap();
    
    generationMap();

    finishGame = false;
    wasRotationToEnemyLastFrame = false;
    userInterface.resetInterface();
    userInterface.showInterface();
    resumeWorld();

}

void World::pauseWorld(){
    if(!worldIsRunning)
        return;

    window.setMouseCursorGrabbed(false);
    window.setMouseCursorVisible(true);
    worldIsRunning = false;
}

void World::resumeWorld(){
    sf::Mouse::setPosition(sf::Vector2i(currentScreenWidth/2, currentScreenHeight/2), window);

    if(worldIsRunning)
        return;

    clock.restart();
    window.setMouseCursorGrabbed(true);
    window.setMouseCursorVisible(false);
    worldIsRunning = true;
}

void World::stopWorld(){
    finishGame = true;
    pauseWorld();
}


void World::generationMap(){
    arrayObjects = mapObj.generationMap();

    for(int i = 0; i < arrayObjects.size(); i++){
        if(arrayObjects[i]->getObjectType() == ObjectType::Player)
            player = static_cast<Player*>(arrayObjects[i]);
        if(arrayObjects[i]->getObjectType() == ObjectType::Enemy){
            enemy = static_cast<Enemy*>(arrayObjects[i]);
        }
        if(arrayObjects[i]->getObjectType() == ObjectType::RectangleExit){
            rectangleExit = static_cast<RectangleExit*>(arrayObjects[i]);
        }
    }

    enemy->setPlayer(player);
}

void World::processingEvent(){
    if (event.type == sf::Event::Closed)
        window.close();
    else if (event.type == sf::Event::Resized)
        resizedEvent();
    else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
        restartWorld();
    else if((event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Numpad7) || event.type == sf::Event::LostFocus) && !finishGame)
        pauseWorld();
    else if((event.type == sf::Event::MouseButtonPressed) && !finishGame)
        resumeWorld();
    else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::V && showHiddenObjectsOnMinimap)
        showHiddenObjectsOnMinimap = false;
    else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::V && !showHiddenObjectsOnMinimap)
        showHiddenObjectsOnMinimap = true;
    else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::M && areaVisionMinimap)
        areaVisionMinimap = false;
    else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::M && !areaVisionMinimap)
        areaVisionMinimap = true;
    else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::K && userInterface.getIsHiddenInterface())
        userInterface.showInterface();
    else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::K && !userInterface.getIsHiddenInterface())
        userInterface.hideInterface();

}

void World::resizedEvent(){
    currentScreenWidth = event.size.width;
    currentScreenHeight = event.size.height;
    view = sf::View(sf::FloatRect(0, 0, currentScreenWidth, currentScreenHeight));
    window.setView(view);
    

    countRays = static_cast<int>(ceil(currentScreenWidth / (float)RAY_FREQUENCY_IN_PIXEL));
    stepStripX = currentScreenWidth / (float)countRays;

    userInterface.setScreenInfo(currentScreenWidth, currentScreenHeight, stepStripX);
}

void World::drawFrame(){
    window.clear();


    userInterface.drawFrame(window, (player->getPositionCenter() - enemy->getPositionCenter()).getLenghtVector() - ENEMY_RADIUS, enemy->getEnemyInChase(), enemy->getEnemyInCarefullyMode(), rayCastResult, 
    findEdges, arrayObjects, *player, mapObj.getMap(), showHiddenObjectsOnMinimap, areaVisionMinimap, player->getStamina(), player->getSprintIsAvailable(), currentFPS);


    window.display();
}


void World::updateObjects(){
    resetMouseDelta();
    player->setMouseDelta(mouseDelta);

    for(int i = 0; i < arrayObjects.size(); i++){
        arrayObjects[i]->updateObject(arrayObjects, deltaTime);
    }

    cheakCatchPlayer();
    checkDeleteObject();
    if(rectangleExit->getFinishGame() || player->getHealth() <= 0){
        userInterface.hideInterface();
        stopWorld();
    } 
    
        
}

void World::checkDeleteObject(){
    for(int i = arrayObjects.size() - 1; i >= 0; i--){
        if(arrayObjects[i]->getMarkedForDeletion()){
            delete(arrayObjects[i]);
            arrayObjects.erase(arrayObjects.begin() + i);
        }
    }
}

void World::resetMouseDelta(){
     if(!window.hasFocus()){
        mouseDelta = 0.0f;
        return;
    }

    sf::Vector2i windowCenter(currentScreenWidth / 2, currentScreenHeight / 2);
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    int deltaX = mousePos.x - windowCenter.x;

    sf::Mouse::setPosition(windowCenter, window);

    if(std::abs(deltaX) < currentScreenWidth / 2)
        mouseDelta = deltaX * PLAYER_SCREEN_ROTATION_SENSITIVITY;
    else
        mouseDelta = 0.0f;   
}

void World::castRay(Point2D startPointRay, float directionGaze){
    rayCastResult.clear();

    for(int k = 0; k < countRays; k++){
        float pixelX = k * stepStripX;
        float screenX = 2.0f * pixelX / (float)currentScreenWidth - 1.0f;
        float relativeAngle = atanf(screenX * FOV_TANGENT) * RAD_TO_DEG;
        float currentAngle = player->getDirectionGaze() + relativeAngle;

        Point2D directionVector(cos(currentAngle * DEG_TO_RAD), sin(currentAngle * DEG_TO_RAD));
        
        RayHit hit = findClosestIntersection(startPointRay, currentAngle, directionVector);

        rayCastResult.push_back(hit);
    }
    searchEdges();
}

void World::searchEdges(){
    findEdges.clear();

    for(int i = 0; i < rayCastResult.size() - 1; i++){
        if(rayCastResult[i].hit && rayCastResult[i + 1].hit && rayCastResult[i].closestObject != rayCastResult[i+1].closestObject){
            float angleLeft  = rayCastResult[i].angle;
            float angleRight = rayCastResult[i+1].angle;
            Object2D* objLeft = rayCastResult[i].closestObject;
            Object2D* objRight = rayCastResult[i+1].closestObject;

            RayHit hitAtLeftAngle = rayCastResult[i];
            RayHit hitAtRightAngle = rayCastResult[i+1];

            for(int iter = 0; iter < 10; iter++){
                float midAngle = (angleLeft + angleRight) / 2.0f;
                Point2D dir(cos(midAngle * DEG_TO_RAD), sin(midAngle * DEG_TO_RAD));
                RayHit midHit = findClosestIntersection(player->getPositionCenter(), midAngle, dir);


                if(midHit.closestObject == objLeft){
                    hitAtLeftAngle = midHit;
                    angleLeft = midAngle;
                }
                else{
                    angleRight = midAngle;
                    hitAtRightAngle = midHit;
                }
            }

            float edgeAngle = (angleLeft + angleRight) * 0.5f;
            float relativeAngle = edgeAngle - player->getDirectionGaze();
            float screenX = tan(relativeAngle * DEG_TO_RAD) / FOV_TANGENT;
            float positionScreanX = (screenX + 1.0f) / 2.0f * currentScreenWidth;

            findEdges.push_back({positionScreanX, i, hitAtLeftAngle, hitAtRightAngle});
            
        }
    }

}

RayHit World::findClosestIntersection(Point2D startPoint, float currentAngle, Point2D& directionVector){
    float minDistance = MAX_FLOAT;
    Object2D* closestObject = nullptr;

    for(int i = 0; i < arrayObjects.size();i++){
            if(ObjectTypeRules::isRayCastingTypeObject(arrayObjects[i]->getObjectType())){
                float dist = arrayObjects[i]->intersectRay(startPoint, directionVector);
                if(minDistance > dist && dist != -1.0f){
                    minDistance = dist;
                    closestObject = arrayObjects[i];
                }
            }
        }
    return createRayHit(minDistance, currentAngle, closestObject);
}

RayHit World::createRayHit(float minDistance, float currentAngle, Object2D* closestObject){
    RayHit hit;
    if(minDistance != MAX_FLOAT && closestObject != nullptr){
        hit.distanceWithoutCorrection = minDistance;

        float differentAngle = currentAngle - player->getDirectionGaze();
        minDistance *= cos(differentAngle * (PI / 180.0f));

        hit.distance = minDistance;
        hit.closestObject = closestObject;
        hit.angle = currentAngle;
        hit.hit = true;
    }
    else{
        hit.distanceWithoutCorrection = MINIMAP_AREA_SIZE * 2;
        hit.angle = currentAngle;
        hit.closestObject = nullptr;
        hit.hit = false;
    }

    return hit;
}

void World::cheakCatchPlayer(){
    if(enemy->getDealsDamage() && !player->getIsCatchToEnemy() && !wasRotationToEnemyLastFrame){
        player->startRotationToEnemy(enemy->getPositionCenter());
        wasRotationToEnemyLastFrame = true;
        userInterface.hideInterface();
    }
    else if(enemy->getDealsDamage() && !player->getIsCatchToEnemy()){
        enemy->teleportToFarWalkableCell(player->getPositionCenter());
        wasRotationToEnemyLastFrame = false;
        userInterface.registerHit(player->getHealth());
        userInterface.showInterface();
    }
}

void World::updateFPS() {

    static int frameCount = 0;
    static sf::Clock fpsClock;
    frameCount++;

    if (fpsClock.getElapsedTime().asMilliseconds() >= 100) {
        currentFPS = frameCount / (fpsClock.restart().asMilliseconds() / 1000.0f);
        frameCount = 0;
    }
}
