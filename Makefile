all:
	g++ -g test.cpp 

check-syntax:
	g++ -o /dev/null -S ${CHK_SOURCES}
