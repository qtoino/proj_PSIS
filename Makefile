program_LIBRARIES := pthread m
LDLIBS += $(foreach library,$(program_LIBRARIES),-l$(library))

program_FLAGS := Wall O3 pedantic
LFLAGS += $(foreach flag,$(program_FLAGS),-$(flag))

all: Client LocalServer

Client: applications.o KVS-lib.o
	gcc -o client applications.o KVS-lib.o $(LFLAGS) $(LDLIBS)

LocalServer: KVS-LocalServer.o Hash.o
	gcc -o localServer KVS-LocalServer.o Hash.o $(LFLAGS) $(LDLIBS)

# AuthServer: KVS-AuthServer.o
# 	gcc -o AuthServer KVS-AuthServer.o $(LFLAGS) $(LDLIBS)

applications.o: Client/applications.c Client/applications.h 
	gcc -c Client/applications.c $(LFLAGS) $(LDLIBS)

KVS-lib.o: Client/KVS-lib.c Client/KVS-lib.h 
	gcc -c Client/KVS-lib.c $(LFLAGS) $(LDLIBS)

KVS-LocalServer.o: LocalServer/KVS-LocalServer.c LocalServer/KVS-LocalServer.h LocalServer/Hash.h
	gcc -c LocalServer/KVS-LocalServer.c $(LFLAGS) $(LDLIBS)

Hash.o: LocalServer/Hash.c LocalServer/Hash.h
	gcc -c LocalServer/Hash.c $(LFLAGS) $(LDLIBS)

# KVS-AuthServer.o: AuthServer/KVS-AuthServer.c
# 	gcc -c AuthServer/KVS-AuthServer.c $(LFLAGS) $(LDLIBS)

clean:
	rm -f *.o *.~ applications *.~KVS-lib *.~KVS-LocalServer *.~Hash