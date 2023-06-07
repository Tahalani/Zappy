/*
** EPITECH PROJECT, 2023
** Zappy
** File description:
** server.h
*/

#ifndef SERVER_H_
    #define SERVER_H_

    #include <bits/types/FILE.h>
    #include <stdbool.h>
    #include <sys/select.h>

    #include "types.h"

int zappy_server(int argc, char const *argv[]);
bool start_server(options_t *options);
bool init_server(server_t *server);
int refresh_fdsets(server_t *server);

bool handle_fdsets(server_t *server);
bool handle_client(server_t *server, client_t *client);

bool init_map(server_t *server);
void free_map(server_t *server);

void start_game(server_t *server);
void refresh_timeout(server_t *server);
void init_tick(server_t *server, long frequence);
bool tick(server_t *server);

task_t *register_task(server_t *server, client_t *client, \
    task_function_t *callback);
void schedule_task(task_t *task, size_t delay, int exec);
void cancel_client_tasks(server_t *server, client_t *client);
void execute_tasks(server_t *server);

void schedule_action(player_t *player, task_function_t *callback, size_t delay);
void flush_action(player_t *player);

bool check_arguments(int argc, char const *argv[], options_t *options);
bool check_number(char const *str, int *storage);

#endif
