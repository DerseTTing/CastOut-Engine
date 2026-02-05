#pragma once
#include <SFML/Graphics.hpp>

class Object2D;

struct RayHit {
    float distance;
    float distanceWithoutCorrection;
    float angle;
    Object2D* closestObject;
    bool hit;      
};