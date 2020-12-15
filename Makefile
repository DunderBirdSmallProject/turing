OBJDIR?=./build/out
TESTOBJDIR?=./build/tests
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
test_cases:=$(TEST_SRC:$(TEST_DIR)/%.cpp=%)
test_objs:=$(test_cases:%=$(TESTOBJDIR)/%)

INCOBJS=$(filter-out %main.o,$(OBJS))

$(test_objs): $(TEST_SRC) $(INCOBJS)
	g++ -I$(SRCDIR) -g -o $@ $< $(INCOBJS)

all-test: $(test_objs)

test: all-test
	$(TESTOBJDIR)/test_cli
	$(TESTOBJDIR)/test_get_turing_machine

clean:
	@rm $(TESTOBJDIR)/*
	@rm $(OBJDIR)/*
	@if [ -f turing ]; then\
		rm turing; \
	fi
