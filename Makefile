OBJDIR?=./out
SRCDIR?=./turing-project
CPPFLAGS?=-I./turing-project
CXXFLAGS?=-g -Werror -Wall
SRCS=$(wildcard $(SRCDIR)/*.cpp)
OBJS=$(SRCS:./turing-project/%.cpp=$(OBJDIR)/%.o)

.PHONY: clean, run, test, all-test

turing: $(OBJS)
	g++ -g -o turing $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	g++ $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

run: turing
	./turing -v programs/case1.tm 0000

TEST_DIR?=./tests
TEST_SRC=$(wildcard $(TEST_DIR)/*.cpp)
TEST_EXE=$(TEST_SRC:./tests/%.cpp=$(TEST_DIR)/%)

$(TEST_DIR)/%: $(TEST_DIR)/%.cpp $(OBJS)
	g++ -I$(SRCDIR) -g -o $@ $< ./out/cli.o ./out/turing.o

all-test: $(TEST_EXE)

test: all-test
	./tests/test_cli
	./tests/test_get_turing_machine

clean:
	@rm $(OBJDIR)/*
	@if [ ! -z $(TEST_EXE) ]; then\
		rm $(TEST_EXE); \
	fi
	@if [ -f turing ]; then\
		rm turing; \
	fi
