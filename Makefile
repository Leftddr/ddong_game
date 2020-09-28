CPPFLAGS=-std=c++11 -lpthread -lncurses

ex_system: ex_system.cpp getch.h
	$(CXX) -o $@ $< $(CPPFLAGS)

clean: 
	rm -rf ex_system
