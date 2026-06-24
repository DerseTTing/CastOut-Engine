#include "../headers/World.hpp"

World::World()
    : window(sf::VideoMode(SCREEN_SIZE_X, SCREEN_SIZE_Y), "CastOut"){
    view = window.getDefaultView();

    currentScreenWidth = SCREEN_SIZE_X;
    currentScreenHeight = SCREEN_SIZE_Y;

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

            castRay(player->getPositionCenter(), player->getDirectionGaze());
            updateObjects();

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
    map.clearMap();
    
    generationMap();
}

void World::pauseWorld(){
    if(!worldIsRunning)
        return;

    window.setMouseCursorGrabbed(false);
    window.setMouseCursorVisible(true);
    worldIsRunning = false;
}

void World::resumeWorld(){
    if(worldIsRunning)
        return;

    clock.restart();
    window.setMouseCursorGrabbed(true);
    window.setMouseCursorVisible(false);
    worldIsRunning = true;
}


void World::generationMap(){
    arrayObjects = map.generationMap();

    for(int i = 0; i < arrayObjects.size(); i++){
        if(arrayObjects[i]->getObjectType() == ObjectType::Player)
            player = static_cast<Player*>(arrayObjects[i]);
    }
}

void World::processingEvent(){
    if (event.type == sf::Event::Closed)
        window.close();
    else if (event.type == sf::Event::Resized)
        resizedEvent();
    else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
        restartWorld();
    else if(event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Numpad7) || event.type == sf::Event::LostFocus)
        pauseWorld();
    else if(event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::GainedFocus)
        resumeWorld();
}

void World::resizedEvent(){
    currentScreenWidth = event.size.width;
    currentScreenHeight = event.size.height;
    view = sf::View(sf::FloatRect(0, 0, currentScreenWidth, currentScreenHeight));
    window.setView(view);
}

void World::drawFrame(){
    window.clear();

    userInterface.drawBackground(window, currentScreenWidth, currentScreenHeight);
    userInterface.drawRayCastResult(window, rayCastResult, currentScreenWidth, currentScreenHeight);
    userInterface.drawMinimap(window, arrayObjects, rayCastResult, *player);
    userInterface.drawStaminaBar(window, player->getStamina(), currentScreenWidth, currentScreenHeight, player->getSprintIsAvailable());
    userInterface.drawFPS(window, currentFPS, currentScreenWidth);

    window.display();
}


void World::updateObjects(){
    for(int i = 0; i < arrayObjects.size(); i++){
        if(arrayObjects[i]->getObjectType() != ObjectType::Player)
            arrayObjects[i]->updateObject(arrayObjects, deltaTime);
    }

    resetMouseDelta();
    player->updatePlayer(arrayObjects, mouseDelta, deltaTime);
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
    for(int i = 0; i < currentScreenWidth; i += RAY_FREQUENCY_IN_PIXEL){
        float screenX = 2.0f * i / (float)currentScreenWidth - 1.0f;
        float relativeAngle = atanf(screenX * FOV_TANGENT) * RAD_TO_DEG;
        float currentAngle = player->getDirectionGaze() + relativeAngle;

        Point2D directionVector(cos(currentAngle * DEG_TO_RAD), sin(currentAngle * DEG_TO_RAD));
        
        RayHit hit = findClosestIntersection(startPointRay, currentAngle, directionVector);

        rayCastResult.push_back(hit);
    }
}
RayHit World::findClosestIntersection(Point2D startPoint, float currentAngle, Point2D& directionVector){
    float minDistance = MAX_FLOAT;
    Object2D* closestObject = nullptr;

    for(int i = 0; i < arrayObjects.size();i++){
            if(arrayObjects[i]->getObjectType() != ObjectType::Player){
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

void World::updateFPS() {

    static int frameCount = 0;
    static sf::Clock fpsClock;
    frameCount++;

    if (fpsClock.getElapsedTime().asMilliseconds() >= 100) {
        currentFPS = frameCount / (fpsClock.restart().asMilliseconds() / 1000.0f);
        frameCount = 0;
    }
}
