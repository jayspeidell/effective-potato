MAINPROG=game
CC=gcc
CXX=g++
CPPFLAGS=-g -std=c++17
LFLAGS= -L/usr/local/lib/ -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl
CFLAGS=-g -I/usr/local/include/
LIBS= -lglfw3 -lGL -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread
VPATH = src
BUILDDIR = build
TESTDIR = test
TARGET=$(BUILDDIR)/$(MAINPROG)
CPPS=$(wildcard $(VPATH)/*.cpp) $(wildcard $(VPATH)/*.c)
LINK=g++ $(CPPFLAGS)
OBJS= $(CPPS:%.c=%.o)




all: $(TARGET)
	# Develop shaders in source file, deploy in build. 
	rsync -a --delete src/shaders/ build/shaders/

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -MMD -o $@ -c $*.cpp glad.c $(CFLAGS)
	#$(CXX) $(CPPFLAGS) -MMD -o $@ -c $(CPPS) $(CFLAGS)

$(TARGET): $(OBJS)
	$(LINK) $(FLAGS) -o $(TARGET) $^ $(LFLAGS)

run:
	./$(TARGET)

clean:
	-/bin/rm -rf *.d *.o $(TARGET)
