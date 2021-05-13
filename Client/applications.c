
#include "applications.h"

int main(void){

    if(establish_connection("group1", "secret1") != 1) {
        printf("Error establish_connection\n");
        exit(-1);
    }

    char key[] = "chave", value[] = "valor";
    if(put_value(key, value) != 1) {
        printf("Error put_value\n");
        exit(-1);
    }

    get_value(key, &value);

    return 0;
}
