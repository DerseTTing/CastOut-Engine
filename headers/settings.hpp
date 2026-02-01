#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

// Настройки окна
constexpr int SCREEN_SIZE_X = 1920;
constexpr int SCREEN_SIZE_Y = 1080;

// Настройки генерации карты
constexpr int WALL_RECTANGLE_QUANTITY = 50;
constexpr int WALL_RECTANGLE_SIZE_MIN = 50;
constexpr int WALL_RECTANGLE_SIZE_MAX = 300;

constexpr int WALL_CIRCLE_QUANTITY = 50;
constexpr int WALL_CIRCLE_RADIUS_MIN = 30;
constexpr int WALL_CIRCLE_RADIUS_MAX = 100;

// Настройки стен
const sf::Color WALL_RECTANGLE_COLOR = sf::Color(214,214,214);
const sf::Color WALL_CIRCLE_COLOR = sf::Color(214,214,214);
const float WALL_HEIGHT = 150;

// Настройки игрока
const sf::Color PLAYER_COLOR = sf::Color(0, 153, 25);
constexpr int PLAYER_RADIUS = 20;
constexpr int PLAYER_SPEED = 150;
constexpr int PLAYER_SCREEN_ROTATION_SENSITIVITY = 100;
constexpr float PLAYER_VIEWING_ANGLE = 90;
constexpr float PLAYER_MAX_DROW_DISTANCE = 1200;

//Настройки заднего фона
const sf::Color BACKGROUND_COLOR = sf::Color(28, 28, 28);

// Настройки миникарты
constexpr float MINIMAP_SHARE_SIZE = 0.3f; //Часть от размера экрана по Y
constexpr float MINIMAP_SIZE = SCREEN_SIZE_Y * MINIMAP_SHARE_SIZE;
constexpr float MINIMAP_OUTLINE_THICKNESS = 12;
const sf::Color MINIMAP_OUTLINE_COLOR = sf::Color(23, 23, 23);
constexpr float MINIMAP_AREA_SIZE = 3000.0f;
const sf::Color MINIMAP_BACK_COLOR = sf::Color(38, 38, 38);
const sf::Color MINIMAP_WALL_COLOR = sf::Color(214,214,214);

// Настройки Raycasting
constexpr int RAY_FREQUENCY_IN_PIXEL = 1;

// Настройка фона
constexpr float DENSITY = 0.003f;


// Математика
constexpr float PI = 3.14159265358979323846f;
constexpr float MAX_FLOAT = 340282350000000000000000000000000000000.0f;
constexpr float LITLE_FLOAT = 1e-9f;
const float FOV_TANGENT = tan(PLAYER_VIEWING_ANGLE * PI / 180.0f / 2.0f);