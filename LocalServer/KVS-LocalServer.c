#include "KVS-LocalServer.h"
#include "Hash.h"

hashtable_t *ht;

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

// Thread of a client
void* client_thread(void * arg) {

    int local_socket = (int) arg;
    printf("new thread -> local socket %d\n", local_socket);
    char *key, *value, *action, *group, *secret;
    
    while (1)
    {    
        action = read_message(local_socket);
        if(strlen(action) == 0){
            break;
        }
        printf("action %d %s\n", strlen(action), action);
        if (strcmp(action, "connection") == 0){
            group = read_message(local_socket);
            printf("group %d %s\n", strlen(group), group);
            secret = read_message(local_socket);
            printf("secret %d %s\n", strlen(secret), secret);
            write(local_socket, "OK", 3);
        }
        if (strcmp(action, "put") == 0){
            key = read_message(local_socket);
            printf("key %d %s\n", strlen(key), key);
            value = read_message(local_socket);
            printf("value %d %s\n", strlen(value), value);
            ht_set( ht, key, value );
            printf( "fds %s\n", ht_get( ht, key ));
        }

        if (strcmp(action, "get") == 0){
            key = read_message(local_socket);
            printf("key %d %s\n", strlen(key), key);
            value = ht_get( ht, key );
            write(local_socket, "get", 4);
            write(local_socket, value, strlen(value)+1);
        }  

        if (strcmp(action, "close") == 0){
            break;
        }   
    }

    close(local_socket);
    printf("exiting thread\n");
    return NULL;
}



int main(void){

    int server_sock, client_sock, len, rc;
    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;     
    char buf[256];
    int backlog = 10;
    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(buf, 0, 256);    

    ht = ht_create(100),           

    /**************************************/
    /* Create a UNIX domain stream socket */
    /**************************************/
    server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_sock == -1){
        perror("SOCKET ERROR: ");
        exit(1);
    }
    
    /***************************************/
    /* Set up the UNIX sockaddr structure  */
    /* by using AF_UNIX for the family and */
    /* giving it a filepath to bind to.    */
    /*                                     */
    /* Unlink the file so the bind will    */
    /* succeed, then bind to that file.    */
    /***************************************/
    server_sockaddr.sun_family = AF_UNIX;   
    strcpy(server_sockaddr.sun_path, SOCK_PATH); 
    len = sizeof(server_sockaddr);
    
    unlink(SOCK_PATH);
    rc = bind(server_sock, (struct sockaddr *) &server_sockaddr, len);
    if (rc == -1){
        perror("BIND ERROR: ");
        close(server_sock);
        exit(1);
    }
    
    /*********************************/
    /* Listen for any client sockets */
    /*********************************/
    rc = listen(server_sock, backlog);
    if (rc == -1){ 
        perror("LISTEN ERROR: ");
        close(server_sock);
        exit(1);
    }
    printf("socket listening...\n");

    while(1){
    
        /*********************************/
        /* Accept an incoming connection */
        /*********************************/
        //client_sock = accept(server_sock, (struct sockaddr *) &client_sockaddr, &len);
        client_sock = accept(server_sock, NULL, NULL);
        if (client_sock == -1){
            perror("ACCEPT ERROR: ");
            close(server_sock);
            close(client_sock);
            exit(1);
        }
        
        printf("accepted one client -> new socket %d\n", client_sock);
        pthread_t t_id;
        pthread_create(&t_id, NULL, client_thread, (void *)client_sock);
        
    }
    /******************************/
    /* Close the sockets and exit */
    /******************************/

    close(client_sock);
    close(server_sock);

    return 0;
}


/*
struct DataItem {
   char* key;   
   char* value;
   struct DataItem* next_item;
};

struct Groups {
    char* group;
    char* secret;
    struct DataItem* data_item;
    struct Groups* next_group;
};

struct Groups* group1 = NULL;
struct DataItem* item1 = NULL;
group1 = (struct Groups*)malloc(sizeof(struct Groups));
item1 = (struct DataItem*)malloc(sizeof(struct DataItem));

item1->key = "chave";
item1->value = "valor";
item1->next_item = NULL;

group1->group = "group1";
group1->secret = "G1";
group1->data_item = item1;
*/