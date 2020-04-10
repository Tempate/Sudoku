CXX           = g++
FLAGS         = -std=c++17 -static
RELEASE_FLAGS = $(FLAGS) -O3 -DNDEBUG -flto -march=native
DEBUG_FLAGS   = $(FLAGS) -O0 -Wall -Wextra -g -Wall -Wextra -pedantic

LINKER  = g++

EXEC    = Sudoku
SRCDIR  = src
TSTDIR  = tests
OBJDIR_SRC = obj/src
OBJDIR_TST = obj/tests
BINDIR  = bin

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(SRCDIR)/*.h)

TESTS    := $(wildcard $(TSTDIR)/*.cpp)

OBJECTS_SRC := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR_SRC)/%.o)
OBJECTS_TST := $(TESTS:$(TSTDIR)/%.cpp=$(OBJDIR_TST)/%.o)

$(BINDIR)/$(EXEC): $(BINDIR) $(OBJDIR_SRC) $(OBJECTS_SRC) $(OBJDIR_TST) $(OBJECTS_TST)
	@$(LINKER) -o $@ $(OBJECTS_SRC) $(OBJECTS_TST) $(LDFLAGS)

$(OBJECTS_SRC): $(OBJDIR_SRC)/%.o : $(SRCDIR)/%.cpp
	@$(CXX) $(FLAGS) -c $< -o $@

$(OBJECTS_TST): $(OBJDIR_TST)/%.o : $(TSTDIR)/%.cpp
	@$(CXX) $(FLAGS) -c $< -o $@

release:
	$(MAKE) FLAGS="$(RELEASE_FLAGS)"

debug:
	$(MAKE) FLAGS="$(DEBUG_FLAGS)" EXEC="$(EXEC)-debug"

bin:
	mkdir -p $(BINDIR)

obj:
	mkdir -p $(OBJDIR_SRC)
	mkdir -p $(OBJDIR_TST)

clean:
	rm -f $(OBJECTS_SRC)
	rm -f $(OBJECTS_TST)

.PHONY: clean