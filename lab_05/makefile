CFLAGS := -std=c99 -pedantic -Wall -Werror -Wpedantic -Wextra -ggdb
CC := gcc
OBJ_PATH := out/
INC_PATH := inc/
SRC_PATH := src/

app.exe: $(OBJ_PATH)process.o $(OBJ_PATH)task.o $(OBJ_PATH)arrqueue.o $(OBJ_PATH)listqueue.o $(OBJ_PATH)main.o
	$(CC) $(CFLAGS) -o $@ $^ -I $(INC_PATH) -lm

$(OBJ_PATH)process.o: $(SRC_PATH)process.c $(INC_PATH)process.h $(INC_PATH)defines.h
	mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -o $@ -c $< -I $(INC_PATH)

$(OBJ_PATH)arrqueue.o: $(SRC_PATH)arrqueue.c $(INC_PATH)arrqueue.h $(INC_PATH)defines.h
	mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -o $@ -c $< -I $(INC_PATH)

$(OBJ_PATH)listqueue.o: $(SRC_PATH)listqueue.c $(INC_PATH)listqueue.h $(INC_PATH)defines.h
	mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -o $@ -c $< -I $(INC_PATH)

$(OBJ_PATH)task.o: $(SRC_PATH)task.c $(INC_PATH)task.h $(INC_PATH)defines.h
	mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -o $@ -c $< -I $(INC_PATH)

$(OBJ_PATH)main.o: $(SRC_PATH)main.c $(INC_PATH)defines.h
	mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -o $@ -c $< -I $(INC_PATH)


clean:
	rm $(OBJ_PATH)*.o *.exe