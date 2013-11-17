#Makefile

all:
	mkdir build
	g++ -c generateCourses.cpp -o build/generateCourses.o
	g++ -c generateFaculty.cpp -o build/generateFaculty.o
	g++ -c generateFriends.cpp -o build/generateFriends.o
	g++ -c generateStudent.cpp -o build/generateStudent.o
	g++ -c Network.cpp -o build/Network.o
	g++ -c Generator.cpp -o build/Generator.o 
	g++ Timekeeper.cpp -o build/Timekeeper
	g++ -o build/Generator Generator.o generateStudent.o generateFriends.o generateFaculty.o generateCourses.o -fopenmp
	g++ Analyzer.cpp -o build/Analyzer
clean:
	rm -fr build