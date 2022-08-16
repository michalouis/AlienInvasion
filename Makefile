# paths
LIB = ./lib
INCLUDE = ./include
MODULES = ./modules

# compiler
CC = gcc

# Compile options. Το -I<dir> λέει στον compiler να αναζητήσει εκεί include files
CFLAGS = -Wall -Werror -g -I$(INCLUDE)
LDFLAGS = -lm

# Αρχεία .o
OBJS = game.o $(MODULES)/state.o $(MODULES)/beams.o $(MODULES)/draw_related_funcs.o $(MODULES)/set_utils.o $(MODULES)/title_screen.o $(MODULES)/game_screen.o $(MODULES)/jet.o $(MODULES)/missile.o $(MODULES)/game_screen_draw.o $(MODULES)/interface.o $(MODULES)/enemies.o

# Το εκτελέσιμο πρόγραμμα
EXEC = AlienInvasion

# # Παράμετροι της βιβλιοθήκης raylib
# include $(LIB)/libraylib.mk

LDFLAGS += -ldl -lpthread -lGL

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(LIB)/libraylib_linux.a -o $(EXEC) $(LDFLAGS)

# # Για να φτιάξουμε τα k08.a/libraylib.a τρέχουμε το make στο lib directory.
# $(LIB)/%.a:
# 	$(MAKE) -C $(LIB) $*.a

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC)