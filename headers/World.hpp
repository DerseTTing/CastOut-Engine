#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "Object2D.hpp"
#include "settings.hpp"
#include "Rectangle.hpp"
#include "UI.hpp"
using namespace std;

class World
{
    private:
        sf::RenderWindow window;
        sf::Event event;
        sf::View view;

        UI userInterface;

        vector<Object2D*> arrayObjects;
        float minimapSize;

    public:
        World();
        ~World();

        void runWorld();

        void GenerationMap();

        void processingEvent();
        void drawFrame();

        void resizedEvent();

        float getRandomNumber(float min, float max);
        int getRandomNumber(int min, int max);

};