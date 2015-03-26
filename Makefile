all: build twenty fifty	hundred clean
twenty:
	./tabu data/tai20_5_1.txt
	./tabu data/tai20_10_1.txt
	./tabu data/tai20_20_1.txt
fifty:
	./tabu data/tai50_5_1.txt
	./tabu data/tai50_10_1.txt
	./tabu data/tai50_20_1.txt
hundred:
	./tabu data/tai100_5_1.txt
	./tabu data/tai100_10_1.txt
	./tabu data/tai100_20_1.txt
debug: build
	./tabu
	make clean
build:
	g++ -std=c++11 -O3 *.cpp -o tabu
clean:
	rm ./tabu