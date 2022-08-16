# paths
LIB = ./lib
INCLUDE = ./include
MODULES = ./modules

# compiler
CC = gcc

# Compile options
CFLAGS = -Wall -Werror -g -I$(INCLUDE)
LDFLAGS = -lm -ldl -lpthread -lGL

# for WSL
export DISPLAY ?= :0

# Object files
OBJS = game.o $(MODULES)/ADTList.o $(MODULES)/ADTSet.o $(MODULES)/ADTVector.o $(MODULES)/state.o $(MODULES)/beams.o $(MODULES)/draw_related_funcs.o $(MODULES)/set_utils.o $(MODULES)/title_screen.o $(MODULES)/game_screen.o $(MODULES)/jet.o $(MODULES)/missile.o $(MODULES)/game_screen_draw.o $(MODULES)/interface.o $(MODULES)/enemies.o

# Executable file
EXEC = AlienInvasion

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(LIB)/libraylib_linux.a -o $(EXEC) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC)