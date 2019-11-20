CXX = g++
CXXFLAGS = -g -Wall -Werror -std=c++11 $(FLAGS)

TARGET = auto
SRCS = tree.cpp parser.cpp
OBJS = ${SRCS:.cpp=.o}

auto: main.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -O3 -o auto main.cpp $(OBJS)

clean:
	${RM} ${TARGET} ${OBJS}
	-rm -r *.dSYM