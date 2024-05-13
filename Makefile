# Project: parking

OBJ  = sem_op.o shm_op.o aleatoire.o
LINKOBJ  = $(OBJ)
BIN  = cinema caisse
CFLAGS = -g #-Wall

all: cinema caisse

cinema: $(LINKOBJ) shm_const.h cinema.c
	$(CC) cinema.c $(LINKOBJ) -o cinema $(CFLAGS)

caisse: $(LINKOBJ) shm_const.h caisse.c
	$(CC) caisse.c $(LINKOBJ) -o caisse $(CFLAGS)

shm_op.o: shm_op.c shm_const.h
	$(CC) -c shm_op.c $(CFLAGS)

sem_op.o: sem_op.c shm_const.h
	$(CC) -c sem_op.c $(CFLAGS)

aleatoire.o: aleatoire.c
	$(CC) -c aleatoire.c $(CFLAGS)

clean: 
	rm -f $(OBJ) $(BIN)


