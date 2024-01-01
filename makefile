CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
FLTKFLAGS = -lfltk -lfltk_images
SRC = ./src/main.cpp ./src/mapWindow.cpp

map:
	$(CXX) $(SRC) -o main $(CXXFLAGS) $(FLTKFLAGS)

debug:
	$(CXX) $(SRC) -g -o main $(CXXFLAGS) $(FLTKFLAGS)

clean:
	rm main