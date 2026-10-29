TARGET   = CastOut-Engine
CXX      = g++
CXXFLAGS = -g -02 -Wall -Wextra -std=c++17 -I./headers -MMD
LDFLAGS  = -lsfml-graphics -lsfml-window -lsfml-system

SRCS = $(shell find src -name '*.cpp')
OBJS = $(SRCS:src/%.cpp=obj/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf obj $(TARGET)

-include $(OBJS:.o=.d)

.PHONY: all run clean