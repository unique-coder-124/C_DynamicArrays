ifeq ($(OS),Windows_NT)
  RM = del
else
  RM = rm -f
endif

CC = clang
CFLAGS =

EXE = main.exe
OBJ = main.o

SRC = main.c
HEADER = DynamicArrays.h

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $(EXE)

$(OBJ): $(SRC) $(HEADER)
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)

clean:
	$(RM) $(OBJ) $(EXE)
