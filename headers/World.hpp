#pragma once
#include <memory>
#include <vector>
#include <random>
#include "Object2D.hpp"
#include "settings.hpp"
#include "Rectangle.hpp"
#include "UI.hpp"
#include "Circle.hpp"
#include "Player.hpp"
#include "ObjectType.hpp"
using namespace std;

class World
{
    private:
        sf::RenderWindow window;
        sf::Event event;
        sf::View view;
        sf::Clock clock;

        UI userInterface;
        Player* player;
        float currentScreenWidth;
        float currentScreenHeight;

        struct RayHit {
            float distance;
            sf::Color color;
            float objectHeight;
            bool hit;      
        };

        vector<Object2D*> arrayObjects;
        vector<RayHit> rayCastResult;
        float deltaTime;


    public:
        World();
        ~World();

        void runWorld();

        void generationMap();

        void processingEvent();
        void drawFrame();

        void resizedEvent();

        void updateObjects(float deltaTime);
        void castRay(Point2D startPointRay, float directionGaze);
        void drawRayCastResult(const vector<RayHit>& rayCastResult);
        
        float getRandomNumber(float min, float max);
        int getRandomNumber(int min, int max);

};