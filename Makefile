all:
	g++ -std=c++11 *.cpp -o tabu && ./tabu
clean:
	rm tabu