echo "Build ecallApp.cpp start"
g++ -Wall -pthread -o ecallApp.exe ecallApp.cpp
echo "Build ecallApp.cpp finish"
echo "Build main.cpp start"
g++ -Wall -pthread -o main.exe main.cpp lib.cpp
echo "Build main.cpp finish"
