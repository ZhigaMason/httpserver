CXX = clang++
CXXFLAGS = -std=c++20 -Wall -pedantic -O2
DEBUGFLAGS = -O0 -g -fsanitize=address

LD = $(CXX)
LDFLAGS = $(CXXFLAGS)

SRC = 	src/main.cpp\
	src/server/server.cpp\
	src/request/request.cpp\
	src/response/static_response_factory.cpp

OBJ = main.out debug.out

DEBUG_DIR = _debug

all: $(OBJ)

main.out: $(SRC:.cpp=.o)
	$(LD) $(LDFLAGS) -o $@ $^

debug.out: $(addprefix $(DEBUG_DIR)/, $(SRC:.cpp=.o))
	$(LD) $(LDFLAGS) $(DEBUGFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(DEBUG_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) -c -o $@ $<

clean:
	rm -f *.out $(SRC:.cpp=.o)
	rm -rf $(DEBUG_DIR)
