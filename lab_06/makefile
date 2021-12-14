INC_PATH=./inc/
OUT_PATH=./out/
SRC_PATH=./src/
UNIT_PATH=./unit_tests/
DEBUG_FLAGS=-g -ggdb -DDEBUG

APP=app.exe

CC=gcc
FLAGS=-std=gnu99 -Wall -Werror -Wpedantic -Wextra -Wfloat-equal -Wfloat-conversion -g -ggdb -DDEBUG
CHECK_FLAGS_HOME=-lcheck -lm -lrt -lsubunit -lpthread
CHECK_FLAGS=-lcheck

SRC=$(wildcard $(SRC_PATH)*.c)
OUT=$(patsubst $(SRC_PATH)%.c, $(OUT_PATH)%.o, $(SRC))
MAINS=$(OUT_PATH)main.o
UNIT_SRC=$(UNIT_PATH)check_main.c
UNIT_OUT=$(patsubst $(UNIT_PATH)%.c, $(OUT_PATH)%.o, $(UNIT_SRC))

$(APP): dirs $(OUT) $(OUT_PATH)main.o
	$(CC) $(OUT) -o $@

$(OUT_PATH)%.o: $(SRC_PATH)%.c $(INC_PATH)%.h
	$(CC) $(FLAGS) $< -c -o $@ -I $(INC_PATH)

$(OUT_PATH)%.o: $(SRC_PATH)%.c
	$(CC) $(FLAGS) $< -c -o $@ -I $(INC_PATH)

unit_tests.exe: dirs $(UNIT_OUT) $(filter-out $(MAINS), $(OUT))
	$(CC) $(FLAGS) -o $@ $(UNIT_OUT) $(filter-out $(MAINS), $(OUT)) $(CHECK_FLAGS)

debug_unit_tests.exe: dirs $(UNIT_OUT) $(filter-out $(MAINS), $(OUT))
	$(CC) $(FLAGS) $(DEBUG_FLAGS) -o $@ $(UNIT_OUT) $(filter-out $(MAINS), $(OUT)) $(CHECK_FLAGS_HOME)

$(OUT_PATH)%.o: $(UNIT_PATH)%.c
	$(CC) $(FLAGS) -o $@ $< -c -I $(INC_PATH)

$(OUT_PATH)%.o: $(UNIT_PATH)%.c $(SRC_PATH)%.h
	$(CC) $(FLAGS) -o $@  $< -c -I $(INC_PATH)

.PHONY: clean
clean:
	rm -rf *.exe $(OUT_PATH)

.PHONY: dirs
dirs:
	mkdir -p $(OUT_PATH)
