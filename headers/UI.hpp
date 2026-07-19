#pragma once
#include <iostream>
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
#include "Key.hpp"
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
        sf::RectangleShape counterKeyBack;
        sf::View oldView;
        sf::View minimapView;
        sf::VertexArray ceilingObj;
        sf::VertexArray floorObj;
        sf::VertexArray minimapPlayerVisibilityArea;
        sf::Font font;
        sf::Text fpsText;
        sf::Text counterKeyText;


        Rectangle* rectangleForMinimap;
        Circle* circleForMinimap;

        float currentScreenWidth;
        float currentScreenHeight;
        float stepStripX;

        float factorNearEnemy;
        bool enemyInChase;
        bool enemyInCarefullyMode;

        struct BillboardSprite {
            float leftSide;
            float rightSide;
            float height;
            float distance;
            sf::Color color;
            bool visible = true;
        };
  

        void minimapChangeView(sf::RenderWindow& window);
        void minimapDrawBackground(sf::RenderWindow& window);
        void minimapDrawPlayerRay(sf::RenderWindow& window, const vector<RayHit>& rayCastResult, const Object2D& player);
        void minimapDrawOutline(sf::RenderWindow& window);
        void minimapDrawAllObject(sf::RenderWindow& window, const vector<Object2D*>& arrayObjects, bool showHiddenObjectsOnMinimap);
        void minimapDrawVisibleObject(sf::RenderWindow& window, const vector<Object2D*>& arrayObjects, const Cell (*map)[MAP_SIZE_X], const Object2D& player);
        void drawSprite(sf::RenderWindow& window, const vector<RayHit>& rayCastResult, const BillboardSprite& spriteInfo);
        void drawStrip(sf::RenderWindow& window, const RayHit& rayHit, float positionX, float width, float distanceToProjection);
        BillboardSprite createEnemySprite(sf::RenderWindow& window, const Enemy& enemy, const Player& player, const vector<RayHit>& rayCastResult);
        BillboardSprite createKeySprite(sf::RenderWindow& window,  const Key& key, const Player& player, const vector<RayHit>& rayCastResult);
        sf::Color getMixingColorExpNear(sf::Color color1, sf::Color color2, float factor, float density);
        sf::Color getMixingColorExpFar(sf::Color color1, sf::Color color2, float factor, float density);
        sf::Color getMixingColorLin(sf::Color color1, sf::Color color2);

    public:
        UI();
        void drawBackground(sf::RenderWindow& window);
        void drawMinimap(sf::RenderWindow& window, const vector<Object2D*>& arrayObjects, const vector<RayHit>& rayCastResult, const Object2D& player, const Cell (*map)[MAP_SIZE_X], bool showHiddenObjectsOnMinimap, bool areaVisionMinimap);
        void drawRayCastResult(sf::RenderWindow& window, const vector<RayHit>& rayCastResult, const vector<EdgeHit>& findEdges);
        void drawStaminaBar(sf::RenderWindow& window, float currentStamina, bool sprintIsAvailable);
        void drawCounterKey(sf::RenderWindow& window, const Player& player);
        void drawAllSprites(sf::RenderWindow& window, const vector<Object2D*>& arrayObjects, const vector<RayHit>& rayCastResult);
        void drawFPS(sf::RenderWindow& window, float currentFPS);

        void setEnemyInfo(const float factorNearEnemy, const bool enemyInChase, const bool enemyInCarefullyMode);
        void setScreenInfo(float currentScreenWidth, float currentScreenHeight, float stepStripX);
};