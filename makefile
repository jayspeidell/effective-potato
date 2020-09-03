MAINPROG=game
CC=gcc
CXX=g++
CPPFLAGS=-g -std=c++17
LFLAGS= -L/usr/local/lib/
CFLAGS=-g -I/usr/local/include/
LIBS= -lglfw3 -lGL -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread
VPATH = src
BUILDDIR = build
TESTDIR = test
TARGET=$(BUILDDIR)/$(MAINPROG)
CPPS=$(wildcard $(VPATH)/*.cpp)
LINK=g++ $(CPPFLAGS)
OBJS=$(CPPS:%.cpp=%.o)




all: $(TARGET)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -MMD -o $@ -c $(VPATH)/*.cpp $(CFLAGS)

$(TARGET): $(OBJS)
	$(LINK) $(FLAGS) -o $(TARGET) $^ $(LFLAGS)

dependencies:
	cmake lib/GLFW/.
	make -C lib/GLFW


run:
	./$(TARGET)

clean:
	-/bin/rm -rf *.d *.o $(TARGET)
