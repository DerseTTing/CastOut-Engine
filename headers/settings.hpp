#pragma once
#include <SFML/Graphics.hpp>

//Настройки окна
constexpr int SCREEN_SIZE_X = 1920;
constexpr int SCREEN_SIZE_Y = 1080;

// Настройки генерации карты
constexpr int WALL_QUANTITY = 5;
constexpr int WALL_SIZE_MIN = 75;
constexpr int WALL_SIZE_MAX = 175;
const sf::Color WALL_COLOR = sf::Color(214,214,214);

// НАСТРОЙКИ UI //


const sf::Color BACKGROUND_COLOR = sf::Color(28, 28, 28);

// Настройки миникарты
constexpr float MINIMAP_SHARE_SIZE = 0.2f; //Часть от размера экрана по Y
constexpr float MINIMA_SIZE = SCREEN_SIZE_Y * MINIMAP_SHARE_SIZE;
constexpr float MINIMAP_AREA_SIZE = 1000.0f;
constexpr float MINIMAP_SCALE = (SCREEN_SIZE_Y*MINIMAP_SHARE_SIZE)/MINIMAP_AREA_SIZE;
const sf::Color MINIMAP_BACK_COLOR = sf::Color(38, 38, 38);
const sf::Color MINIMAP_WALL_COLOR = sf::Color(214,214,214);