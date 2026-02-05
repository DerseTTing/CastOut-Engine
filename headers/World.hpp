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
#include "RayHit.hpp"
using namespace std;

class World
{   
    public:
        World();
        ~World();

        void runWorld();

        void generationMap();

        void processingEvent();
        void drawFrame();

        void resizedEvent();

        void updateObjects();
        void castRay(Point2D startPointRay, float directionGaze);
        
        float getRandomNumber(float min, float max);
        int getRandomNumber(int min, int max);

        void updateFPS();

    private:
        sf::RenderWindow window;
        sf::Event event;
        sf::View view;
        sf::Clock clock;

        UI userInterface;
        Player* player;
        float currentScreenWidth;
        float currentScreenHeight;

        float currentFPS;

        vector<Object2D*> arrayObjects;
        vector<RayHit> rayCastResult;
        float deltaTime;

        RayHit findClosestIntersection(Point2D startPoint, float currentAngle, Point2D& directionVector);
        RayHit createRayHit(float minDistance, float currentAngle, Object2D* closestObject);
};