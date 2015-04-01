TABUPAIR = -t pair
SIZE20 = -l 20
# 20 / 50 / 100 / 20~100
SIZE50 = -l 100
# 100 / 300 / 600 / 100~600
SIZE100 = -l 500
# 500 / 1000 / 2000 500~2000
all: build twenty fifty	hundred clean
twenty:
	./tabu -i data/tai20_5_1.txt
	./tabu -i data/tai20_10_1.txt
	./tabu -i data/tai20_20_1.txt
fifty:
	./tabu -i data/tai50_5_1.txt
	./tabu -i data/tai50_10_1.txt
	./tabu -i data/tai50_20_1.txt
hundred:
	./tabu -i data/tai100_5_1.txt
	./tabu -i data/tai100_10_1.txt
	./tabu -i data/tai100_20_1.txt
debug: build
	./tabu
	make clean
build:
	g++ -std=c++11 -O3 *.cpp -o tabu
clean:
	rm ./tabu