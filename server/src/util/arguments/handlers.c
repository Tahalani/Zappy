/*
** EPITECH PROJECT, 2023
** server
** File description:
** handlers.c
*/

#include <stdio.h>

#include "args.h"
#include "constants.h"
#include "types.h"

int port_handler(int argc, char const *argv[], options_t *options, int idx)
{
    if (argc <= idx + 1) {
        fprintf(stderr, "Error: %s requires an argument\n", argv[idx]);
        return -1;
    }
    if (options->port != 0) {
        fprintf(stderr, "Error: Port already set\n");
        return -1;
    }
    if (!check_port(argv[idx + 1], &options->port)) {
        return -1;
    }
    return 1;
}

int width_handler(int argc, char const *argv[], options_t *options, int idx)
{
    if (argc <= idx + 1) {
        fprintf(stderr, "Error: %s requires an argument\n", argv[idx]);
        return -1;
    }
    if (options->width != 0) {
        fprintf(stderr, "Error: Width already set\n");
        return -1;
    }
    if (!check_positive(argv[idx + 1], "Width", &options->width)) {
        return -1;
    }
    return 1;
}

int height_handler(int argc, char const *argv[], options_t *options, int idx)
{
    if (argc <= idx + 1) {
        fprintf(stderr, "Error: %s requires an argument\n", argv[idx]);
        return -1;
    }
    if (options->height != 0) {
        fprintf(stderr, "Error: Height already set\n");
        return -1;
    }
    if (!check_positive(argv[idx + 1], "Height", &options->height)) {
        return -1;
    }
    return 1;
}

int clients_handler(int argc, char const *argv[], options_t *options, int idx)
{
    if (argc <= idx + 1) {
        fprintf(stderr, "Error: %s requires an argument\n", argv[idx]);
        return -1;
    }
    if (options->clients != 0) {
        fprintf(stderr, "Error: Clients number already set\n");
        return -1;
    }
    if (!check_positive(argv[idx + 1], "Clients", &options->clients)) {
        return -1;
    }
    return 1;
}

int freq_handler(int argc, char const *argv[], options_t *options, int idx)
{
    if (argc <= idx + 1) {
        fprintf(stderr, "Error: %s requires an argument\n", argv[idx]);
        return -1;
    }
    if (options->freq != 0) {
        fprintf(stderr, "Error: Frequency already set\n");
        return -1;
    }
    if (!check_positive(argv[idx + 1], "Frequency", &options->freq)) {
        return -1;
    }
    if (options->freq > MAX_FREQ) {
        fprintf(stderr, "Error: Frequency cannot be greater than %d\n", \
            MAX_FREQ);
        return -1;
    }
    return 1;
}
