# The compiler
CC = g++

# Compiler flags
CFLAGS  = -std=c++11

# OpenCV libraries
CVLIBPATH = -L/opt/homebrew/Cellar/opencv/4.7.0_1/lib/
CVLIBS = -lopencv_highgui -lopencv_core -lopencv_imgcodecs -lopencv_imgproc

# OpenCV headers
CVHEADERPATH = -I/opt/homebrew/Cellar/opencv/4.7.0_1/include/opencv4

# Build targets
task1: lab1_task1/helloworld.cpp
	$(CC) $(CFLAGS) -o task1 lab1_task1/helloworld.cpp $(CVHEADERPATH) $(CVLIBPATH) $(CVLIBS)

task2: lab1_task2/helloworld.cpp
	$(CC) $(CFLAGS) -o task2 lab1_task2/helloworld.cpp $(CVHEADERPATH) $(CVLIBPATH) $(CVLIBS)

task3: lab1_task3/helloworld.cpp
	$(CC) $(CFLAGS) -o task3 lab1_task3/helloworld.cpp $(CVHEADERPATH) $(CVLIBPATH) $(CVLIBS)

task4: lab1_task4/helloworld.cpp
	$(CC) $(CFLAGS) -o task4 lab1_task4/helloworld.cpp $(CVHEADERPATH) $(CVLIBPATH) $(CVLIBS)

task5: lab1_task5/helloworld.cpp
	$(CC) $(CFLAGS) -o task5 lab1_task5/helloworld.cpp $(CVHEADERPATH) $(CVLIBPATH) $(CVLIBS)

clean:
	$(RM) task1 task2 task3 task4 task5