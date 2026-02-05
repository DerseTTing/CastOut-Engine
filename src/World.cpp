#include "../headers/World.hpp"

World::World()
    : window(sf::VideoMode(SCREEN_SIZE_X, SCREEN_SIZE_Y), "CastOut"){
    view = window.getDefaultView();

    currentScreenWidth = SCREEN_SIZE_X;
    currentScreenHeight = SCREEN_SIZE_Y;
    generationMap();
}

World::~World() {
    for (Object2D* obj : arrayObjects) {
        delete obj;
    }
}

void World::runWorld(){
    while(window.isOpen()){
        while (window.pollEvent(event)){
            processingEvent();

        }
        deltaTime = clock.restart().asSeconds();

        castRay(player->getPositionCenter(), player->getDirectionGaze());
        updateObjects();
        updateFPS();

        drawFrame();
    }
}

void World::generationMap(){
    for(int i = 0; i < WALL_RECTANGLE_QUANTITY; i++){
        float posX = getRandomNumber(0.0f, MINIMAP_AREA_SIZE);
        float posY = getRandomNumber(0.0f, MINIMAP_AREA_SIZE);
        int width = getRandomNumber(WALL_RECTANGLE_SIZE_MIN, WALL_RECTANGLE_SIZE_MAX);
        int length = getRandomNumber(WALL_RECTANGLE_SIZE_MIN, WALL_RECTANGLE_SIZE_MAX);
        Rectangle* ptrObj = new Rectangle(Point2D(posX,posY), width, length, WALL_HEIGHT, WALL_RECTANGLE_COLOR, ObjectType::RectangleWall); 
        arrayObjects.push_back(ptrObj);
    }
    for(int i = 0; i < WALL_CIRCLE_QUANTITY; i++){
        float posX = getRandomNumber(0.0f, MINIMAP_AREA_SIZE);
        float posY = getRandomNumber(0.0f, MINIMAP_AREA_SIZE);
        int radius = getRandomNumber(WALL_CIRCLE_RADIUS_MIN, WALL_CIRCLE_RADIUS_MAX);
        Circle* ptrObj = new Circle(Point2D(posX,posY), radius, WALL_HEIGHT, WALL_CIRCLE_COLOR, ObjectType::CircleWall);
        arrayObjects.push_back(ptrObj);
    }

    float posX = getRandomNumber(0.0f, MINIMAP_AREA_SIZE);
    float posY = getRandomNumber(0.0f, MINIMAP_AREA_SIZE);
    player = new Player(Point2D(posX,posY), PLAYER_RADIUS, WALL_HEIGHT, PLAYER_COLOR, ObjectType::Player);
    arrayObjects.push_back(player);
}

void World::processingEvent(){
    if (event.type == sf::Event::Closed)
        window.close();
    if (event.type == sf::Event::Resized)
        resizedEvent();
        
}

void World::drawFrame(){
    window.clear();

    userInterface.drawBackground(window, currentScreenWidth, currentScreenHeight);
    userInterface.drawRayCastResult(window, rayCastResult, currentScreenWidth, currentScreenHeight);
    userInterface.drawMinimap(window, arrayObjects, rayCastResult, player->getPositionCenter());
    userInterface.drawFPS(window, currentFPS, currentScreenWidth);

    window.display();
}


void World::updateObjects(){
    for(int i = 0; i < arrayObjects.size(); i++){
        arrayObjects[i]->updateObject(arrayObjects, deltaTime);
    }
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

void World::resizedEvent(){
    currentScreenWidth = event.size.width;
    currentScreenHeight = event.size.height;
    view = sf::View(sf::FloatRect(0, 0, currentScreenWidth, currentScreenHeight));
    window.setView(view);
}

int World::getRandomNumber(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}

float World::getRandomNumber(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
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
