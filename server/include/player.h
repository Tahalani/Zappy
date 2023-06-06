/*
** EPITECH PROJECT, 2023
** server
** File description:
** player.h
*/

#ifndef PLAYER_H_
    #define PLAYER_H_

    #define PLAYER_COMMANDS_COUNT \
        (sizeof(PLAYER_COMMANDS) / sizeof(player_command_t))

    #define FOOD_CONSUME_TICKS 12
    #define FOOD_DEFAULT 10

    #define PLAYER_UNUSED_SLOTS "Connect_nbr"
    #define PLAYER_DEATH "dead"
    #define PLAYER_UNKNOWN "ko"

    #include <stdbool.h>

    #include "types.h"

bool try_join_team(server_t *server, client_t *client, char *line);
void handle_player_command(server_t *server, client_t *client, char *line);

void food_callback(server_t *server, client_t *client);

void slots_handler(server_t *server, client_t *client);

typedef void (player_handler_t) (server_t *server, client_t *client);

typedef struct player_command {
    char const *command;
    player_handler_t *function;
} player_command_t;

static const player_command_t PLAYER_COMMANDS[] = {
        { PLAYER_UNUSED_SLOTS, &slots_handler },
};

#endif
