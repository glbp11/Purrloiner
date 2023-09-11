#include <fstream>
#include "ECS\Components.h"
#include "ECS\ECS.h"
#include "Game.h"
#include "Map.h"

extern Manager manager;
std::vector<Vector2D> openSpaces;

Map::Map(std::string tID, int ms, int ts) : texID(tID), mapScale(ms), tileSize(ts) {
    scaledSize = ms * ts;
}

Map::~Map() {}

void Map::LoadMap(std::string path, int sizeX, int sizeY) {
    int srcX, srcY;
    char c[2];
    c[1] = '\0';
    std::fstream mapFile;
    mapFile.open(path);

    for (int y = 0; y < sizeY; y++) {
        for (int x = 0; x < sizeX; x++) {
            mapFile.get(c[0]);
            srcY = atoi(&c[0]) * tileSize;
            mapFile.get(c[0]);
            srcX = atoi(&c[0]) * tileSize;
            AddTile(srcX, srcY, x*scaledSize, y*scaledSize, texID);
            mapFile.ignore();
        }
    }

    mapFile.ignore();

    for(int y = 0; y < sizeY; y++) {
        for (int x = 0; x < sizeX; x++) {
            mapFile.get(c[0]);
            if (c[0] == '0') {
                openSpaces.push_back(Vector2D(x,y));
                mapFile.ignore();
                mapFile.ignore();
            } else {
                char cx[2], cy[2];
                cx[1] = cy[1] = '\0';
                mapFile.get(cx[0]);
                srcX = atoi(&cx[0]) * tileSize;
                mapFile.get(cy[0]);
                srcY = atoi(&cy[0]) * tileSize;

                auto& tcol(manager.addEntity());
                tcol.addComponent<ColliderComponent>("terrain", x*scaledSize, y*scaledSize, scaledSize);
                tcol.addGroup(Game::groupColliders);

                switch (c[0]) {
                    case '1':
                        AddTile(srcX, srcY, x*scaledSize, y*scaledSize, "objects");
                        break;
                    case '2':
                        AddTile(srcX, srcY, x*scaledSize, y*scaledSize, "fences");
                        break;
                    case '3':
                        AddTile(srcX, srcY, x*scaledSize, y*scaledSize, "house");
                        break;
                    default:
                        break;
                 }
            }
            mapFile.ignore();
        }
    }
    mapFile.close();
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos, std::string tID) {
    auto& tile(manager.addEntity());
    tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, tID);
    tile.addGroup(Game::groupMap);
}
