#pragma once
#include <memory>
#include <random>

class Random
{
    public:
        static float getRandomNumber(float min, float max);
        static int getRandomNumber(int min, int max);
};