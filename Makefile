################################################################################
### Ce fichier appartient au projet Aerial Image Project (AIP).
###
### Copyright (c) ...
###
### Les sources de AIP sont distribuées sans aucune garantie.
################################################################################

CC = g++
CFLAGS  = -g -Wall -std=c++14

INCLUDES = -I.
LFLAGS = -lm

OBJ = obj/Color.o obj/Image.o obj/Analyst.o obj/FireSimulator.o obj/main.o
TARGET = main.exe

all: $(TARGET)

$(TARGET): $(OBJ)
		$(CC) $(CFLAGS) $(LFLAGS) $(OBJ) -o $(TARGET)

obj/main.o: src/main.cpp head/Color.h head/Image.h head/Analyst.h
		$(CC) $(CFLAGS) $(INCLUDES) -c src/main.cpp -o obj/main.o

obj/Color.o: src/Color.cpp head/Color.h
		$(CC) $(CFLAGS) $(INCLUDES) -c src/Color.cpp -o obj/Color.o

obj/Image.o: src/Image.cpp head/Color.h head/Image.h
		$(CC) $(CFLAGS) $(INCLUDES) -c src/Image.cpp -o obj/Image.o

obj/Analyst.o: src/Analyst.cpp head/Color.h head/Image.h head/Analyst.h
		$(CC) $(CFLAGS) $(INCLUDES) -c src/Analyst.cpp -o obj/Analyst.o

obj/FireSimulator.o: src/FireSimulator.cpp head/Color.h head/Image.h head/Analyst.h head/FireSimulator.h
		$(CC) $(CFLAGS) $(INCLUDES) -c src/FireSimulator.cpp -o obj/FireSimulator.o

clean:
		rm -f *~ *.o *.aip *.svg main.exe
