run: runme
	./runme

runme: main.o PreProcess.o FeatureExtractor.o
	g++ -o runme main.o PreProcess.o FeatureExtractor.o

PreProcess.o: ./SP/PreProcess.cpp
	g++ ./SP/PreProcess.cpp -o ./SP/PreProcess.o

FeatureExtractor.o: SP/FeatureExtractor.cpp
	g++ ./SP/FeatureExtractor.cpp -o ./SP/FeatureExtractor.o



 