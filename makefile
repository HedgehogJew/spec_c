TARGET = Parser.out
CC = gcc

PREF_SRC = ./src/
PREF_OBJ = ./obj/
SRC = $(wildcard $(PREF_SRC)*.c)
OBJ = $(filter-out $(PREF_OBJ)example.o, $(patsubst $(PREF_SRC)%.c, $(PREF_OBJ)%.o, $(SRC)))

$(TARGET) : $(OBJ)
	$(CC)  $(OBJ) -o $(TARGET)

$(PREF_OBJ)%.o : $(PREF_SRC)%.c
	$(CC) -c $< -o $@

clean :
	rm $(TARGET) $(PREF_OBJ)*.o *.out

example :
	$(CC) -c $(PREF_SRC)filework.c -o $(PREF_OBJ)filework.o
	$(CC) -c $(PREF_SRC)example.c -o $(PREF_OBJ)example.o
	$(CC) -c $(PREF_SRC)crc.c -o $(PREF_OBJ)crc.o
	$(CC) $(PREF_OBJ)filework.o $(PREF_OBJ)example.o $(PREF_OBJ)crc.o -o Example.out