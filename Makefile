CXX           = g++
FLAGS         = -std=c++17 -static
RELEASE_FLAGS = $(FLAGS) -O3 -DNDEBUG -flto -march=native
DEBUG_FLAGS   = $(FLAGS) -O0 -Wall -Wextra -g -Wall -Wextra -pedantic

LINKER  = g++

BINDIR  = bin

EXEC_SRC = Sudoku
EXEC_TST = Tests

SRCDIR  = src
TSTDIR  = tests

OBJDIR_SRC = obj/src
OBJDIR_TST = obj/tests

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
HEADERS := $(wildcard $(SRCDIR)/*.h)

SOURCES_TST := $(wildcard $(TSTDIR)/*.cpp)
HEADERS_TST := $(wildcard $(TSTDIR)/*.h)

OBJECTS_SRC := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR_SRC)/%.o)
OBJECTS_TST := $(TESTS:$(TSTDIR)/%.cpp=$(OBJDIR_TST)/%.o)

$(BINDIR)/$(EXEC_SRC): $(BINDIR) $(OBJDIR_SRC) $(OBJECTS_SRC)
	@$(LINKER) -o $@ $(OBJECTS_SRC) $(LDFLAGS)

$(BINDIR)/$(EXEC_TST): $(BINDIR) $(OBJDIR_TST) $(OBJECTS_TST)
	@$(LINKER) -o $@ $(OBJECTS_TST) $(LDFLAGS)

$(OBJECTS_SRC): $(OBJDIR_SRC)/%.o : $(SRCDIR)/%.cpp
	@$(CXX) $(FLAGS) -c $< -o $@

$(OBJECTS_TST): $(OBJDIR_TST)/%.o : $(TSTDIR)/%.cpp
	@$(CXX) $(FLAGS) -c $< -o $@

release:
	$(MAKE) FLAGS="$(RELEASE_FLAGS)"

debug:
	$(MAKE) FLAGS="$(DEBUG_FLAGS)" EXEC="$(EXEC_SRC)-debug"

bin:
	mkdir -p $(BINDIR)

obj:
	mkdir -p $(OBJDIR_SRC)
	mkdir -p $(OBJDIR_TST)

clean:
	rm -f $(OBJECTS_SRC)
	rm -f $(OBJECTS_TST)

.PHONY: clean