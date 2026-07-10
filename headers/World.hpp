#pragma once
#include <iostream>
#include <vector>
#include "Object2D.hpp"
#include "settings.hpp"
#include "Rectangle.hpp"
#include "UI.hpp"
#include "Circle.hpp"
#include "Player.hpp"
#include "ObjectType.hpp"
#include "RayHit.hpp"
#include "Map.hpp"
using namespace std;

class World
{   
    public:
        World();
        ~World();

        void runWorld();
        void restartWorld();
        void pauseWorld();
        void resumeWorld();

        void generationMap();

        void processingEvent();
        void drawFrame();

        void resizedEvent();

        void updateObjects();

        void resetMouseDelta();

        void castRay(Point2D startPointRay, float directionGaze);

        void updateFPS();

    private:
        sf::RenderWindow window;
        sf::Event event;
        sf::View view;
        sf::Clock clock;

        UI userInterface;
        Map mapObj;
        Player* player;
        Enemy* enemy;
        float currentScreenWidth;
        float currentScreenHeight;

        bool worldIsRunning = true;

        float currentFPS;

        float mouseDelta = 0.0f;

        bool showItemOnMinimap = false;
        bool areaVisionMinimap = true;

        vector<Object2D*> arrayObjects;
        vector<RayHit> rayCastResult;
        float deltaTime;

        RayHit findClosestIntersection(Point2D startPoint, float currentAngle, Point2D& directionVector);
        RayHit createRayHit(float minDistance, float currentAngle, Object2D* closestObject);
};