#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "settings.hpp"
#include "Point2D.hpp"
#include "Object2D.hpp"
#include "Rectangle.hpp"
#include "RayHit.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
using namespace std;

class UI
{
    private:
        sf::RectangleShape minimapBack;
        sf::RectangleShape outline;
        sf::RectangleShape strip;
        sf::RectangleShape enemyStrip;
        sf::RectangleShape staminaBarBack;
        sf::RectangleShape staminaBar;
        sf::View oldView;
        sf::View minimapView;
        sf::VertexArray ceiling;
        sf::VertexArray floor;
        sf::VertexArray minimapPlayerVisibilityArea;
        sf::Font font;
        sf::Text fpsText;

        Rectangle* rectangleForMinimap;
        Circle* circleForMinimap;

        float factorNearEnemy;
        bool enemyInChase;

        void minimapChangeView(sf::RenderWindow& window);
        void minimapDrawBackground(sf::RenderWindow& window);
        void minimapDrawPlayerRay(sf::RenderWindow& window, const vector<RayHit>& rayCastResult, const Object2D& player);
        void minimapDrawOutline(sf::RenderWindow& window);
        void minimapDrawAllObject(sf::RenderWindow& window, const vector<Object2D*>& arrayObjects, bool showItemOnMinimap);
        void minimapDrawVisibleObject(sf::RenderWindow& window, const vector<Object2D*>& arrayObjects, const Cell (*map)[MAP_SIZE_X], const Object2D& player);
        sf::Color getMixingColorExp(sf::Color color1, sf::Color color2, float factor);

    public:
        UI();
        void drawBackground(sf::RenderWindow& window, float currentScreenWidth, float currentScreenHeight);
        void drawMinimap(sf::RenderWindow& window, const vector<Object2D*>& arrayObjects, const vector<RayHit>& rayCastResult, const Object2D& player, const Cell (*map)[MAP_SIZE_X], bool showItemOnMinimap, bool areaVisionMinimap);
        void drawRayCastResult(sf::RenderWindow& window, const vector<RayHit>& rayCastResult, float currentScreenWidth, float currentScreenHeight);
        void drawStaminaBar(sf::RenderWindow& window, float currentStamina, float currentScreenWidth, float currentScreenHeight, bool sprintIsAvailable);
        void drawEnemy(sf::RenderWindow& window, const Enemy& enemy, const Player& player, const vector<RayHit>& rayCastResult, float currentScreenWidth, float currentScreenHeight);
        void drawFPS(sf::RenderWindow& window, float currentFPS, float currentScreenWidth);

        void setEnemyInfo(const float factorNearEnemy, const bool enemyInChase);
};