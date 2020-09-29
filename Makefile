CPPFLAGS=-std=c++11 -lpthread -lncurses

ddong_game: ddong_game.cpp getch.h
	$(CXX) -o $@ $< $(CPPFLAGS)

clean: 
	rm -rf ddong_game
