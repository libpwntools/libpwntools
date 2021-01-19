all:
	g++ -std=c++11 -fstack-protector-all -g ./exp.cpp -o exp
clean:
	exp
