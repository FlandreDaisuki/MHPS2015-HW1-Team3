TABUPAIR = -t pair
SIZE20 = -l 20
# 20 / 50 / 100 / 20~100
SIZE50 = -l 100
# 100 / 300 / 600 / 100~600
SIZE100 = -l 500
# 500 / 1000 / 2000 500~2000

all:
	make once TABUPAIR="-t pair" SIZE20="-l 20" SIZE50="-l 100" SIZE100="-l 500" PREX="pair-min-"
	make once TABUPAIR="-t pair" SIZE20="-l 50" SIZE50="-l 300" SIZE100="-l 1000" PREX="pair-mid-"
	make once TABUPAIR="-t pair" SIZE20="-l 100" SIZE50="-l 600" SIZE100="-l 2000" PREX="pair-max-"
	make once TABUPAIR="-t pair" SIZE20="-l 20~100" SIZE50="-l 100~600" SIZE100="-l 500~2000" PREX="pair-var-"
	make once TABUPAIR="-t objv" SIZE20="-l 20" SIZE50="-l 100" SIZE100="-l 500" PREX="objv-min-"
	make once TABUPAIR="-t objv" SIZE20="-l 50" SIZE50="-l 300" SIZE100="-l 1000" PREX="objv-mid-"
	make once TABUPAIR="-t objv" SIZE20="-l 100" SIZE50="-l 600" SIZE100="-l 2000" PREX="objv-max-"
	make once TABUPAIR="-t objv" SIZE20="-l 20~100" SIZE50="-l 100~600" SIZE100="-l 500~2000" PREX="objv-var-"
once: build twenty fifty hundred clean
	cd outputdata
	make rename
	cd ../csv
	make rename
	cd ..
	mv outputdata/* total/
	mv csv/* total/
twenty:
	./tabu -i data/tai20_5_1.txt $(TABUPAIR) $(SIZE20)
	./tabu -i data/tai20_10_1.txt $(TABUPAIR) $(SIZE20)
	./tabu -i data/tai20_20_1.txt $(TABUPAIR) $(SIZE20)
fifty:
	./tabu -i data/tai50_5_1.txt $(TABUPAIR) $(SIZE50)
	./tabu -i data/tai50_10_1.txt $(TABUPAIR) $(SIZE50)
	./tabu -i data/tai50_20_1.txt $(TABUPAIR) $(SIZE50)
hundred:
	./tabu -i data/tai100_5_1.txt $(TABUPAIR) $(SIZE100)
	./tabu -i data/tai100_10_1.txt $(TABUPAIR) $(SIZE100)
	./tabu -i data/tai100_20_1.txt $(TABUPAIR) $(SIZE100)
rename:
	mv tai20_5_1.txt $(PREX)tai20_5_1.txt
	mv tai20_10_1.txt $(PREX)tai20_10_1.txt
	mv tai20_20_1.txt $(PREX)tai20_20_1.txt
	mv tai50_5_1.txt $(PREX)tai50_5_1.txt
	mv tai50_10_1.txt $(PREX)tai50_10_1.txt
	mv tai50_20_1.txt $(PREX)tai50_20_1.txt
	mv tai100_5_1.txt $(PREX)tai100_5_1.txt
	mv tai100_10_1.txt $(PREX)tai100_10_1.txt
	mv tai100_20_1.txt $(PREX)tai100_20_1.txt
debug: build
	./tabu
	make clean
build:
	g++ -std=c++11 -O3 *.cpp -o tabu
clean:
	rm ./tabu