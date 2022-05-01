# Παράμετροι για χρήση της βιλιοθήκης raylib

ifdef WASM
	# Emscripten
	CC = emcc
	CFLAGS += -DWASM
	LDFLAGS += -s ASYNCIFY -s USE_GLFW=3  -s TOTAL_MEMORY=67108864 -s FORCE_FILESYSTEM=1 --shell-file $(LIB)/libraylib.html
	ifneq (,$(wildcard assets))
		LDFLAGS += --preload-file=assets
	endif
	EXEC := $(EXEC).html

else ifeq ($(OS),Windows_NT)
	# Windows
	LDFLAGS += -lgdi32 -lwinmm

	ifneq (,$(findstring Microsoft,$(shell uname -a)))
		# Windows build from within WSL (make OS=Windows_NT)
		# Use the mingw32 compiler
		CC = x86_64-w64-mingw32-gcc
	endif

else ifeq ($(shell uname -s),Linux)
	# Linux
	LDFLAGS += -ldl -lpthread -lGL
	export DISPLAY ?= :0	# mainly for WSL
else
	# macOS
	LDFLAGS += -framework OpenGL -framework IOKit -framework Cocoa
endif