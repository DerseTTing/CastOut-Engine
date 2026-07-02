#include "../headers/settings.hpp"

// Настройки окна
const int SCREEN_SIZE_X = 1920;
const int SCREEN_SIZE_Y = 1080;

// Настройки генерации карты
const int MAP_CELL_SIZE = 64;
const int MAP_NUMBER_ROOM_DIVISIONS = 8;
const float MAP_ROOM_DIVISIONS_SKIP_PERCENT = 0.45f;
const int MAP_ROOM_DIVISIONS_SKIP_START = 4;
const float MAP_ROOM_DIVISIONS_PROPORTION_MAX = 0.65f;
const float MAP_ROOM_DIVISIONS_PROPORTION_MIN = 0.35f;
const float MAP_DOOR_PERCENT = 0.07f;

// Генерация комнат
const int ROOM_MIN_SIZE = 8;
const int ROOM_SIZE_EMPTY = 9; // 0-12
const int ROOM_SIZE_LITTLE = 14; // 12-16
const int ROOM_SIZE_MIDLE = 18; // 16-20

// Настройки стен
const sf::Color WALL_RECTANGLE_COLOR = sf::Color(214, 214, 214);
const sf::Color WALL_CIRCLE_COLOR = sf::Color(214, 214, 214);
const float WALL_HEIGHT = 150.0f;

// Настройки игрока
const sf::Color PLAYER_COLOR = sf::Color(0, 153, 25);
const int PLAYER_RADIUS = 25;
const int PLAYER_SPEED = 250;
const float PLAYER_SHIFT_SPEED_MULTIPLIER = 2.1f;
const float PLAYER_MAX_STAMINA = 2.5f;
const float PLAYER_REGENERATION_STAMINA = 0.3f;
const float PLAYER_DEGENERATION_STAMINA = 0.9f;
const float PLAYER_PART_STAMINA_TO_SHIFT = 0.45f;
const float PLAYER_SCREEN_ROTATION_SENSITIVITY = 0.1f;
const float PLAYER_VIEWING_ANGLE = 90.0f;

// Настройки врага
const int ENEMY_RADIUS = 25;
const int ENEMY_HEIGHT = 150;
const int ENEMY_SPEED_SEARCH = 440;
const int ENEMY_SPEED_CHASE = 290;
const sf::Color ENEMY_COLOR = sf::Color(171, 37, 22);
const float ENEMY_FOG_DENSITY = 0.001f;


// Настройки миникарты
const float MINIMAP_SHARE_SIZE = 0.3f;
const float MINIMAP_SIZE = static_cast<float>(SCREEN_SIZE_Y) * MINIMAP_SHARE_SIZE;
const float MINIMAP_OUTLINE_THICKNESS = 12.0f;
const sf::Color MINIMAP_OUTLINE_COLOR = sf::Color(23, 23, 23);
const sf::Color MINIMAP_WALL_COLOR = sf::Color(214, 214, 214);
const float MINIMAP_AREA_SIZE = static_cast<float>(MAP_SIZE_X * MAP_CELL_SIZE);
const sf::Color MINIMAP_BACK_COLOR = sf::Color(38, 38, 38);
const sf::Color MINIMAP_RAY_PLAYER_COLOR = sf::Color(15, 115, 0, 200);
const float MINIMAP_RAY_PLAYER_THICKNESS = 1.0f;
const float MINIMAP_CHARACTER_SIZE_MULTIPLIER = 3.5f;

// Настройки Raycasting
const int RAY_FREQUENCY_IN_PIXEL = 1;

// Настройки фона
const float FOG_DENSITY = 0.003f;
const sf::Color BACKGROUND_COLOR_NEAR_FLOOR = sf::Color(59, 59, 59);
const sf::Color BACKGROUND_COLOR_NEAR_CEILING = sf::Color(18, 18, 18);
const sf::Color BACKGROUND_COLOR_FAR = sf::Color(18, 18, 18);

// Настройки стамин бара
const float STAMINA_BAR_SIZE_SHARE_X = 0.01f;
const float STAMINA_BAR_SIZE_SHARE_Y = 0.4f;
const float STAMINA_BAR_BACKGROUND_SHARE_INDENT = 0.01f;
const float STAMINA_BAR_POSITION_SHARE_X = 1.0f - STAMINA_BAR_SIZE_SHARE_X - STAMINA_BAR_BACKGROUND_SHARE_INDENT;
const float STAMINA_BAR_POSITION_SHARE_Y = 0.5f;
const sf::Color STAMINA_BAR_BACKGROUND_COLOR = sf::Color(38, 38, 38);
const sf::Color STAMINA_BAR_COLOR = sf::Color(219, 219, 219);
const sf::Color STAMINA_BAR_COLOR_DEFICIENCY = sf::Color(79, 58, 58);

// Вычисляемые параметры
const float FOV_TANGENT = tan(PLAYER_VIEWING_ANGLE * DEG_TO_RAD / 2.0f);

// Организовать нормально шансы генерации комнат
// 
// Фикс багов (сквозь стены)
// Отключение миникарты
// Красный фон при приближении врага
// Ключи
// Ранение
// Убийство 
// Выход
//