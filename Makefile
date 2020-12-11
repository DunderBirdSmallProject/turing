OBJDIR?=./out
SRCDIR?=./turing-project
CPPFLAGS?=-I./turing-project
CXXFLAGS?=-Werror -Wall
SRCS=$(wildcard $(SRCDIR)/*.cpp)
OBJS=$(SRCS:./turing-project/%.cpp=$(OBJDIR)/%.o)

$(info $(SRCS))
$(info $(OBJS))

turing: $(OBJS)
	g++ -o turing $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	g++ $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean, run

clean:
	rm $(OBJDIR)/*
	rm turing

run: turing
	./turing -v programs/case1.tm 0000
