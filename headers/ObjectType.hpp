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
    Enemy,
    Key

    
};

// Если меняешь типы, проверь тут

namespace ObjectTypeRules{
    inline bool isWalkable(ObjectType objT){
        return objT != ObjectType::RectangleWallAround && objT != ObjectType::RectangleWall && 
                objT != ObjectType::CircleWall && objT != ObjectType::CircleWallGenerator;
    }

    inline bool isPlaceForGeneration(ObjectType objT){
        return objT == ObjectType::None;
    }

    inline bool isHiddenOnMinimapTypeObject(ObjectType objT){
        return objT == ObjectType::Enemy || objT == ObjectType::Key;
    }

    inline bool isPlayerNotCoolisionTypeObject(ObjectType objT){
        return objT == ObjectType::Player || objT == ObjectType::Key || objT == ObjectType::Enemy;
    }
    inline bool isRayCastingTypeObject(ObjectType objT){
        return objT != ObjectType::Player && objT != ObjectType::Enemy && objT != ObjectType::Key;
    }

}
