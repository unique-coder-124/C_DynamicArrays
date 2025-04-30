ifeq ($(OS),Windows_NT)
  RM = del
  MKDIR = if not exist "$(BUILDDIR)" mkdir "$(BUILDDIR)"
else
  RM = rm -f
  MKDIR = mkdir -p $(BUILDDIR)
endif

CC = clang
CFLAGS =

EXE = main.exe
OBJ = main.o

SRC = main.c
HEADER = DynamicArrays.h

BUILDDIR = build/
OBJFILE = $(BUILDDIR)$(OBJ)
EXEFILE = $(BUILDDIR)$(EXE)

all: $(EXEFILE)

$(EXEFILE): $(OBJFILE)
	$(CC) $(OBJFILE) -o $(EXEFILE)

$(OBJFILE): $(SRC) $(HEADER)
	$(MKDIR)
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJFILE)

clean:
	$(RM) $(BUILDDIR)*.o $(BUILDDIR)*.exe
