#ifndef KVS_LOCAL_H
#define KVS_LOCAL_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/unistd.h>
#include <pthread.h>
#include "KVS-lib.h"

#define SIZE 20
#define SOCK_PATH  "/tmp/server"

struct Key_value {
   char* key;   
   char* value;
   struct Key_value* next_item;
} Key_value;

struct Groups {
    char* group;
    char* secret;
    struct Key_value* data_item;
    struct Groups* next_group;
} Groups;

int client_sock, len;
struct sockaddr_un server_addr; 
struct sockaddr_un client_addr;

#endif