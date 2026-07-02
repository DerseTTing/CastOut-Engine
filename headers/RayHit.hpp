#pragma once
#include <SFML/Graphics.hpp>
#include "settings.hpp"

class Object2D;

struct RayHit {
    float distance = MAX_FLOAT;
    float distanceWithoutCorrection;
    float angle;
    Object2D* closestObject;
    bool hit;      
};