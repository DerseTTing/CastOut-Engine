#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "settings.hpp"
#include "Point2D.hpp"
#include "Object2D.hpp"
#include "Rectangle.hpp"
#include "RayHit.hpp"
using namespace std;

class UI
{
    private:
        sf::RectangleShape minimapBack;
        sf::RectangleShape outline;
        sf::RectangleShape strip;
        sf::View oldView;
        sf::View minimapView;
        sf::VertexArray ceiling;
        sf::VertexArray floor;
        sf::VertexArray minimapPlayerVisibilityArea;
        sf::Font font;
        sf::Text fpsText;

        void minimapChangeView(sf::RenderWindow& window);
        void minimapDrawBackground(sf::RenderWindow& window);
        void minimapDrawPlayerRay(sf::RenderWindow& window, const vector<RayHit>& rayCastResult, const Point2D& playerPosition);
        void minimapDrawOutline(sf::RenderWindow& window);

    public:
        UI();
        void drawBackground(sf::RenderWindow& window, float currentScreenWidth, float currentScreenHeight);
        void drawMinimap(sf::RenderWindow& window, const vector<Object2D*>& arrayObjects, const vector<RayHit>& rayCastResult, const Point2D& playerPosition);
        void drawRayCastResult(sf::RenderWindow& window, const vector<RayHit>& rayCastResult, float currentScreenWidth, float currentScreenHeight);
        void drawFPS(sf::RenderWindow& window, float currentFPS, float currentScreenWidth);
};