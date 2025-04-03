src := jsonParser/jsonParser.cpp
better_src := jsonParserBetter/jsonParserBetter.cpp
better:
	g++ main.cpp $(better_src) -o main
build:
	g++ main.cpp $(src) -o main
run:better
	./main
clean:
	rm main
