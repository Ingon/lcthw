#include <stdlib.h>
#include "game.h"

int Map_init(void *self)
{
    Map *map = self;

    Room *hall = NEW(Room, "The greate Hall");
    Room *throne = NEW(Room, "The throne room");
    Room *arena = NEW(Room, "The arena, with the minotaur");
    Room *kitchen = NEW(Room, "Kitchen, you have the knife now");

    arena->bad_guy = NEW(Monster, "The evil minotaur");

    hall->north = throne;

    throne->west = arena;
    throne->east = kitchen;
    throne->south = hall;

    arena->east = throne;
    kitchen->west = throne;

    map->start = hall;
    map->location = hall;

    return 1;
}

int main(int argc, char const *argv[])
{
    Map *game = NEW(Map, "The hall of the minotaur");
    return run_game(game);
}



