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

SOURCES  := $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(TSTDIR)/*.cpp)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR_SRC)/%.o)
OBJECTS  := $(OBJECTS:$(TSTDIR)/%.cpp=$(OBJDIR_TST)/%.o)

$(BINDIR)/$(EXEC): $(BINDIR) $(OBJDIR_SRC) $(OBJECTS)
	@$(LINKER) -o $@ $(OBJECTS) $(LDFLAGS)

$(BINDIR)/$(EXEC): $(BINDIR) $(OBJDIR_TST) $(OBJECTS)
	@$(LINKER) -o $@ $(OBJECTS) $(LDFLAGS)

$(OBJECTS): $(OBJDIR_SRC)/%.o : $(SRCDIR)/%.cpp
	@$(CXX) $(FLAGS) -c $< -o $@

$(OBJECTS): $(OBJDIR_TST)/%.o : $(TSTDIR)/%.cpp
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
	rm -f $(OBJECTS)

.PHONY: clean