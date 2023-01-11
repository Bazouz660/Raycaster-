CC = g++
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
CFLAGS = -I include -std=c++17
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

all: game

game: $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

re:	clean all

clean:
	rm -f src/*.o game
