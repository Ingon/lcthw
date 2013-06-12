#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "game.h"

int Monster_attack(void *self, int damage)
{
    Monster *monster = self;

    printf("You attack %s!\n", monster->_(description));

    monster->hit_points -= damage;

    if(monster->hit_points > 0) {
        printf("It is still alive.\n");
        return 0;
    } else {
        printf("It is dead\n");
        return 1;
    }
}

int Monster_init(void *self)
{
    Monster *monster = self;
    monster->hit_points = 10;
    return 1;
}

Object MonsterProto = {
    .init = Monster_init,
    .attack = Monster_attack
};

void *Room_move(void *self, Direction direction)
{
    Room *room = self;
    Room *next = NULL;

    if(direction == NORTH && room->north) {
        printf("You go north into:\n");
        next = room->north;
    } else if(direction == SOUTH && room->south) {
        printf("You go south into:\n");
        next = room->south;
    } else if(direction == EAST && room->east) {
        printf("You go east into:\n");
        next = room->east;
    } else if(direction == WEST && room->west) {
        printf("You go west into:\n");
        next = room->west;
    } else {
        printf("You cannot go into this direction.\n");
        return NULL;
    }

    if(next) {
        next->_(describe)(next);
    }

    return next;
}

int Room_attack(void *self, int damage)
{
    Room *room = self;
    Monster *monster = room->bad_guy;

    if(monster) {
        // monster->_(attack)(monster, damage);
        __(monster, attack, damage);
        return 1;
    } else {
        printf("You flail in the air at nothing. Idiot.\n");
        return 0;
    }
}

Object RoomProto = {
    .move = Room_move,
    .attack = Room_attack
};

void *Map_move(void *self, Direction direction)
{
    Map *map = self;
    Room *location = map->location;
    Room *next = NULL;

    // next = location->_(move)(location, direction);
    next = __(location, move, direction);

    if(next) {
        map->location = next;
    }

    return next;
}

int Map_attack(void *self, int damage)
{
    Map *map = self;
    Room *room = map->location;

    // return room->_(attack)(room, damage);
    return __(room, attack, damage);
}

Object MapProto = {
    .init = Map_init,
    .attack = Monster_attack,
    .move = Map_move
};

int process_input(Map *game)
{
    printf("\n/> ");

    char ch = getchar();
    getchar(); // clear enter

    int damage = rand() % 4;

    switch(ch) {
        case -1:
            printf("Giving up? You kick ass.\n");
            return 0;
            break;

        case 'n':
            // game->_(move)(game, NORTH);
            __(game, move, NORTH);
            break;
        case 's':
            // game->_(move)(game, SOUTH);
            __(game, move, SOUTH);
            break;
        case 'e':
            // game->_(move)(game, EAST);
            __(game, move, EAST);
            break;
        case 'w':
            // game->_(move)(game, WEST);
            __(game, move, WEST);
            break;

        case 'a':
            // game->_(attack)(game, damage);
            __(game, attack, damage);
            break;

        case 'l':
            printf("You can go:\n");
            if(game->location->north) printf("NORTH\n");
            if(game->location->south) printf("SOUTH\n");
            if(game->location->east) printf("EAST\n");
            if(game->location->west) printf("WEST\n");
            break;

        default:
            printf("What?: %d\n", ch);
    }

    return 1;
}

int run_game(Map *game)
{
    srand(time(NULL));

    printf("You enter the ");
    game->location->_(describe)(game->location);

    while(process_input(game)) {
    }

    return 0;
}
