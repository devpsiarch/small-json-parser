src := $(wildcard source/*.cpp)
build:
	g++ main.cpp $(src) -o main
run:build
	./main
clean:
	rm main
