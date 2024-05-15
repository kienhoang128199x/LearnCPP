myOFile:= ecallApp.o lib.o main.o
ecallApp: $(myOFile)
	echo "check dependency file" $(myOFile)
	echo "Build ecallApp.exe"
	g++ -Wall -pthread ecallApp.o lib.o -o ecallApp.exe
	echo "Build main.exe"
	g++ -Wall -pthread main.o lib.o -o main.exe

ecallApp.o: ecallApp.cpp
	g++ -c -fpic ecallApp.cpp -o ecallApp.o

main.o: main.cpp
	g++ -c -fpic main.cpp -o main.o

lib.o: lib.cpp
	g++ -c -fpic lib.cpp -o lib.o

clean:
	rm *.exe *.o

run: 
	./main.exe