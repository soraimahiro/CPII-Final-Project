#ifndef _CLIENT_H
#define _CLIENT_H
#include <arpa/inet.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "architecture.h"
#include "vector.h"
void init_client();
void receive(game *game_status);
void send_data(void *data, size_t size);
void destroy_client();

#endif