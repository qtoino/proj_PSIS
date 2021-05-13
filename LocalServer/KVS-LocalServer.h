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

#define SOCK_PATH  "/tmp/server"

typedef struct Key_value {
   char* key;   
   char* value;
   struct Key_value* next_item;
} Key_value;

#endif