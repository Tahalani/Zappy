/*
** EPITECH PROJECT, 2023
** server
** File description:
** move.c
*/

#include <sys/queue.h>

#include "constants.h"
#include "graphical.h"
#include "player.h"
#include "server.h"
#include "types.h"
#include "util.h"

static vector_t get_direction(direction_type_t direction)
{
    vector_t vector = {0, 0};

    switch (direction) {
        case NORTH:
            vector.y = -1;
            break;
        case EAST:
            vector.x = 1;
            break;
        case SOUTH:
            vector.y = 1;
            break;
        case WEST:
            vector.x = -1;
            break;
    }
    return vector;
}

void forward_callback(server_t *server, client_t *client)
{
    vector_t vector = get_direction(client->player->direction);
    int x = (client->player->pos->x + vector.x) % server->options->width;
    int y = (client->player->pos->y + vector.y) % server->options->height;
    tile_t *tile = &server->zappy->map[y][x];

    SLIST_REMOVE(&client->player->pos->players, client->player, player, \
        next_tile);
    SLIST_INSERT_HEAD(&tile->players, client->player, next_tile);
    client->player->pos = tile;
    flush_action(client->player);
    send_graphical_event(server, "%s %zu %d %d %zu%s", \
        GRAPHICAL_PLAYER_POSITION, client->player->id, x, y, \
        client->player->direction, LINE_BREAK);
    append_buffer(client->buffer, "%s%s", PLAYER_OK, LINE_BREAK);
}

void forward_handler(UNUSED server_t *server, client_t *client, \
    UNUSED char *line)
{
    schedule_action(client->player, &forward_callback, FORWARD_DELAY);
}
