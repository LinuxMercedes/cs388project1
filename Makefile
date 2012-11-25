all:
	g++ -g *.cpp 

check-syntax:
	g++ -o /dev/null -S ${CHK_SOURCES}
