OBJDIR?=./out
SRCDIR?=./turing-project
CPPFLAGS?=-I./turing-project
CXXFLAGS?=-g -Werror -Wall -std=c++11
SRCS=$(wildcard $(SRCDIR)/*.cpp)
OBJS=$(SRCS:./turing-project/%.cpp=$(OBJDIR)/%.o)

.PHONY: clean, run, test, all-test

turing: $(OBJS)
	g++ -g -o turing $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	g++ $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

run: turing
	@./turing tests/palindrome_detector_2tapes.tm 11

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
	@rm $(TEST_EXE)
	@rm $(OBJDIR)/*
	@if [ -f turing ]; then\
		rm turing; \
	fi
