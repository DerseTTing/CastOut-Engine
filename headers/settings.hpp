#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

// Математика
inline constexpr float PI = 3.14159265358979323846f;
inline constexpr float DEG_TO_RAD = PI / 180.0f;
inline constexpr float RAD_TO_DEG = 180.0f / PI;
inline constexpr float MAX_FLOAT = 3.4028235e38f;
inline constexpr float LITLE_FLOAT = 1e-9f;

// Настройки окна
extern const int SCREEN_SIZE_X;
extern const int SCREEN_SIZE_Y;

// Настройки генерации карты
inline constexpr int MAP_SIZE_X = 80;
inline constexpr int MAP_SIZE_Y = 80;
extern const int MAP_CELL_SIZE;
extern const int MAP_NUMBER_ROOM_DIVISIONS;
extern const float MAP_ROOM_DIVISIONS_SKIP_PERCENT;
extern const int MAP_ROOM_DIVISIONS_SKIP_START;
extern const float MAP_ROOM_DIVISIONS_PROPORTION_MAX;
extern const float MAP_ROOM_DIVISIONS_PROPORTION_MIN;
extern const float MAP_DOOR_PERCENT;

// Генерация комнат
extern const int ROOM_MIN_SIZE;
extern const int ROOM_SIZE_EMPTY;
extern const int ROOM_SIZE_LITTLE;
extern const int ROOM_SIZE_MIDLE;

// Настройки стен
extern const sf::Color WALL_RECTANGLE_COLOR;
extern const sf::Color WALL_CIRCLE_COLOR;
extern const float WALL_HEIGHT;

// Настройки игрока
extern const sf::Color PLAYER_COLOR;
extern const int PLAYER_RADIUS;
extern const int PLAYER_SPEED;
extern const float PLAYER_SHIFT_SPEED_MULTIPLIER;
extern const float PLAYER_MAX_STAMINA;
extern const float PLAYER_REGENERATION_STAMINA;
extern const float PLAYER_DEGENERATION_STAMINA;
extern const float PLAYER_PART_STAMINA_TO_SHIFT;
extern const float PLAYER_SCREEN_ROTATION_SENSITIVITY;
extern const float PLAYER_VIEWING_ANGLE;

// Настройки врага
extern const int ENEMY_RADIUS;

// Настройки миникарты
extern const float MINIMAP_SHARE_SIZE;
extern const float MINIMAP_SIZE;
extern const float MINIMAP_OUTLINE_THICKNESS;
extern const sf::Color MINIMAP_OUTLINE_COLOR;
extern const sf::Color MINIMAP_WALL_COLOR;
extern const float MINIMAP_AREA_SIZE;
extern const sf::Color MINIMAP_BACK_COLOR;
extern const sf::Color MINIMAP_WALL_COLOR;
extern const sf::Color MINIMAP_RAY_PLAYER_COLOR;
extern const float MINIMAP_RAY_PLAYER_THICKNESS;
extern const float MINIMAP_CHARACTER_SIZE_MULTIPLIER;

// Настройки Raycasting
extern const int RAY_FREQUENCY_IN_PIXEL;

// Настройка фона
extern const float DENSITY;
extern const sf::Color BACKGROUND_COLOR_NEAR_FLOOR;
extern const sf::Color BACKGROUND_COLOR_NEAR_CEILING;
extern const sf::Color BACKGROUND_COLOR_FAR;

// Настройки стамин бара
extern const float STAMINA_BAR_SIZE_SHARE_X;
extern const float STAMINA_BAR_SIZE_SHARE_Y;
extern const float STAMINA_BAR_BACKGROUND_SHARE_INDENT;
extern const float STAMINA_BAR_POSITION_SHARE_X;
extern const float STAMINA_BAR_POSITION_SHARE_Y;
extern const sf::Color STAMINA_BAR_BACKGROUND_COLOR;
extern const sf::Color STAMINA_BAR_COLOR;
extern const sf::Color STAMINA_BAR_COLOR_DEFICIENCY;

// Вычисляемые параметры
extern const float FOV_TANGENT;