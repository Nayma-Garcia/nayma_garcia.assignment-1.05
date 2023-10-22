#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <limits.h> 
#include <string.h>
#include <ncurses.h>

#define LENGTH 21
#define WIDTH 80
#define GRASS_PAIR 1
#define TREE_PAIR 2
#define WATER_PAIR 3
#define BOULDER_PAIR 4
#define PLAYER_PAIR 5
#define POKECENTER 6
#define POKEMART 7
#define PATH 8

typedef struct {
    char map[LENGTH][WIDTH];
} Map;

typedef struct {
    char terrainChar;
    int minRegions;
    int maxRegions;
    int minWidth;
    int maxWidth;
    int minHeight;
    int maxHeight;
} TerrainType;

TerrainType terrainTypes[] = {
    {':', 4, 10, 4, 13, 4, 9},  // tall grass
    {'^', 2, 6, 4, 13, 4, 9},   // trees
    {'~', 1, 5, 4, 13, 4, 9},   // water
    {'%', 1, 5, 4, 13, 4, 9},   // boulders
};

typedef struct {
    char characterChar;
    int movementCost[11];
} Trainer;

Trainer trainerTypes[] = {
    {'h', {INT_MAX, INT_MAX, 10, 10, 10, 2, 10, INT_MAX, INT_MAX, INT_MAX, 10}}, // hiker
    {'r', {INT_MAX, INT_MAX, 10, 50, 50, 15, 10, 15, 15, INT_MAX, INT_MAX}},  // rival
    {'s', {INT_MAX, INT_MAX, 10, 50, 50, 20, 10, INT_MAX, INT_MAX, INT_MAX, INT_MAX}} , //sentire
    {'w', {INT_MAX, INT_MAX, 10, 50, 50, 20, 10, INT_MAX, INT_MAX, INT_MAX, INT_MAX}} , //wanderer
    {'p', {INT_MAX, INT_MAX, 10, 50, 50, 20, 10, INT_MAX, INT_MAX, INT_MAX, INT_MAX}} , //pacer
    {'e', {INT_MAX, INT_MAX, 10, 50, 50, 20, 10, INT_MAX, INT_MAX, INT_MAX, INT_MAX}}  // explorer
};

typedef struct {
    int x;
    int y;
} Coordinate;

typedef struct {
    char symbol;
    Coordinate position;
} Hiker;

Hiker hiker;

typedef struct {
    char symbol;
    Coordinate position;
} PC;

PC pc;

typedef struct {
    char symbol;
    Coordinate position;
} Rival;

Rival rival;

typedef struct {
    char symbol;
    Coordinate position;
    int direction; 
} Pacer;

Pacer pacer;

typedef struct {
    char symbol;
    Coordinate position;
    int direction; 
} Wanderer;

Wanderer wanderer;

typedef struct {
    char symbol;
    Coordinate position;
} Sentrie;

Sentrie sentrie;

typedef struct {
    char symbol;
    Coordinate position;
    int direction; 
} Explorer;

Explorer explorer;

typedef struct {
    char symbol;
    Coordinate position;
    int direction; 
} PokeCenter;

PokeCenter pokeCenter;

typedef struct {
    char symbol;
    Coordinate position;
    int direction; 
} PokeMart;

PokeMart pokeMart;

int hikerDirection = 0; 
int rivalDirection = 0; 

int hasHikerBeenBattled = 0;
int hasRivalBeenBattled = 0;
int hasPacerBeenBattled = 0;
int hasWandererBeenBattled = 0;
int hasSentrieBeenBattled = 0;
int hasExplorerBeenBattled = 0;

void generateMap(Map *map, int numtrainer);
void printMap(Map *map);
void fillMapGrass(Map *map);
void generateBorder(Map *map);
void genPathCM(Map *map);
void generateTerrain(Map *map);
int getRandom(int min, int max);
int isPathThere(char symbol, int x, int y, int width, int height, Map *map);
void updateHikerLocation(Map *map);
void updateRivalLocation(Map *map);
void updatePacerLocation(Map *map);
void updateWandererLocation(Map *map);
void updateExplorerLocation(Map *map);
void updatePCLocation(Map *map, int move);
void openTrainerLocationList(Map *map);
void hikerPosFromPC(int hikerX, int hikerY, int pcX, int pcY);
void rivalPosFromPC(int rivalX, int rivalY, int pcX, int pcY);
void pacerPosFromPC(int pacerX, int pacerY, int pcX, int pcY);
void wandererPosFromPC(int wandererX, int wandererY, int pcX, int pcY);
void sentriePosFromPC(int sentrieX, int sentrieY, int pcX, int pcY);
void explorerPosFromPC(int explorerX, int explorerY, int pcX, int pcY);
int isPConCorM(Map *map);
void isTrainerThere();
void isHikerThere();
void isRivalThere();
void isPacerThere();
void isWandererThere();
void isSentrieThere();
void isExplorerThere();
void  clearMapAroundTrainerList();



int numtrainers = 10;

int main(int argc, char* argv[]) {

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    start_color();
    init_pair(GRASS_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(WATER_PAIR, COLOR_BLUE, COLOR_BLACK);
    init_pair(BOULDER_PAIR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(TREE_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(PLAYER_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(POKECENTER, COLOR_CYAN, COLOR_BLACK);
    init_pair(POKEMART, COLOR_CYAN, COLOR_BLACK);
    init_pair(PATH, COLOR_YELLOW, COLOR_BLACK);


    if (argc > 2) {
        printf("Usage: %s [<numtrainers>]\n", argv[0]);
        return 1;
    }

    if (argc == 2) {
        numtrainers = atoi(argv[1]);
        if (numtrainers < 0) {
            printf("Number of trainers must be non-negative.\n");
            return 1;
        }
    } else {
        numtrainers = 0;
    }

    srand(time(NULL));


    Map map;
    generateMap(&map, numtrainers);
    int move;

     printMap(&map);
     mvprintw(22, 0, "Please enter a command");

    while ((move = getch()) != 'q') {
         isTrainerThere();
        if(move == '7' || move == 'y'){
            updatePCLocation(&map, move);
            updateHikerLocation(&map);
            updateRivalLocation(&map);
            updatePacerLocation(&map);
            updateWandererLocation(&map);
            updateExplorerLocation(&map);
            fflush(stdout);
            usleep(50000);
            printMap(&map);
        }else if(move == '8' || move == 'k'){
        updatePCLocation(&map, move);
            updateHikerLocation(&map);
            updateRivalLocation(&map);
            updatePacerLocation(&map);
            updateWandererLocation(&map);
            updateExplorerLocation(&map);
            fflush(stdout);
            usleep(50000);
            printMap(&map);
       }else if(move == '9' || move == 'u'){
       updatePCLocation(&map, move);
            updateHikerLocation(&map);
            updateRivalLocation(&map);
            updatePacerLocation(&map);
            updateWandererLocation(&map);
            updateExplorerLocation(&map);
            fflush(stdout);
            usleep(50000);
            printMap(&map);
       }else if(move =='6' || move == 'l'){
        updatePCLocation(&map, move);
            updateHikerLocation(&map);
            updateRivalLocation(&map);
            updatePacerLocation(&map);
            updateWandererLocation(&map);
            updateExplorerLocation(&map);
            fflush(stdout);
            usleep(50000);
            printMap(&map);
       }else if(move =='3' || move == 'n'){
       updatePCLocation(&map, move);
            updateHikerLocation(&map);
            updateRivalLocation(&map);
            updatePacerLocation(&map);
            updateWandererLocation(&map);
            updateExplorerLocation(&map);
            fflush(stdout);
            usleep(50000);
            printMap(&map);
       }else if(move =='2' || move == 'j'){
        updatePCLocation(&map, move);
            updateHikerLocation(&map);
            updateRivalLocation(&map);
            updatePacerLocation(&map);
            updateWandererLocation(&map);
            updateExplorerLocation(&map);
            fflush(stdout);
            usleep(50000);
            printMap(&map);
       }else if(move =='1' || move == 'b'){
        updatePCLocation(&map, move);
            updateHikerLocation(&map);
            updateRivalLocation(&map);
            updatePacerLocation(&map);
            updateWandererLocation(&map);
            updateExplorerLocation(&map);
            fflush(stdout);
            usleep(50000);
            printMap(&map);
       }else if(move =='4' || move == 'h'){
            updatePCLocation(&map, move);
            updateHikerLocation(&map);
            updateRivalLocation(&map);
            updatePacerLocation(&map);
            updateWandererLocation(&map);
            updateExplorerLocation(&map);
            fflush(stdout);
            usleep(50000);
            printMap(&map);
       }else if(move == '5' || move =='.' || move == ' '){
            updatePCLocation(&map, move);
            updateHikerLocation(&map);
            updateRivalLocation(&map);
            updatePacerLocation(&map);
            updateWandererLocation(&map);
            updateExplorerLocation(&map);
            fflush(stdout);
            usleep(50000);
            printMap(&map);
       }else if(move =='t'){
          fflush(stdout);
          usleep(50000);
          printMap(&map);

          clearMapAroundTrainerList();
          //hiker
          hikerPosFromPC(hiker.position.x, hiker.position.y,pc.position.x, pc.position.y);
          //rival
          rivalPosFromPC(rival.position.x, rival.position.y,pc.position.x, pc.position.y);
          //pacer
          pacerPosFromPC(pacer.position.x, pacer.position.y,pc.position.x, pc.position.y);
          //wanderer
          wandererPosFromPC(wanderer.position.x, wanderer.position.y,pc.position.x, pc.position.y);
          //sentrie
          sentriePosFromPC(sentrie.position.x, sentrie.position.y,pc.position.x, pc.position.y);
          //explorer
          explorerPosFromPC(explorer.position.x, explorer.position.y,pc.position.x, pc.position.y);
       }else if(move == '>' && isPConCorM(&map) == 1){
         mvprintw(22, 0, "Welcome to the PokeCenter!");
       }else if (move == '>' && isPConCorM(&map) == 2){
        mvprintw(22, 0, "Welcome to the PokeMart!");
       }else if(move == '<' || move == 27){
            updatePCLocation(&map, move);
            fflush(stdout);
            usleep(500000);
            printMap(&map);
       }

      
    }

    endwin();
    return 0;
}

void generateMap(Map *map, int numtrainers) {
    fillMapGrass(map);
    generateBorder(map);
    genPathCM(map);
    generateTerrain(map);

        pc.symbol = '@';
    do {
        pc.position.x = getRandom(1, WIDTH - 2);
        pc.position.y = getRandom(1, LENGTH - 2);
    } while (map->map[pc.position.y][pc.position.x] != '#');

    for (int i = 0; i <= numtrainers; i++){
        hiker.symbol = 'h';
    do {
        hiker.position.x = getRandom(1, WIDTH - 2);
        hiker.position.y = getRandom(1, LENGTH - 2);
    } while (map->map[hiker.position.y][hiker.position.x] == '#');

    rival.symbol = 'r';
    do {
        rival.position.x = getRandom(1, WIDTH - 2);
        rival.position.y = getRandom(1, LENGTH - 2);
    } while (map->map[rival.position.y][rival.position.x] == '#' ||
             (rival.position.x == hiker.position.x && rival.position.y == hiker.position.y) ||
             (rival.position.x == pc.position.x && rival.position.y == pc.position.y));

    pacer.symbol = 'p';
    do {
        pacer.position.x = getRandom(1, WIDTH - 2);
        pacer.position.y = getRandom(1, LENGTH - 2);
    } while (map->map[pacer.position.y][pacer.position.x] == '#' ||
             (pacer.position.x == hiker.position.x && pacer.position.y == hiker.position.y) ||
             (pacer.position.x == pc.position.x && pacer.position.y == pc.position.y) ||
             (pacer.position.x == rival.position.x && pacer.position.y == rival.position.y));

    pacer.direction = 0; 

   wanderer.symbol = 'w';
    do {
        wanderer.position.x = getRandom(1, WIDTH - 2);
        wanderer.position.y = getRandom(1, LENGTH - 2);
    } while ((map->map[wanderer.position.y][wanderer.position.x] != ':') || 
             (wanderer.position.x == hiker.position.x && wanderer.position.y == hiker.position.y) ||
             (wanderer.position.x == pc.position.x && wanderer.position.y == pc.position.y) ||
             (wanderer.position.x == rival.position.x && wanderer.position.y == rival.position.y) ||
             (wanderer.position.x == pacer.position.x && wanderer.position.y == pacer.position.y));

    wanderer.direction = getRandom(0, 3);

 sentrie.symbol = 's'; 

do {
    sentrie.position.x = getRandom(1, WIDTH - 2);
    sentrie.position.y = getRandom(1, LENGTH - 2);
} while (map->map[sentrie.position.y][sentrie.position.x] != '.');
map->map[sentrie.position.y][sentrie.position.x] = sentrie.symbol;

explorer.symbol = 'e';
do {
    explorer.position.x = getRandom(1, WIDTH - 2);
    explorer.position.y = getRandom(1, LENGTH - 2);
} while ((map->map[explorer.position.y][explorer.position.x] != ':') || 
         (explorer.position.x == hiker.position.x && explorer.position.y == hiker.position.y) ||
         (explorer.position.x == pc.position.x && explorer.position.y == pc.position.y) ||
         (explorer.position.x == rival.position.x && explorer.position.y == rival.position.y) ||
         (explorer.position.x == pacer.position.x && explorer.position.y == pacer.position.y) ||
         (explorer.position.x == wanderer.position.x && explorer.position.y == wanderer.position.y) ||
         (explorer.position.x == sentrie.position.x && explorer.position.y == sentrie.position.y));

explorer.direction = getRandom(0, 3);
    }
    
}


void printMap(Map *map) {
    int x;
    int y;
    clear();
    for (y = 0; y < LENGTH; y++) {
        for (x = 0; x < WIDTH; x++) {
            if (x == sentrie.position.x && y == sentrie.position.y) {
                mvaddch(y, x, sentrie.symbol);
            } else if (x == hiker.position.x && y == hiker.position.y) {
                mvaddch(y,x, hiker.symbol);
            } else if (x == pc.position.x && y == pc.position.y) {
                attron(COLOR_PAIR(PLAYER_PAIR));
                 mvaddch(y,x, pc.symbol);
                attroff(COLOR_PAIR(PLAYER_PAIR));
            } else if (x == rival.position.x && y == rival.position.y) {
                 mvaddch(y,x, rival.symbol);
            } else if (x == pacer.position.x && y == pacer.position.y) {
                 mvaddch(y,x, pacer.symbol);
            } else if (x == wanderer.position.x && y == wanderer.position.y) {
                 mvaddch(y,x, wanderer.symbol);
            } else if (x == explorer.position.x && y == explorer.position.y) {  
                 mvaddch(y,x, explorer.symbol);
            } else {
                 mvaddch(y,x, map->map[y][x]);
            }
        }
        printf("\n");
    }
    refresh();
}


void fillMapGrass(Map *map) {
    int x, y;
    for (y = 0; y < LENGTH; y++) {
        for (x = 0; x < WIDTH; x++) {
            map->map[y][x] = '.';
        }
    }
}

void generateBorder(Map *map) {
    int x, y;
    for (x = 0; x < WIDTH; x++) {
        map->map[0][x] = '%'; 
        map->map[LENGTH - 1][x] = '%';
    }
    for (y = 0; y < LENGTH; y++) {
        map->map[y][0] = '%';
        map->map[y][WIDTH - 1] = '%';
    }
}

void genPathCM(Map *map) {
    int x, y;
    // Making the path that goes from east to west
    y = rand() % (LENGTH - 3) + 1;  // So it doesn't touch the top border
    for (x = 0; x < WIDTH; x++) {
        map->map[y][x] = '#';
    }

    x = rand() % (WIDTH - 3) + 1;  
    if (y < LENGTH - 1 && x < WIDTH - 1) {
        pokeCenter.symbol = 'C';
        pokeCenter.position.x = x;
        pokeCenter.position.y = y + 1;
    } else {
        pokeCenter.symbol = 'C';
        pokeCenter.position.x = x;
        pokeCenter.position.y = y - 1;
    }

    if (y < LENGTH - 1 && x < WIDTH - 1) {
        pokeMart.symbol = 'M';
        pokeMart.position.x = x + 1;
        pokeMart.position.y = y + 1;
    } else if (map->map[y][x] == '#') {
        pokeMart.symbol = 'M';
        pokeMart.position.x = x + 1;
        pokeMart.position.y = y;
    } else {
        pokeMart.symbol = 'M';
        pokeMart.position.x = x - 1;
        pokeMart.position.y = y - 1;
    }

    map->map[pokeCenter.position.y][pokeCenter.position.x] = pokeCenter.symbol;
    map->map[pokeMart.position.y][pokeMart.position.x] = pokeMart.symbol;

    x = rand() % (WIDTH - 3); 
    for (y = 0; y < LENGTH; y++) {
        map->map[y][x] = '#';
    }
}



void generateTerrain(Map *map) {
    int numTerrainTypes = sizeof(terrainTypes) / sizeof(terrainTypes[0]);

    for (int i = 0; i < numTerrainTypes; i++) {
        TerrainType terrainType = terrainTypes[i];
        int x, y;

        do {
            x = getRandom(1, WIDTH - terrainType.maxWidth - 1);
            y = getRandom(1, LENGTH - terrainType.maxHeight - 1);
        } while (isPathThere('#', x, y, terrainType.maxWidth, terrainType.maxHeight, map));

        for (int ty = y; ty < y + terrainType.maxHeight; ty++) {
            for (int tx = x; tx < x + terrainType.maxWidth; tx++) {
                map->map[ty][tx] = terrainType.terrainChar;
            }
        }
    }
    refresh();
}

int getRandom(int min, int max) {
    return min + rand() % (max - min + 1);
}

int isPathThere(char symbol, int x, int y, int width, int height, Map *map) {
    for (int ty = y; ty < y + height; ty++) {
        for (int tx = x; tx < x + width; tx++) {
            if (map->map[ty][tx] == symbol) {
                return 1;
            }
        }
    }
    return 0;
}

void updateHikerLocation(Map *map) {
    // Calculate the direction to move towards the PC
    int dx = pc.position.x - hiker.position.x;
    int dy = pc.position.y - hiker.position.y;

    int newX = hiker.position.x;
    int newY = hiker.position.y;

    if (dx > 0 && map->map[hiker.position.y][hiker.position.x + 1] != '@' && dx > 0 && map->map[hiker.position.y][hiker.position.x + 1] != '%') {
        newX++;
    } else if (dx < 0 && map->map[hiker.position.y][hiker.position.x - 1] != '@' && dx < 0 && map->map[hiker.position.y][hiker.position.x - 1] != '%') {
        newX--;
    } else if (dy > 0 && map->map[hiker.position.y + 1][hiker.position.x] != '@' && dy > 0 && map->map[hiker.position.y + 1][hiker.position.x] != '%') {
        newY++;
    } else if (dy < 0 && map->map[hiker.position.y - 1][hiker.position.x] != '@' && dy < 0 && map->map[hiker.position.y - 1][hiker.position.x] != '%') {
        newY--;
    }

    if (newX == pc.position.x && newY == pc.position.y) {
        int alternateDirection = rand() % 4;
        switch (alternateDirection) {
            case 0:
                if (map->map[hiker.position.y][hiker.position.x + 1] != '@' && map->map[hiker.position.y][hiker.position.x + 1] != '%') {
                    newX++;
                }
                break;
            case 1:
                if (map->map[hiker.position.y][hiker.position.x - 1] != '@' && map->map[hiker.position.y][hiker.position.x - 1] != '%') {
                    newX--;
                }
                break;
            case 2:
                if (map->map[hiker.position.y + 1][hiker.position.x] != '@' && map->map[hiker.position.y + 1][hiker.position.x] != '%') {
                    newY++;
                }
                break;
            case 3:
                if (map->map[hiker.position.y - 1][hiker.position.x] != '@' && map->map[hiker.position.y - 1][hiker.position.x] != '%') {
                    newY--;
                }
                break;
        }
    }

    // Ensure the new position is within the map boundaries
    if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < LENGTH) {
        hiker.position.x = newX;
        hiker.position.y = newY;
    }
}

void updateRivalLocation(Map *map) {
    int dx = pc.position.x - rival.position.x;
    int dy = pc.position.y - rival.position.y;

    int newX = rival.position.x;
    int newY = rival.position.y;

    if (dx > 0 && map->map[rival.position.y][rival.position.x + 1] != '@' && map->map[rival.position.y][rival.position.x + 1] != '%' &&
        map->map[rival.position.y][rival.position.x + 1] != 'h' && map->map[rival.position.y][rival.position.x + 1] != 'p' &&
        map->map[rival.position.y][rival.position.x + 1] != 'w' && map->map[rival.position.y][rival.position.x + 1] != 's' &&
        map->map[rival.position.y][rival.position.x + 1] != 'r') {
        newX++;
    } else if (dx < 0 && map->map[rival.position.y][rival.position.x - 1] != '@' && map->map[rival.position.y][rival.position.x - 1] != '%' &&
               map->map[rival.position.y][rival.position.x - 1] != 'h' && map->map[rival.position.y][rival.position.x - 1] != 'p' &&
               map->map[rival.position.y][rival.position.x - 1] != 'w' && map->map[rival.position.y][rival.position.x - 1] != 's' &&
               map->map[rival.position.y][rival.position.x - 1] != 'r') {
        newX--;
    } else if (dy > 0 && map->map[rival.position.y + 1][rival.position.x] != '@' && map->map[rival.position.y + 1][rival.position.x] != '%' &&
               map->map[rival.position.y + 1][rival.position.x] != 'h' && map->map[rival.position.y + 1][rival.position.x] != 'p' &&
               map->map[rival.position.y + 1][rival.position.x] != 'w' && map->map[rival.position.y + 1][rival.position.x] != 's' &&
               map->map[rival.position.y + 1][rival.position.x] != 'r') {
        newY++;
    } else if (dy < 0 && map->map[rival.position.y - 1][rival.position.x] != '@' && map->map[rival.position.y - 1][rival.position.x] != '%' &&
               map->map[rival.position.y - 1][rival.position.x] != 'h' && map->map[rival.position.y - 1][rival.position.x] != 'p' &&
               map->map[rival.position.y - 1][rival.position.x] != 'w' && map->map[rival.position.y - 1][rival.position.x] != 's' &&
               map->map[rival.position.y - 1][rival.position.x] != 'r') {
        newY--;
    }

    if (newX == pc.position.x && newY == pc.position.y) {
        int alternateDirection = rand() % 4;
        switch (alternateDirection) {
            case 0:
                if (map->map[rival.position.y][rival.position.x + 1] != '@' && map->map[rival.position.y][rival.position.x + 1] != '%' &&
                    map->map[rival.position.y][rival.position.x + 1] != 'h' && map->map[rival.position.y][rival.position.x + 1] != 'p' &&
                    map->map[rival.position.y][rival.position.x + 1] != 'w' && map->map[rival.position.y][rival.position.x + 1] != 's' &&
                    map->map[rival.position.y][rival.position.x + 1] != 'r') {
                    newX++;
                }
                break;
            case 1:
                if (map->map[rival.position.y][rival.position.x - 1] != '@' && map->map[rival.position.y][rival.position.x - 1] != '%' &&
                    map->map[rival.position.y][rival.position.x - 1] != 'h' && map->map[rival.position.y][rival.position.x - 1] != 'p' &&
                    map->map[rival.position.y][rival.position.x - 1] != 'w' && map->map[rival.position.y][rival.position.x - 1] != 's' &&
                    map->map[rival.position.y][rival.position.x - 1] != 'r') {
                    newX--;
                }
                break;
            case 2:
                if (map->map[rival.position.y + 1][rival.position.x] != '@' && map->map[rival.position.y + 1][rival.position.x] != '%' &&
                    map->map[rival.position.y + 1][rival.position.x] != 'h' && map->map[rival.position.y + 1][rival.position.x] != 'p' &&
                    map->map[rival.position.y + 1][rival.position.x] != 'w' && map->map[rival.position.y + 1][rival.position.x] != 's' &&
                    map->map[rival.position.y + 1][rival.position.x] != 'r') {
                    newY++;
                }
                break;
            case 3:
                if (map->map[rival.position.y - 1][rival.position.x] != '@' && map->map[rival.position.y - 1][rival.position.x] != '%' &&
                    map->map[rival.position.y - 1][rival.position.x] != 'h' && map->map[rival.position.y - 1][rival.position.x] != 'p' &&
                    map->map[rival.position.y - 1][rival.position.x] != 'w' && map->map[rival.position.y - 1][rival.position.x] != 's' &&
                    map->map[rival.position.y - 1][rival.position.x] != 'r') {
                    newY--;
                }
                break;
        }
    }

    if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < LENGTH) {
        rival.position.x = newX;
        rival.position.y = newY;
    }
}




void updatePacerLocation(Map *map) {
    int newX = pacer.position.x;
    int newY = pacer.position.y;

    if (pacer.direction == 0) { 
        newX++;
    } else if (pacer.direction == 1) { 
        newX--;
    } else if (pacer.direction == 2) { 
        newY--;
    } else if (pacer.direction == 3) { 
        newY++;
    }

    char nextTerrain = map->map[newY][newX];

    if (!(newX >= WIDTH - 1 || newX <= 0 || newY >= LENGTH - 1 || newY <= 0) &&
        (nextTerrain != '%' && nextTerrain != '^' && nextTerrain != '~')) {
        pacer.position.x = newX;
        pacer.position.y = newY;
    } else {
        if (pacer.direction == 0) {
            pacer.direction = 1; 
        } else if (pacer.direction == 1) {
            pacer.direction = 0; 
        } else if (pacer.direction == 2) {
            pacer.direction = 3;
        } else if (pacer.direction == 3) {
            pacer.direction = 2; 
        }
    }
}




void updateWandererLocation(Map *map) {
    int newX = wanderer.position.x;
    int newY = wanderer.position.y;

    if (!(newX + 1 >= WIDTH - 1 || newX - 1 <= 0 || newY + 1 >= LENGTH - 1 || newY - 1 <= 0)) {
        char currentTerrain = map->map[newY][newX];
        char nextTerrain = currentTerrain;

        if (wanderer.direction == 0) {
            nextTerrain = map->map[newY][newX + 1];
        } else if (wanderer.direction == 1) { 
            nextTerrain = map->map[newY][newX - 1];
        } else if (wanderer.direction == 2) { 
            nextTerrain = map->map[newY - 1][newX];
        } else if (wanderer.direction == 3) { 
            nextTerrain = map->map[newY + 1][newX];
        }

        if (nextTerrain != currentTerrain) {
            wanderer.direction = rand() % 4;
        } else {
            if (wanderer.direction == 0) { 
                newX++;
            } else if (wanderer.direction == 1) { 
                newX--;
            } else if (wanderer.direction == 2) { 
                newY--;
            } else if (wanderer.direction == 3) { 
                newY++;
            }
        }
    } else {
        wanderer.direction = rand() % 4;
    }

    wanderer.position.x = newX;
    wanderer.position.y = newY;
}

void updateExplorerLocation(Map *map) {
    int newX, newY;

    do {
        newX = explorer.position.x;
        newY = explorer.position.y;

        int randomDirection = rand() % 8;

        switch (randomDirection) {
            case 0: 
                newX++;
                break;
            case 1: 
                newX--;
                break;
            case 2: 
                newY--;
                break;
            case 3: 
                newY++;
                break;
            case 4: 
                newX++;
                newY--;
                break;
            case 5: 
                newX--;
                newY--;
                break;
            case 6: 
                newX++;
                newY++;
                break;
            case 7: 
                newX--;
                newY++;
                break;
            default:
                break;
        }

        if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < LENGTH) {
            char nextTerrain = map->map[newY][newX];

            if (nextTerrain != '&' && nextTerrain != '^' && nextTerrain != '~' &&
                nextTerrain != 'h' && nextTerrain != 'r' && nextTerrain != 'p' &&
                nextTerrain != 'w' && nextTerrain != 's' && nextTerrain != '%') {
                explorer.position.x = newX;
                explorer.position.y = newY;
            }
        } else {
            explorer.direction = rand() % 8;  
        }
    } while (map->map[explorer.position.y][explorer.position.x] == '&' || 
             map->map[explorer.position.y][explorer.position.x] == '^' || 
             map->map[explorer.position.y][explorer.position.x] == '~' ||
             map->map[explorer.position.y][explorer.position.x] == 'h' || 
             map->map[explorer.position.y][explorer.position.x] == 'r' || 
             map->map[explorer.position.y][explorer.position.x] == 'p' || 
             map->map[explorer.position.y][explorer.position.x] == 'w' ||
             map->map[explorer.position.y][explorer.position.x] == 's' ||
             map->map[explorer.position.y][explorer.position.x] == '%');
}

void updatePCLocation(Map *map, int move){
    int newX, newY;

    newX = pc.position.x;
    newY = pc.position.y;

       if(move == '7' || move == 'y'){
        newX--;
        newY--;
       }else if(move == '8' || move == 'k'){
        newY--;
       }else if(move == '9' || move == 'u'){
        newX++;
        newY--;
       }else if(move =='6' || move == 'l'){
        newX++;
       }else if(move =='3' || move == 'n'){
        newX++;
        newY++;
       }else if(move =='2' || move == 'j'){
        newY++;
       }else if(move =='1' || move == 'b'){
        newX--;
        newY++;
       }else if(move =='4' || move == 'h'){
        newX--;
       }else if(move == '5' || move == '.' || move == ' '){
        newX = pc.position.x;
        newY = pc.position.y;
       }else if(move == '<' || move == 27){
        newX = pc.position.x;
        newY = pc.position.y;
       }

   if (newX >= 0 && newX < WIDTH - 1 && newY >= 0 && newY < LENGTH - 1) {
        pc.position.x = newX;
        pc.position.y = newY;
    }

}

void hikerPosFromPC(int hikerX, int hikerY, int pcX, int pcY) {
    int north = pcY - hikerY;
    int south = hikerY - pcY;
    int east = hikerX - pcX;
    int west = pcX - hikerX;

    char directionStr[100]; 

    if (north > 0) {
        sprintf(directionStr, "%d north", north);
    } else if (south > 0) {
        sprintf(directionStr, "%d south", south);
    } 

    if (west > 0) {
        sprintf(directionStr + strlen(directionStr), " by %d west", west);
    } else if (east > 0) {
        sprintf(directionStr + strlen(directionStr), " by %d east", east);
    } 

    mvprintw(4, 25, "hiker: %s", directionStr);
    refresh(); 
}

void rivalPosFromPC(int rivalX, int rivalY, int pcX, int pcY) {
    int south = rivalY - pcY;
    int north = pcY - rivalY;
    int east = rivalX - pcX;
    int west = pcX - rivalX;

    char directionStr[100]; 

    if (north > 0) {
        sprintf(directionStr, "%d north", north);
    } else if (south > 0) {
        sprintf(directionStr, "%d south", south);
    } 

    if (west > 0) {
        sprintf(directionStr + strlen(directionStr), " by %d west", west);
    } else if (east > 0) {
        sprintf(directionStr + strlen(directionStr), " by %d east", east);
    } 

    mvprintw(5, 25, "rival: %s", directionStr);
    refresh(); 
}

void pacerPosFromPC(int pacerX, int pacerY, int pcX, int pcY) {
    int south = pacerY - pcY;
    int north = pcY - pacerY;
    int east = pacerX - pcX;
    int west = pcX - pacerX;

    char directionStr[100]; 

    if (north > 0) {
        sprintf(directionStr, "%d north", north);
    } else if (south > 0) {
        sprintf(directionStr, "%d south", south);
    } 

    if (west > 0) {
        sprintf(directionStr + strlen(directionStr), " by %d west", west);
    } else if (east > 0) {
        sprintf(directionStr + strlen(directionStr), " by %d east", east);
    } 

    mvprintw(6, 25, "pacer: %s", directionStr);
    refresh(); 
}

void wandererPosFromPC(int wandererX, int wandererY, int pcX, int pcY) {
    int south = wandererY - pcY;
    int north = pcY - wandererY;
    int east = wandererX - pcX;
    int west = pcX - wandererX;

    char directionStr[100]; 

    if (north > 0) {
        sprintf(directionStr, "%d north", north);
    } else if (south > 0) {
        sprintf(directionStr, "%d south", south);
    } 

    if (west > 0) {
        sprintf(directionStr + strlen(directionStr), " by %d west", west);
    } else if (east > 0) {
        sprintf(directionStr + strlen(directionStr), " by %d east", east);
    } 

    mvprintw(7, 25, "wanderer: %s", directionStr);
    refresh(); 
}

void sentriePosFromPC(int sentrieX, int sentrieY, int pcX, int pcY) {
    int south = sentrieY - pcY;
    int north = pcY - sentrieY;
    int east = sentrieX - pcX;
    int west = pcX - sentrieX;

    char directionStr[100]; 

    if (north > 0) {
        sprintf(directionStr, "%d north", north);
    } else if (south > 0) {
        sprintf(directionStr, "%d south", south);
    } 

    if (west > 0) {
        sprintf(directionStr + strlen(directionStr), " by %d west", west);
    } else if (east > 0) {
        sprintf(directionStr + strlen(directionStr), " by %d east", east);
    } 

    mvprintw(8, 25, "sentrie: %s", directionStr);
    refresh(); 
}

void explorerPosFromPC(int explorerX, int explorerY, int pcX, int pcY) {
    int south = explorerY - pcY;
    int north = pcY - explorerY;
    int east = explorerX - pcX;
    int west = pcX - explorerX;

    char directionStr[100]; 

    if (north > 0) {
        sprintf(directionStr, "%d north", north);
    } else if (south > 0) {
        sprintf(directionStr, "%d south", south);
    } 

    if (west > 0) {
        sprintf(directionStr + strlen(directionStr), " by %d west", west);
    } else if (east > 0) {
        sprintf(directionStr + strlen(directionStr), " by %d east", east);
    } 

    mvprintw(9, 25, "explorer: %s", directionStr);
    refresh(); 
}

int isPConCorM(Map *map) {
    if (pc.position.x == pokeCenter.position.x && pc.position.y == pokeCenter.position.y) {
        return 1; 
    }

    if(pc.position.x == pokeMart.position.x && pc.position.y == pokeMart.position.y){
        return 2;
    }
    return 0; 
}

void isTrainerThere() {
 if(hasHikerBeenBattled == 0){
    isHikerThere();
 } 
  
  if(hasRivalBeenBattled == 0){
    isRivalThere();
 }
 if(hasPacerBeenBattled == 0){
    isPacerThere();;
 }
 if(hasWandererBeenBattled == 0){
    isWandererThere();
 }
 if(hasSentrieBeenBattled == 0){
    isSentrieThere();
 }
 if(hasExplorerBeenBattled == 0){
    isExplorerThere();
 }
  
}

void isHikerThere(){
    int pcX = pc.position.x;
    int pcY = pc.position.y;
    int hikerX = hiker.position.x;
    int hikerY = hiker.position.y;
    int printBattle = 0;
     if(pcX == hikerX && pcY == hikerY){
     printBattle = 1;
   }

   if(pcX-1 == hikerX && pcY == hikerY){
     printBattle = 1;
   }

   if(pcX+1 == hikerX &&  pcY == hikerY){
     printBattle = 1;
   }

   if(pcY-1 == hikerY && pcX == hikerX){
     printBattle = 1;
   }

   if(pcY+1 == hikerY && pcX == hikerX){
     printBattle = 1;
   }

    if(printBattle == 1){
        hasHikerBeenBattled = 1;
        mvprintw(22, 0, "You have entered a battle with a hiker!");
    }

}

void isRivalThere(){
    int pcX = pc.position.x;
    int pcY = pc.position.y;
    int rivalX = rival.position.x;
    int rivalY = rival.position.y;
       int printBattle = 0;
     if(pcX == rivalX && pcY == rivalY){
     printBattle = 1;
   }

   if(pcX-1 == rivalX && pcY == rivalY){
    printBattle = 1;
   }

   if(pcX+1 == rivalX &&  pcY == rivalY){
    printBattle = 1;
   }

   if(pcY-1 == rivalY && pcX == rivalX){
    printBattle = 1;
   }

   if(pcY+1 == rivalY && pcX == rivalX){
    printBattle = 1;
   }

     if(printBattle == 1){
        hasRivalBeenBattled = 1;
        mvprintw(22, 0, "You have entered a battle with a rival!");
    }

}

void isPacerThere(){
    int pcX = pc.position.x;
    int pcY = pc.position.y;
    int pacerX = pacer.position.x;
    int pacerY = pacer.position.y;
    int printBattle = 0;
     if(pcX == pacerX && pcY == pacerY){
     printBattle = 1;
   }

   if(pcX-1 == pacerX && pcY == pacerY){
    printBattle = 1;
   }

   if(pcX+1 == pacerX &&  pcY == pacerY){
    printBattle = 1;
   }

   if(pcY-1 == pacerY && pcX == pacerX){
    printBattle = 1;
   }

   if(pcY+1 == pacerY && pcX == pacerX){
    printBattle = 1;
   }

      if(printBattle == 1){
        hasPacerBeenBattled = 1;
        mvprintw(22, 0, "You have entered a battle with a pacer!");
    }

}

void isWandererThere(){
    int pcX = pc.position.x;
    int pcY = pc.position.y;
    int wandererX = wanderer.position.x;
    int wandererY = wanderer.position.y;
    int printBattle = 0;
     if(pcX == wandererX && pcY == wandererY){
     printBattle = 1;
   }

   if(pcX-1 == wandererX && pcY == wandererY){
     printBattle = 1;
   }

   if(pcX+1 == wandererX &&  pcY == wandererY){
     printBattle = 1;
   }

   if(pcY-1 == wandererY && pcX == wandererX){
     printBattle = 1;
   }

   if(pcY+1 == wandererY && pcX == wandererX){
     printBattle = 1;
   }

    if(printBattle == 1){
        hasWandererBeenBattled = 1;
        mvprintw(22, 0, "You have entered a battle with a wanderer!");
    }

}

void isSentrieThere(){
    int pcX = pc.position.x;
    int pcY = pc.position.y;
    int sentrieX = sentrie.position.x;
    int sentrieY = sentrie.position.y;
    int printBattle = 0;
     if(pcX == sentrieX && pcY == sentrieY){
     printBattle = 1;
   }

   if(pcX-1 == sentrieX && pcY == sentrieY){
    printBattle = 1;
   }

   if(pcX+1 == sentrieX &&  pcY == sentrieY){
    printBattle = 1;
   }

   if(pcY-1 == sentrieY && pcX == sentrieX){
    printBattle = 1;
   }

   if(pcY+1 == sentrieY && pcX == sentrieX){
    printBattle = 1;
   }

      if(printBattle == 1){
        hasSentrieBeenBattled = 1;
        mvprintw(22, 0, "You have entered a battle with a sentrie!");
    }

}

void isExplorerThere(){
    int pcX = pc.position.x;
    int pcY = pc.position.y;
    int explorerX = explorer.position.x;
    int explorerY = explorer.position.y;
    int printBattle = 0;
     if(pcX == explorerX && pcY == explorerY){
     printBattle = 1;
   }

   if(pcX-1 == explorerX && pcY == explorerY){
    printBattle = 1;
   }

   if(pcX+1 == explorerX &&  pcY == explorerY){
    printBattle = 1;
   }

   if(pcY-1 == explorerY && pcX == explorerX){
    printBattle = 1;
   }

   if(pcY+1 == explorerY && pcX == explorerX){
    printBattle = 1;
   }

       if(printBattle == 1){
        hasExplorerBeenBattled = 1;
        mvprintw(22, 0, "You have entered a battle with an Explorer!");
    }

}

void clearMapAroundTrainerList() {
    int startY = 3;  
    int endY = 10;   
    int startX = 22;  
    int endX = 55;    

    for (int y = startY; y <= endY; y++) {
        for (int x = startX; x <= endX; x++) {
            mvprintw(y, x, " "); 
        }
    }
}

