#pragma once
#include "ObjectType.hpp"

struct Cell {
    ObjectType objectType = ObjectType::None;
    bool indestructible = false;
    bool createObject = false;
};