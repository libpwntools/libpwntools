all:
	g++ -std=c++11 -fstack-protector-all -g test/exp.cpp -o test/exp
clean:
	exp
