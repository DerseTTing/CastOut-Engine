#include "../headers/World.hpp"

World::World()
    : window(sf::VideoMode(SCREEN_SIZE_X, SCREEN_SIZE_Y), "CastOut"){
    view = window.getDefaultView();

    minimapSize = SCREEN_SIZE_Y * MINIMAP_SHARE_SIZE;
    GenerationMap();
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
        drawFrame();
    }
}

void World::GenerationMap(){
    sf::Color color = WALL_COLOR;
    for(int i = 0; i < WALL_QUANTITY; i++){
        float posX = getRandomNumber(0.0f, MINIMAP_AREA_SIZE);
        float posY = getRandomNumber(0.0f, MINIMAP_AREA_SIZE);
        int width = getRandomNumber(WALL_SIZE_MIN, WALL_SIZE_MAX);
        int height = getRandomNumber(WALL_SIZE_MIN, WALL_SIZE_MAX);
        Rectangle* ptrObj = new Rectangle(Point2D(posX,posY), width, height, color);
        arrayObjects.push_back(ptrObj);
    }
}

void World::processingEvent(){
    if (event.type == sf::Event::Closed)
        window.close();
    if (event.type == sf::Event::Resized)
        resizedEvent();
        
}

void World::drawFrame(){
    window.clear();

    userInterface.drawMinimap(window, arrayObjects);
    userInterface.drawBackground(window);

    window.display();
}

void World::resizedEvent(){
    float currentWidth = event.size.width;
    float currentHeight = event.size.height;

    float defaultRatio = (float)SCREEN_SIZE_X / (float)SCREEN_SIZE_Y;
    float currentRatio = currentWidth / currentHeight;

    float posX = 0;
    float posY = 0;
    float viewWidth = 1.0f;
    float viewHeight = 1.0f;
    if(defaultRatio < currentRatio){
        viewWidth = defaultRatio / currentRatio;
        posX = (1.0f - viewWidth) / 2.0f;
    }
    else{
        viewHeight = currentRatio / defaultRatio;
        posY = (1.0f - viewHeight) / 2.0f;
    }
    view.setViewport(sf::FloatRect(posX, posY, viewWidth, viewHeight));
    window.setView(view);

    minimapSize = currentHeight * MINIMAP_SHARE_SIZE;
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
