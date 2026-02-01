#include "../headers/World.hpp"

World::World()
    : window(sf::VideoMode(SCREEN_SIZE_X, SCREEN_SIZE_Y), "CastOut"){
    view = window.getDefaultView();

    if(font.loadFromFile("fonts/ArchiveUkr.ttf")){
        fpsText.setFont(font);
        fpsText.setCharacterSize(15);
        fpsText.setFillColor(sf::Color::Blue);
    }

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
        updateObjects(deltaTime);
        castRay(player->getPositionCenter(), player->getDirectionGaze());
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
    drawRayCastResult(rayCastResult);
    userInterface.drawMinimap(window, arrayObjects);
    updateFPS();

    window.display();
}


void World::updateObjects(float deltaTime){
    for(int i = 0; i < arrayObjects.size(); i++){
        arrayObjects[i]->updateObject(deltaTime);
    }
}

void World::castRay(Point2D startPointRay, float directionGaze){
    rayCastResult.clear();
    for(int i = 0; i < currentScreenWidth; i += RAY_FREQUENCY_IN_PIXEL){
        float screenX = 2.0f * i / (float)currentScreenWidth - 1.0f;
        float relativeAngle = atanf(screenX * FOV_TANGENT) * 180.0f / PI;;
        float currentAngle = player->getDirectionGaze() + relativeAngle;

        float minDistance = MAX_FLOAT;
        float minDistanceObjectHeight;
        sf::Color minDistanceObjectColor;
        float posX_directionVector = cos(currentAngle * PI / 180);
        float posY_directionVector = sin(currentAngle * PI / 180);
        for(int j = 0; j < arrayObjects.size();j++){
            if(arrayObjects[j]->getObjectType() != ObjectType::Player){
                float dist = arrayObjects[j]->intersectRay(player->getPositionCenter(), currentAngle, Point2D(posX_directionVector, posY_directionVector));
                if(minDistance > dist && dist != -1.0f){
                    minDistance = dist;
                    minDistanceObjectHeight = arrayObjects[j]->getObjectHeight();
                    minDistanceObjectColor = arrayObjects[j]->getColor();
                }
            }
        }
        float differentAngle = currentAngle - player->getDirectionGaze();
        minDistance *= cos(differentAngle * (PI / 180.0f));
        RayHit hit;
        if(minDistance != MAX_FLOAT){
            hit.distance = minDistance;
            hit.objectHeight = minDistanceObjectHeight;
            hit.color = minDistanceObjectColor;
            hit.hit = true;
        }
        else
            hit.hit = false;

        rayCastResult.push_back(hit);
    }
}

void World::drawRayCastResult(const vector<RayHit>& rayCastResult){
    sf::RectangleShape rectangle;
    float distanceToProjection = (currentScreenHeight / 2.0f) / FOV_TANGENT;
    for(int i = 0; i < rayCastResult.size(); i++){
        if(rayCastResult[i].hit){
            float objectHeight = rayCastResult[i].objectHeight;
            float distance = rayCastResult[i].distance;
            if (distance < 0.1f) distance = 0.1f;
            float heightStrip = objectHeight * distanceToProjection / distance;
            float stepStripX = currentScreenWidth / rayCastResult.size();

            sf::Color color = rayCastResult[i].color;
            float fogFactor = 1.0f - expf(-distance * DENSITY);;

            color.r = color.r * (1 - fogFactor) + BACKGROUND_COLOR.r * fogFactor;
            color.b = color.b * (1 - fogFactor) + BACKGROUND_COLOR.b * fogFactor;
            color.g = color.g * (1 - fogFactor) + BACKGROUND_COLOR.g * fogFactor;

            rectangle.setSize(sf::Vector2f(stepStripX, heightStrip));
            rectangle.setPosition(i * stepStripX, (currentScreenHeight - heightStrip)/2);
            rectangle.setFillColor(color);
            window.draw(rectangle);
        }
    }
}

void World::resizedEvent(){
    currentScreenWidth = event.size.width;
    currentScreenHeight = event.size.height;
    // Просто обновляем View под новый размер без черных полос
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

    fpsText.setPosition(currentScreenWidth - 62, 5);
    fpsText.setString("FPS: " + std::to_string(static_cast<int>(currentFPS)));

    window.draw(fpsText);

}

