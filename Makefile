PROG = tester
SRCS = tester.cpp 
DEPS = $(SRCS:.cpp=.d)
OBJS = $(SRCS:.cpp=.o)

CXXFLAGS = -Wall -g

all: $(PROG)

$(PROG): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

%.d: %.cpp
	$(CXX) -MM $(CPPFLAGS) $< > $@
	@sed -i 's/^\($*\)\.o:/\1.o $@:/' $@

-include $(DEPS)

clean:
	@$(RM) $(OBJS) $(DEPS) $(PROG)



