CC := g++

CFLAGS := -Wall -Werror -g

SRC := AStar.cpp Grid.cpp Point.cpp Square.cpp PathFinder.cpp main.cpp
OUT := main

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)
