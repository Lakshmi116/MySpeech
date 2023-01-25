runme: app.o speech_utilities.o
	g++ -o runme app.o speech_utilities.o

app.o: app.cpp
	g++ -c -g app.cpp

speech_utilities.o: speech_utilities.cpp
	g++ -c -g speech_utilities.cpp 

clean:
	rm app.o speech_utilities.o runme

run: runme

