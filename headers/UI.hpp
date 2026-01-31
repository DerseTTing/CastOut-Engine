#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "settings.hpp"
#include "Point2D.hpp"
#include "Object2D.hpp"
#include "Rectangle.hpp"
using namespace std;

class UI
{
    private:
        sf::RectangleShape minimapBack;
        sf::RectangleShape outline;
        sf::RectangleShape background;
        sf::View oldView;
        sf::View minimapView;
    public:
        void drawBackground(sf::RenderWindow& window, float currentScreenWidth, float currentScreenHeight);
        void drawMinimap(sf::RenderWindow& window, const vector<Object2D*>& arrayObjects);
      
};