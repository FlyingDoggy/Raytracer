FILE=Raytracing

########
#   Directories
S_DIR=Source
B_DIR=Build

########
#   Output
EXEC=$(B_DIR)/$(FILE)

# default build settings
CC_OPTS=-c -pipe -Wall -Wno-switch -O3 -std=c++11
LN_OPTS=
CC=g++

########
#       SDL options
SDL_CFLAGS := $(shell sdl-config --cflags)
GLM_CFLAGS := -I$(GLMDIR)
SDL_LDFLAGS := $(shell sdl-config --libs)

########
#   This is the default action
all:Build


########
#   Object list
#
OBJ = $(B_DIR)/$(FILE).o $(B_DIR)/Intersection.o $(B_DIR)/Camera.o $(B_DIR)/Lighting.o $(B_DIR)/Triangle.o


########
#   Objects


$(B_DIR)/$(FILE).o : $(S_DIR)/$(FILE).cpp $(S_DIR)/SDLauxiliary.h $(S_DIR)/TestModel.h
	$(CC) $(CC_OPTS) -o $(B_DIR)/$(FILE).o $(S_DIR)/$(FILE).cpp $(SDL_CFLAGS) $(GLM_CFLAGS)

$(B_DIR)/Intersection.o : $(S_DIR)/Intersection.cpp $(S_DIR)/Intersection.h
	$(CC) $(CC_OPTS) -o $(B_DIR)/Intersection.o $(S_DIR)/Intersection.cpp $(SDL_CFLAGS) $(GLM_CFLAGS)

$(B_DIR)/Camera.o : $(S_DIR)/Camera.cpp $(S_DIR)/Camera.h
	$(CC) $(CC_OPTS) -o $(B_DIR)/Camera.o $(S_DIR)/Camera.cpp $(SDL_CFLAGS) $(GLM_CFLAGS)

$(B_DIR)/Lighting.o : $(S_DIR)/Lighting.cpp $(S_DIR)/Lighting.h
	$(CC) $(CC_OPTS) -o $(B_DIR)/Lighting.o $(S_DIR)/Lighting.cpp $(SDL_CFLAGS) $(GLM_CFLAGS)

$(B_DIR)/Triangle.o : $(S_DIR)/Triangle.cpp $(S_DIR)/Triangle.h
	$(CC) $(CC_OPTS) -o $(B_DIR)/Triangle.o $(S_DIR)/Triangle.cpp $(SDL_CFLAGS) $(GLM_CFLAGS)

########
#   Main build rule
Build : $(OBJ) Makefile
	$(CC) $(LN_OPTS) -o $(EXEC) $(OBJ) $(SDL_LDFLAGS)


clean:
	rm -f $(B_DIR)/*
