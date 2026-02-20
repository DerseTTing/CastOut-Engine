#include "../headers/Map.hpp"

vector<Object2D*> Map::generationMap(){
    generationWallsAround();
    generationRooms();
    generationPlayer();

    return createArrayObjects();
}

void Map::generationWallsAround(){
    for(int y = 0; y < MAP_SIZE_Y; y++){
        for(int x = 0; x < MAP_SIZE_X; x++){
            if(y == 0 || x == 0 || y == MAP_SIZE_Y - 1 || x == MAP_SIZE_X - 1){
                map[y][x].objectType = ObjectType::RectangleWall;
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
    firstRoom.positionAngle = Point2D(1, 1);
    firstRoom.width = MAP_SIZE_X - 2;
    firstRoom.length = MAP_SIZE_Y - 2;

    roomsBuffer1.push_back(firstRoom);

    for(int i = 0; i < MAP_NUMBER_ROOM_DIVISIONS; i++){
        for(int j = 0; j < pointerRoomShare->size(); j++){
            if(Random::getRandomNumber(0.0f, 1.0f) < MAP_ROOM_DIVISIONS_SKIP_PERCENT && i + 1 >= MAP_ROOM_DIVISIONS_SKIP_START){
                pointerRoomSave->push_back((*pointerRoomShare)[j]);
            }
            else if((*pointerRoomShare)[j].width > MAP_ROOM_MIN_SIZE && (*pointerRoomShare)[j].length > MAP_ROOM_MIN_SIZE){
                float proportionDivisions = Random::getRandomNumber(MAP_ROOM_DIVISIONS_PROPORTION_MIN, MAP_ROOM_DIVISIONS_PROPORTION_MAX);
                if((*pointerRoomShare)[j].width > (*pointerRoomShare)[j].length){
                    int positionCutX = round((*pointerRoomShare)[j].positionAngle.getX() + (*pointerRoomShare)[j].width * proportionDivisions);
                    int positionCutY = (*pointerRoomShare)[j].positionAngle.getY();

                    Room room1;
                    room1.width = positionCutX - (*pointerRoomShare)[j].positionAngle.getX();
                    room1.length = (*pointerRoomShare)[j].length;
                    room1.positionAngle = (*pointerRoomShare)[j].positionAngle;
                    pointerRoomSave->push_back(room1);

                    Room room2;
                    room2.width = (*pointerRoomShare)[j].width - room1.width - 1;
                    room2.length = (*pointerRoomShare)[j].length;
                    room2.positionAngle = Point2D(positionCutX + 1, (*pointerRoomShare)[j].positionAngle.getY());
                    pointerRoomSave->push_back(room2);

                    cutRoom(Point2D(positionCutX, positionCutY), false, (*pointerRoomShare)[j].length);
                }
                else{
                    int positionCutX = (*pointerRoomShare)[j].positionAngle.getX();
                    int positionCutY = (*pointerRoomShare)[j].positionAngle.getY() + (*pointerRoomShare)[j].length * proportionDivisions;

                    Room room1;
                    room1.width = (*pointerRoomShare)[j].width;
                    room1.length = positionCutY - (*pointerRoomShare)[j].positionAngle.getY();
                    room1.positionAngle = (*pointerRoomShare)[j].positionAngle;
                    pointerRoomSave->push_back(room1);

                    Room room2;
                    room2.width = (*pointerRoomShare)[j].width;
                    room2.length = (*pointerRoomShare)[j].length - room1.length - 1;
                    room2.positionAngle = Point2D((*pointerRoomShare)[j].positionAngle.getX(), positionCutY + 1);
                    pointerRoomSave->push_back(room2);

                    cutRoom(Point2D(positionCutX, positionCutY), true, (*pointerRoomShare)[j].width);
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
            fixingWallNearDoor(Point2D(x-1, y), true);
        if(map[y][x+lengthCut].objectType == ObjectType::Door)
            fixingWallNearDoor(Point2D(x+lengthCut, y), true);

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
            fixingWallNearDoor(Point2D(x, y-1), false);
        if(map[y+lengthCut][x].objectType == ObjectType::Door)
            fixingWallNearDoor(Point2D(x, y+lengthCut), false);

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

void Map::fixingWallNearDoor(const Point2D& positionDoor, bool horizontally){
    int posDoorX = positionDoor.getX();
    int posDoorY = positionDoor.getY();
    if(horizontally){
        map[posDoorY][posDoorX].createObject = true;
        map[posDoorY][posDoorX].indestructible = true;
        map[posDoorY][posDoorX].objectType = ObjectType::RectangleWall;

        /*if(!map[posDoorY][posDoorX+1].indestructible){
            map[posDoorY][posDoorX+1].createObject = false;
            map[posDoorY][posDoorX+1].indestructible = false;
            map[posDoorY][posDoorX+1].objectType = ObjectType::Door;
        }

        if(!map[posDoorY][posDoorX-1].indestructible){
            map[posDoorY][posDoorX-1].createObject = false;
            map[posDoorY][posDoorX-1].indestructible = false;
            map[posDoorY][posDoorX-1].objectType = ObjectType::Door;
        }*/
    }
    else{
        map[posDoorY][posDoorX].createObject = true;
        map[posDoorY][posDoorX].indestructible = true;
        map[posDoorY][posDoorX].objectType = ObjectType::RectangleWall;

        /*if(!map[posDoorY+1][posDoorX].indestructible){
            map[posDoorY+1][posDoorX].createObject = false;
            map[posDoorY+1][posDoorX].indestructible = false;
            map[posDoorY+1][posDoorX].objectType = ObjectType::Door;
        }

        if(!map[posDoorY-1][posDoorX].indestructible){
            map[posDoorY-1][posDoorX].createObject = false;
            map[posDoorY-1][posDoorX].indestructible = false;
            map[posDoorY-1][posDoorX].objectType = ObjectType::Door;
        }*/
    }
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


void Map::generationPlayer(){
    map[2][2].objectType = ObjectType::Player;
    map[2][2].createObject = true;
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

                if(sizeX > 1 || objectType == ObjectType::Player) {
                
                    for(int i = startX; i < startX + sizeX; i++)
                        map[y][i].createObject = false;

                    Point2D position = Point2D(startX + sizeX/2.0f, y + 0.5f);

                    if(objectType == ObjectType::RectangleWall){
                        Rectangle* rectangle = new Rectangle(position * MAP_CELL_SIZE, sizeX * MAP_CELL_SIZE, 1 * MAP_CELL_SIZE, WALL_HEIGHT, WALL_RECTANGLE_COLOR, ObjectType::RectangleWall); 
                        arrayObjects.push_back(rectangle);
                    }
                    if(objectType == ObjectType::Player){
                        Circle* player = new Player(position * MAP_CELL_SIZE, PLAYER_RADIUS, WALL_HEIGHT, PLAYER_COLOR, ObjectType::Player);
                        arrayObjects.push_back(player);
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
                while(y < MAP_SIZE_Y && map[y][x].createObject == true && map[y][x].objectType == objectType){
                    sizeY++;
                    y++;
                }

                Point2D position = Point2D(x + 0.5f, startY + sizeY/2.0f);

                if(objectType == ObjectType::RectangleWall){
                    Rectangle* rectangle = new Rectangle(position * MAP_CELL_SIZE, 1 * MAP_CELL_SIZE, sizeY * MAP_CELL_SIZE, WALL_HEIGHT, WALL_RECTANGLE_COLOR, ObjectType::RectangleWall); 
                    arrayObjects.push_back(rectangle);
                }
                if(objectType == ObjectType::Player){
                    Circle* player = new Player(position * MAP_CELL_SIZE, PLAYER_RADIUS, WALL_HEIGHT, PLAYER_COLOR, ObjectType::Player);
                    arrayObjects.push_back(player);
                }
            }
            else
                y++;
        }
    }
    return arrayObjects;
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