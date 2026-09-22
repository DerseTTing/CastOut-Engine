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
const sf::Color WALL_COLOR = sf::Color(190, 190, 190);
const sf::Color WALL_COLOR_AFTER_HIT = sf::Color(120, 120, 120);
const sf::Color WALL_COLOR_AFTER_DEATH = sf::Color(143, 77, 77);
const float WALL_HEIGHT = 150.0f;

// Настройки игрока
const sf::Color PLAYER_COLOR = sf::Color(0, 153, 25);
const int PLAYER_RADIUS = 25;
const int PLAYER_HEALTH = 2;
const int PLAYER_SPEED = 250; 
const float PLAYER_SHIFT_SPEED_MULTIPLIER = 2.1f;
const float PLAYER_MAX_STAMINA = 2.5f;
const float PLAYER_MAX_STAMINA_PART_AFTER_HIT = 0.45f;
const float PLAYER_REGENERATION_STAMINA = 0.3f;
const float PLAYER_DEGENERATION_STAMINA = 0.9f;
const float PLAYER_PART_STAMINA_TO_SHIFT = 0.45f;
const float PLAYER_SCREEN_ROTATION_SENSITIVITY = 0.08f;
const float PLAYER_VIEWING_ANGLE = 90.0f;
const float PLAYER_TIME_ROTATION_TO_ENEMY = 0.30f;
const float PLAYER_TIME_SCREAM_TO_ENEMY = 0.8f;

// Настройки врага
const int ENEMY_RADIUS = 25;
const int ENEMY_HEIGHT = 150;
const int ENEMY_SPEED_SEARCH = 400;//400;
const int ENEMY_SPEED_CHASE = 310; //310;
const int ENEMY_RADIUS_DAMAGE = ENEMY_RADIUS + PLAYER_RADIUS + 5.0f;
const sf::Color ENEMY_COLOR = sf::Color(171, 37, 22);
const float ENEMY_FOG_DENSITY = 0.001f;
const float ENEMY_FOG_DENSITY_AFTER_HIT = ENEMY_FOG_DENSITY * 2.0f;

// Настройки ключа
const int KEY_COUNT = 5;
const int KEY_RADIUS = 19;
const int KEY_HEIGHT = 150;
const float KEY_PICK_UP_RADIUS = KEY_RADIUS + PLAYER_RADIUS - 10;
const sf::Color KEY_COLOR = sf::Color(179, 173, 0);
const sf::Color KEY_COLOR_AFTER_DEATH = sf::Color(79, 77, 0);
const float KEY_FOG_DENSITY = 0.0018f;
const float KEY_FOG_DENSITY_AFTER_HIT = KEY_FOG_DENSITY * 2.0f;

// Настройки счетчика ключей
const int COUNTER_KEY_TEXT_SIZE = 28;
const float COUNTER_KEY_BACKGROUND_SHARE_POSITION_X = 1.0f - COUNTER_KEY_BACKGROUND_SHARE_SIZE_X;
const float COUNTER_KEY_BACKGROUND_SHARE_POSITION_Y = 1.0f - COUNTER_KEY_BACKGROUND_SHARE_SIZE_Y;
const float COUNTER_KEY_BACKGROUND_SHARE_SIZE_X = 0.15f;
const float COUNTER_KEY_BACKGROUND_SHARE_SIZE_Y = 0.075f;
const float COUNTER_KEY_TEXT_SHARE_POSITION_X = COUNTER_KEY_BACKGROUND_SHARE_POSITION_X + 0.048f;
const float COUNTER_KEY_TEXT_SHARE_POSITION_Y = COUNTER_KEY_BACKGROUND_SHARE_POSITION_Y +  0.025f;
const sf::Color COUNTER_KEY_TEXT_COLOR = sf::Color::Blue;
const sf::Color COUNTER_KEY_BACKGROUND_COLOR = sf::Color(38, 38, 38);

// Настройки выхода
const float RECTANGLE_EXIT_DISTANCE_TRIGGER_FINISH = PLAYER_RADIUS;
const sf::Color RECTANGLE_EXIT_COLOR = sf::Color(73, 173, 50);
const sf::Color RECTANGLE_EXIT_COLOR_AFTER_DEATH = sf::Color(7, 36, 0);

// Настройки миникарты
const float MINIMAP_SHARE_SIZE = 0.3f;
const int MINIMAP_BOUNDED_REGION_CELL = 14;
const float MINIMAP_SIZE = static_cast<float>(SCREEN_SIZE_Y) * MINIMAP_SHARE_SIZE;
const float MINIMAP_OUTLINE_THICKNESS = 12.0f;
const sf::Color MINIMAP_OUTLINE_COLOR = sf::Color(23, 23, 23);
const sf::Color MINIMAP_WALL_COLOR = sf::Color(214, 214, 214);
const sf::Color MINIMAP_RECTANGLE_EXIT_COLOR = sf::Color(73, 173, 50);
const float MINIMAP_AREA_SIZE = static_cast<float>(MAP_SIZE_X * MAP_CELL_SIZE);
const sf::Color MINIMAP_BACK_COLOR = sf::Color(38, 38, 38);
const sf::Color MINIMAP_RAY_PLAYER_COLOR = sf::Color(15, 115, 0, 200);
const float MINIMAP_CHARACTER_SIZE_MULTIPLIER = 3.0f; // 4.0f



// Настройки Raycasting
const int RAY_FREQUENCY_IN_PIXEL = 1;

// Настройки фона
const float FOG_DENSITY = 0.003f;
const float FOG_DENSITY_AFTER_HIT = FOG_DENSITY * 1.7f;
const float DANGER_DENSITY = 0.0007f; // Больше - меньше цвета
const sf::Color BACKGROUND_COLOR_NEAR_FLOOR = sf::Color(51, 51, 51);
const sf::Color BACKGROUND_COLOR_NEAR_FLOOR_AFTER_DEATH = sf::Color(46, 28, 28);
const sf::Color BACKGROUND_COLOR_NEAR_CEILING = sf::Color(17, 17, 17);
const sf::Color BACKGROUND_COLOR_NEAR_CEILING_AFTER_DEATH = sf::Color(23, 16, 16);
const sf::Color BACKGROUND_COLOR_FAR = sf::Color(17, 17, 17);
const sf::Color BACKGROUND_COLOR_FAR_AFTER_DEATH = sf::Color(23, 16, 16);
const sf::Color DANGER_COLOR = sf::Color(97, 5, 2);
const sf::Color CAREFULLY_COLOR = sf::Color(181, 109, 0);


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


// Планы:
// 
// меню победы
// Ранение (снижение стамины и обзора миникарты, темнее миникарта)
// Здоровье
// 
// 
// Сделано:
// После альтаба ставиться пауза. После выхода из паузы не меняется поворот игрока
// баг с текстурами сквозь стены пофикшен
// Заметнее выход на карте
// Уменьшил сенсу на 20 процентов
// Ключи теперь больше растворяются в тумане
// слегка темнее стены и пол
// ранение. 
// чит код
// изменены цвета, + при ранинии
// уменьшил карту
/*
Вопросы:
Скороcnm врага
Сенса игрока
Длительность анимации ранения
Размер карты
Количество ключей
Дальность прорисовки до/после ранения














*/