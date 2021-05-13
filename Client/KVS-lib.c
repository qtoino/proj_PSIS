
#include "applications.h"


char * read_message(int local_socket){
    char * line = malloc(sizeof(char));
    int i = 0;
    int c;
    do{
        read(local_socket, &c, 1);
        
        line[i] = c;
        i++;
        if(realloc(line, i+1) == NULL){
            perror("realloc:" );
            exit(1);
        }
        line[i] = '\0';
    }while(c != '\0');
    return line;
}


void* client_thread(void * arg) {

    int local_socket = (int) arg;
    printf("new thread -> local socket %d\n", local_socket);
    char *value, *action;
    
    while (1)
    {    
        action = read_message(local_socket);
        if(strlen(action) == 0){
            break;
        }
        printf("action %d %s\n", strlen(action), action);
        if (strcmp(action, "OK") == 0){
            printf("Connection OK\n");
        }
        if (strcmp(action, "NOK") == 0){
            printf("Connection NOK\n");
        }
        if (strcmp(action, "get") == 0){
            value = read_message(local_socket);
            printf("get value: %s\n", value);
        }
    }

    close(local_socket);
    printf("exiting thread\n");
    return NULL;
}


int establish_connection (char * group_id, char * secret){

    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    memset(&client_addr, 0, sizeof(struct sockaddr_un));

    client_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_sock == -1) {
        perror("SOCKET ERROR = ");
        return -1;
    }

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCK_PATH);
    len = sizeof(server_addr);

    if(connect(client_sock, (struct sockaddr *) &server_addr, len) == -1) {
        perror("CONNECT ERROR = ");
        close(client_sock);
        return -2;
    }
    printf("Connected\n");

    pthread_t t_id;
    pthread_create(&t_id, NULL, client_thread, client_sock);

    write(client_sock, "connection", 11);
    write(client_sock, group_id, strlen(group_id)+1);
    write(client_sock, secret, strlen(secret)+1);

    return 1;
}

int put_value(char * key, char * value) {

    write(client_sock, "put", 4);
    write(client_sock, key, strlen(key)+1);
    write(client_sock, value, strlen(value)+1);

    return 1;
}


int get_value(char * key, char ** value) {

    write(client_sock, "get", 4);
    write(client_sock, key, strlen(key)+1);   

    while (1)
    {
        sleep(1);
    }
    

    return 1;
}

// int delete_value(char * key) {
//     for (size_t i = 0; i < SIZE; i++)
//         if (hashArray[i]->key == key)
//         {
//             hashArray[i] = NULL;
//             return 1;
//         }
//     return 0;
// }


int register_callback(char * key, void (*callback_function)(char *)) {
    return 1;
}

int close_connection(){
    write(client_sock, "close", 6);
    return(1);
}
