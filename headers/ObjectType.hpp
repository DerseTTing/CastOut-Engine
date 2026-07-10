#pragma once

enum class ObjectType
{
    None,
    RectangleWall,
    RectangleWallAround,
    Door,
    CircleWall,
    CircleWallGenerator,
    CircleGenerator,
    Circle,
    Player,
    Enemy
};

// Если меняешь непроходимые типы, обнови Enemy::isWalkableCell()