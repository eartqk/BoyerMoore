all: solution

solution: main.cpp
	g++ -std=c++17 main.cpp -Ofast -o solution

clean:
	rm -f solution
