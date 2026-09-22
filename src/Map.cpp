#include "../headers/Map.hpp"

vector<Object2D*> Map::generationMap(){
    generationWallsAround();
    generationRooms();
    fullRooms();
    generationPlayer();
    generationEnemy();
    generationKey();
    generationExit();


    return createArrayObjects();
}

void Map::generationWallsAround(){
    for(int y = 0; y < MAP_SIZE_Y; y++){
        for(int x = 0; x < MAP_SIZE_X; x++){
            if(y == 0 || x == 0 || y == MAP_SIZE_Y - 1 || x == MAP_SIZE_X - 1){
                map[y][x].objectType = ObjectType::RectangleWallAround;
                map[y][x].indestructible = true;
                map[y][x].createObject = true;
            }
        }
    }
}

void Map::generationRooms(){
    vector<Room> roomsBuffer1;
    vector<Room> roomsBuffer2;
    vector<Room>* pointerRoomSave = &roomsBuffer2;
    vector<Room>* pointerRoomShare = &roomsBuffer1;
    Room firstRoom;
    firstRoom.setPositionAngle(Point2D(1, 1));
    firstRoom.setWidth(MAP_SIZE_X - 2);
    firstRoom.setLenght(MAP_SIZE_Y - 2);

    roomsBuffer1.push_back(firstRoom);

    for(int i = 0; i < MAP_NUMBER_ROOM_DIVISIONS; i++){
        for(int j = 0; j < pointerRoomShare->size(); j++){
            if(Random::getRandomNumber(0.0f, 1.0f) < MAP_ROOM_DIVISIONS_SKIP_PERCENT && i + 1 >= MAP_ROOM_DIVISIONS_SKIP_START){
                pointerRoomSave->push_back((*pointerRoomShare)[j]);
            }
            else if((*pointerRoomShare)[j].getWidth() > ROOM_MIN_SIZE && (*pointerRoomShare)[j].getLenght() > ROOM_MIN_SIZE){
                float proportionDivisions = Random::getRandomNumber(MAP_ROOM_DIVISIONS_PROPORTION_MIN, MAP_ROOM_DIVISIONS_PROPORTION_MAX);
                if((*pointerRoomShare)[j].getWidth() > (*pointerRoomShare)[j].getLenght()){
                    int positionCutX = round((*pointerRoomShare)[j].getPositionAngle().getX() + (*pointerRoomShare)[j].getWidth() * proportionDivisions);
                    int positionCutY = (*pointerRoomShare)[j].getPositionAngle().getY();

                    Room room1;
                    room1.setWidth(positionCutX - (*pointerRoomShare)[j].getPositionAngle().getX());
                    room1.setLenght((*pointerRoomShare)[j].getLenght());
                    room1.setPositionAngle((*pointerRoomShare)[j].getPositionAngle());
                    pointerRoomSave->push_back(room1);

                    Room room2;
                    room2.setWidth((*pointerRoomShare)[j].getWidth() - room1.getWidth() - 1);
                    room2.setLenght((*pointerRoomShare)[j].getLenght());
                    room2.setPositionAngle(Point2D(positionCutX + 1, (*pointerRoomShare)[j].getPositionAngle().getY()));
                    pointerRoomSave->push_back(room2);

                    cutRoom(Point2D(positionCutX, positionCutY), false, (*pointerRoomShare)[j].getLenght());
                }
                else{
                    int positionCutX = (*pointerRoomShare)[j].getPositionAngle().getX();
                    int positionCutY = (*pointerRoomShare)[j].getPositionAngle().getY() + (*pointerRoomShare)[j].getLenght() * proportionDivisions;

                    Room room1;
                    room1.setWidth((*pointerRoomShare)[j].getWidth());
                    room1.setLenght(positionCutY - (*pointerRoomShare)[j].getPositionAngle().getY());
                    room1.setPositionAngle((*pointerRoomShare)[j].getPositionAngle());
                    pointerRoomSave->push_back(room1);

                    Room room2;
                    room2.setWidth((*pointerRoomShare)[j].getWidth());
                    room2.setLenght((*pointerRoomShare)[j].getLenght() - room1.getLenght() - 1);
                    room2.setPositionAngle(Point2D((*pointerRoomShare)[j].getPositionAngle().getX(), positionCutY + 1));
                    pointerRoomSave->push_back(room2);

                    cutRoom(Point2D(positionCutX, positionCutY), true, (*pointerRoomShare)[j].getWidth());
                }
            }
        }
        vector<Room>* buffer = pointerRoomSave;
        pointerRoomSave = pointerRoomShare;
        pointerRoomShare = buffer;
        (*pointerRoomSave).clear();
    }

    rooms = *pointerRoomShare;
}

void Map::cutRoom(Point2D startPosition, bool horizontally, int lengthCut){
    vector<int> doorPlaces = getDoorPlaces(lengthCut);
    int counterCell = 0;

    if(horizontally){
        int x = startPosition.getX();
        int y = startPosition.getY();

        if(map[y][x-1].objectType == ObjectType::Door)
            fixingWallNearDoor(Point2D(x-1, y));
        if(map[y][x+lengthCut].objectType == ObjectType::Door)
            fixingWallNearDoor(Point2D(x+lengthCut, y));

        map[y][x-1].indestructible = true;
        map[y][x+lengthCut].indestructible = true;

        do{
            if(doorPlaces[counterCell] == 0 || map[y][x].indestructible == true){
                map[y][x].objectType = ObjectType::RectangleWall;
                map[y][x].createObject = true;
            }else
                map[y][x].objectType = ObjectType::Door;
            counterCell++;
            x++;
        }while(x < MAP_SIZE_X - 1 && map[y][x].objectType == ObjectType::None);
    }
    else{
        int x = startPosition.getX();
        int y = startPosition.getY();

        if(map[y-1][x].objectType == ObjectType::Door)
            fixingWallNearDoor(Point2D(x, y-1));
        if(map[y+lengthCut][x].objectType == ObjectType::Door)
            fixingWallNearDoor(Point2D(x, y+lengthCut));

        map[y-1][x].indestructible = true;
        map[y+lengthCut][x].indestructible = true;

        do{
            if(doorPlaces[counterCell] == 0 || map[y][x].indestructible == true){
                map[y][x].objectType = ObjectType::RectangleWall;
                map[y][x].createObject = true;
            }else
                map[y][x].objectType = ObjectType::Door;
            counterCell++;
            y++;
        }while(y < MAP_SIZE_Y - 1 && map[y][x].objectType == ObjectType::None);
    }
}

void Map::fixingWallNearDoor(const Point2D& positionDoor){
    int posDoorX = positionDoor.getX();
    int posDoorY = positionDoor.getY();

    map[posDoorY][posDoorX].createObject = true;
    map[posDoorY][posDoorX].indestructible = true;
    map[posDoorY][posDoorX].objectType = ObjectType::RectangleWall;
}


vector<int> Map::getDoorPlaces(int lengthWall){
    vector<int> doorPlaces;
    bool doorExists = false;
    for(int i = 0; i < lengthWall; i++){
        if(Random::getRandomNumber(0.0f, 1.0f) < MAP_DOOR_PERCENT && i > 2 && i < lengthWall - 3){
            doorPlaces.push_back(2);
            doorExists = true;
        }
        else
            doorPlaces.push_back(0);
    }

    if(!doorExists)
        doorPlaces[Random::getRandomNumber(2, lengthWall-2)] = 2;

    for(int i = 0; i < lengthWall; i++){
        if((i != 0 && doorPlaces[i-1] == 2) || (i != lengthWall - 1 && doorPlaces[i+1] == 2)){
            doorPlaces[i] = 1;
        }
    }

    return doorPlaces;
}

void Map::fullRooms(){
    for(int i = 0; i < rooms.size(); i++){
        rooms[i].fullRoom(map);
    }
}

void Map::generationExit(){
    vector<Point2D> cellForGeneration;
    vector<Point2D> cellForGenerationReserve;
    for(int x = 1; x < MAP_SIZE_X - 1; x++){
        if(ObjectTypeRules::isPlaceForGeneration(map[1][x].objectType)){
            cellForGeneration.push_back(Point2D(x, 0));
        }
        else if(ObjectTypeRules::isWalkable(map[1][x].objectType)){
            cellForGenerationReserve.push_back(Point2D(x, 0));
        }

        if(ObjectTypeRules::isPlaceForGeneration(map[MAP_SIZE_Y - 2][x].objectType)){
            cellForGeneration.push_back(Point2D(x, MAP_SIZE_Y - 1));
        }
        else if(ObjectTypeRules::isWalkable(map[MAP_SIZE_Y - 2][x].objectType)){
            cellForGenerationReserve.push_back(Point2D(x, MAP_SIZE_X - 1));
        }
    }

    for(int y = 1; y < MAP_SIZE_Y - 1; y++){
        if(ObjectTypeRules::isPlaceForGeneration(map[y][1].objectType)){
            cellForGeneration.push_back(Point2D(0, y));
        }
        else if(ObjectTypeRules::isWalkable(map[y][1].objectType)){
            cellForGenerationReserve.push_back(Point2D(0, y));
        }

        if(ObjectTypeRules::isPlaceForGeneration(map[y][MAP_SIZE_X - 2].objectType)){
            cellForGeneration.push_back(Point2D(MAP_SIZE_X - 1, y));
        }
        else if(ObjectTypeRules::isWalkable(map[y][MAP_SIZE_X - 2].objectType)){
            cellForGenerationReserve.push_back(Point2D(MAP_SIZE_X - 1, y));
        }
    }


    if(cellForGeneration.size() > 0){
        int idCell = Random::getRandomNumber(0, cellForGeneration.size() - 1);
        map[(int)cellForGeneration[idCell].getY()][(int)cellForGeneration[idCell].getX()].createObject = true;
        map[(int)cellForGeneration[idCell].getY()][(int)cellForGeneration[idCell].getX()].objectType = ObjectType::RectangleExit;
    }
    else if(cellForGenerationReserve.size() > 0){
        int idCell = Random::getRandomNumber(0, cellForGenerationReserve.size() - 1);
        map[(int)cellForGenerationReserve[idCell].getY()][(int)cellForGenerationReserve[idCell].getX()].createObject = true;
        map[(int)cellForGenerationReserve[idCell].getY()][(int)cellForGenerationReserve[idCell].getX()].objectType = ObjectType::RectangleExit;
    }




}

void Map::generationPlayer(){
    int k = 10000;
    vector<Point2D> cellForPlayer;

    for(int yCell = 0; yCell < MAP_SIZE_Y; yCell++){
        for(int xCell = 0; xCell < MAP_SIZE_X; xCell++){
            if(ObjectTypeRules::isPlaceForGeneration(map[yCell][xCell].objectType))
                cellForPlayer.push_back(Point2D(xCell, yCell));
        }
    }
    if(cellForPlayer.size() != 0){
        int idCell = Random::getRandomNumber(0, cellForPlayer.size() - 1);
        Point2D selectCellForPlayer = cellForPlayer[idCell];
        map[(int)selectCellForPlayer.getY()][(int)selectCellForPlayer.getX()].createObject = true;
        map[(int)selectCellForPlayer.getY()][(int)selectCellForPlayer.getX()].objectType = ObjectType::Player;
    }
    else{
        map[2][2].objectType = ObjectType::Player;
        map[2][2].createObject = true;
    }

}

void Map::generationEnemy(){
    Point2D positionPlayer;
    for(int y = 0; y < MAP_SIZE_Y; y++){
        for(int x = 0; x < MAP_SIZE_X;x++){
            if(map[y][x].objectType == ObjectType::Player)
            positionPlayer = Point2D(x, y);
        }
    }
    
    PathFinding pathFinding(map);

    Point2D positionEnemy = pathFinding.getFarWalkableCell(positionPlayer);
    map[(int)positionEnemy.getY()][(int)positionEnemy.getX()].objectType = ObjectType::Enemy;
    map[(int)positionEnemy.getY()][(int)positionEnemy.getX()].createObject = true;
}

void Map::generationKey(){
    if(KEY_COUNT == 0) return;
    
    int sizeMap = max(MAP_SIZE_X, MAP_SIZE_Y);
    int keyNearArea = sizeMap / KEY_COUNT * 2;


    vector<Point2D> addedKeys;
    vector<Point2D> cellForKey;
    vector<Point2D> cellForKeyReserve;

    for(int yCell = 0; yCell < MAP_SIZE_Y; yCell++){
        for(int xCell = 0; xCell < MAP_SIZE_X; xCell++){
            if(ObjectTypeRules::isPlaceForGeneration(map[yCell][xCell].objectType)){
                cellForKey.push_back(Point2D(xCell, yCell));
                cellForKeyReserve.push_back(Point2D(xCell, yCell));
            }
        }
    }

    while(addedKeys.size() != KEY_COUNT && cellForKey.size() + cellForKeyReserve.size() > 0){
        
        if(addedKeys.size() != 0){
            for(int idCell = cellForKey.size() - 1; idCell >= 0; idCell--){
                if((cellForKey[idCell] - addedKeys.back()).getLenghtVector() < keyNearArea){
                    cellForKey.erase(cellForKey.begin() + idCell);
                }
            }

        }


        if(cellForKey.size() != 0){
            int idCell = Random::getRandomNumber(0, cellForKey.size() - 1);
            Point2D selectCellForKey = cellForKey[idCell];

            addedKeys.push_back(selectCellForKey);
            map[(int)selectCellForKey.getY()][(int)selectCellForKey.getX()].createObject = true;
            map[(int)selectCellForKey.getY()][(int)selectCellForKey.getX()].objectType = ObjectType::Key;

            cellForKey.erase(cellForKey.begin() + idCell);

            for(int i = 0; i < cellForKeyReserve.size(); i++){
                if(cellForKeyReserve[i] == selectCellForKey)
                    cellForKeyReserve.erase(cellForKeyReserve.begin() + i);
            }

        }
        else if(cellForKeyReserve.size() != 0){
            int idCell = Random::getRandomNumber(0, cellForKeyReserve.size() - 1);
            Point2D selectCellForKey = cellForKeyReserve[idCell];

            addedKeys.push_back(selectCellForKey);
            map[(int)selectCellForKey.getY()][(int)selectCellForKey.getX()].createObject = true;
            map[(int)selectCellForKey.getY()][(int)selectCellForKey.getX()].objectType = ObjectType::Key;

            cellForKeyReserve.erase(cellForKeyReserve.begin() + idCell);

        }
        else
            return;
            
    }
}

vector<Object2D*> Map::createArrayObjects(){
    vector<Object2D*> arrayObjects;

    for(int y = 0; y < MAP_SIZE_Y; y++){
        for(int x = 0; x < MAP_SIZE_X;){
            if(map[y][x].createObject == true){
                ObjectType objectType = map[y][x].objectType;
                int startX = x;
                int sizeX = 0;
                while(x < MAP_SIZE_X && map[y][x].createObject == true && map[y][x].objectType == objectType){
                    sizeX++;
                    x++;
                }

                if(sizeX > 1 && (objectType == ObjectType::RectangleWall || objectType == ObjectType::RectangleWallAround)){
                
                    for(int i = startX; i < startX + sizeX; i++)
                        map[y][i].createObject = false;

                    Point2D position = Point2D(startX + sizeX/2.0f, y + 0.5f);

                    if(objectType == ObjectType::RectangleWall){
                        Rectangle* rectangle = new Rectangle(position * MAP_CELL_SIZE, sizeX * MAP_CELL_SIZE, 1 * MAP_CELL_SIZE, WALL_HEIGHT, WALL_COLOR, ObjectType::RectangleWall); 
                        arrayObjects.push_back(rectangle);
                    }
                    if(objectType == ObjectType::RectangleWallAround){
                        Rectangle* rectangle = new Rectangle(position * MAP_CELL_SIZE, sizeX * MAP_CELL_SIZE, 1 * MAP_CELL_SIZE, WALL_HEIGHT, WALL_COLOR, ObjectType::RectangleWallAround); 
                        arrayObjects.push_back(rectangle);
                    }
                }
            }
            else
                x++;
        }
    }
    
    for(int x = 0; x < MAP_SIZE_X; x++){
        for(int y = 0; y < MAP_SIZE_Y;){
            if(map[y][x].createObject == true){
                ObjectType objectType = map[y][x].objectType;
                int startY = y;
                int sizeY = 0;
                if(objectType == ObjectType::RectangleWall || objectType == ObjectType::RectangleWallAround){
                    while(y < MAP_SIZE_Y && map[y][x].createObject == true && map[y][x].objectType == objectType){
                        sizeY++;
                        y++;
                    }
                }
                else{
                    sizeY++;
                    y++;
                }

                for(int i = startY; i < startY + sizeY; i++)
                    map[i][x].createObject = false;

                Point2D position = Point2D(x + 0.5f, startY + sizeY/2.0f);

                if(objectType == ObjectType::RectangleWall){
                    Rectangle* rectangle = new Rectangle(position * MAP_CELL_SIZE, 1 * MAP_CELL_SIZE, sizeY * MAP_CELL_SIZE, WALL_HEIGHT, WALL_COLOR, ObjectType::RectangleWall); 
                    arrayObjects.push_back(rectangle);
                }
                if(objectType == ObjectType::RectangleWallAround){
                    Rectangle* rectangle = new Rectangle(position * MAP_CELL_SIZE, 1 * MAP_CELL_SIZE, sizeY * MAP_CELL_SIZE, WALL_HEIGHT, WALL_COLOR, ObjectType::RectangleWallAround); 
                    arrayObjects.push_back(rectangle);
                }
                if(objectType == ObjectType::Player){
                    Player* player = new Player(position * MAP_CELL_SIZE, PLAYER_RADIUS, WALL_HEIGHT, PLAYER_COLOR, ObjectType::Player);
                    this->player = player;
                    arrayObjects.push_back(player);
                }
                if(objectType == ObjectType::CircleGenerator || objectType == ObjectType::CircleWallGenerator){
                    int circleSize = 1;
                    int bias = 1;
                    

                    while(map[startY][x + bias].objectType == ObjectType::Circle || map[startY][x + bias].objectType == ObjectType::CircleWall){
                        circleSize++;
                        bias++;
                    }

                    float radius = circleSize / 2.0f;
                    position = Point2D(x + radius, startY + radius);
                    Circle* circle = new Circle(position * MAP_CELL_SIZE, radius * MAP_CELL_SIZE, WALL_HEIGHT, WALL_COLOR, ObjectType::CircleWall);
                    arrayObjects.push_back(circle);
                }
                if(objectType == ObjectType::Enemy){
                    Enemy* enemy = new Enemy(position * MAP_CELL_SIZE, ENEMY_RADIUS, ENEMY_HEIGHT, ENEMY_COLOR, ObjectType::Enemy);
                    PathFinding* pathFinding = new PathFinding(map);
                    enemy->setPathFinding(pathFinding);
                    this->enemy = enemy;
                    arrayObjects.push_back(enemy);
                }
                if(objectType == ObjectType::Key){
                    Key* key = new Key(position * MAP_CELL_SIZE, KEY_RADIUS, KEY_HEIGHT, KEY_COLOR, ObjectType::Key);
                    arrayObjects.push_back(key);
                }
                if(objectType == ObjectType::RectangleExit){
                    RectangleExit* rectangleExit = new RectangleExit(position * MAP_CELL_SIZE, 1 * MAP_CELL_SIZE, sizeY * MAP_CELL_SIZE, WALL_HEIGHT, RECTANGLE_EXIT_COLOR, ObjectType::RectangleExit);

                    bool extendAlongY = (x == 0 || x == MAP_SIZE_X - 1);
                    bool extendBefore = false;
                    bool extendAfter = false;

                    if(extendAlongY){
                        int innerX;
                        if(x == 0)
                            innerX = x + 1;
                        else
                            innerX = x - 1;

                        if(startY - 1 >= 0){
                            if(ObjectTypeRules::isWalkable(map[startY - 1][innerX].objectType))
                                extendBefore = true;
                        }

                        if(startY + 1 < MAP_SIZE_Y){
                            if(ObjectTypeRules::isWalkable(map[startY + 1][innerX].objectType))
                                extendAfter = true;
                        }
                    }
                    else{
                        int innerY;
                        if(startY == 0)
                            innerY = startY + 1;
                        else
                            innerY = startY - 1;

                        if(x - 1 >= 0){
                            if(ObjectTypeRules::isWalkable(map[innerY][x - 1].objectType))
                                extendBefore = true;
                        }

                        if(x + 1 < MAP_SIZE_X){
                            if(ObjectTypeRules::isWalkable(map[innerY][x + 1].objectType))
                                extendAfter = true;
                        }
                    }

                    rectangleExit->setSizeOnMinimap(extendAlongY, extendBefore, extendAfter);

                    arrayObjects.push_back(rectangleExit);
                }
            }
            else
                y++;
        }
    }

    enemy->setPlayer(player);

    return arrayObjects;
}

const Cell(*Map::getMap())[MAP_SIZE_X]{
    return map;
}

void Map::clearMap(){
    for(int y = 0; y < MAP_SIZE_Y; y++){
        for(int x = 0; x < MAP_SIZE_X; x++){
            map[y][x].createObject = false;
            map[y][x].indestructible = false;
            map[y][x].objectType = ObjectType::None;
        }
    }
}