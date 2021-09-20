
CXX = g++
CFLAGS = -Wall
LDFLAGS = 

target = _main
srcs = main.cpp 
objs = $(srcs:.cpp=.o)

.PHONY: all
all: $(target)

$(target): $(objs)
	$(CXX) $(LDFLAGS) -o _main $^

$(objs):%.o:%.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

.PHONY: FORCE
FORCE: 

clean:
	rm -f _main *.o

