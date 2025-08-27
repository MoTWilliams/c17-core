# Use GCC compiler
CC := gcc

# Set default value of the debug and colors flag
DEBUG ?= 0
NO_COLOR ?= 1

# Make 'prog' the default for plain `make` command
.DEFAULT_GOAL := prog

# Enforce strict (pedantic) C99 standard
# Enable common and extra warnings
# Use descriptive debug information (-g) 
# Set DEBUG and NO_COLOR macros from Makefile
# Generate dependency lists and add phony rules for headers
CFLAGS += -std=c99 -pedantic -Wall -Wextra -g \
		  -DDEBUG=$(DEBUG) -DNO_COLOR=$(NO_COLOR) -MMD -MP

# Construct build trees
BUILDDIR := build
OBJDIR := $(BUILDDIR)/$(BUILDTYPE)/obj
DEPDIR := $(BUILDDIR)/$(BUILDTYPE)/dep

%/:
	@mkdir -p $@

# --- ALL BUILDS ---
INCLUDE := $(addprefix -iquote , $(sort $(shell find src -type d)))
CPPFLAGS += $(INCLUDE)

SRC := $(sort $(shell find src -name '*.c'))

CORE_OBJDIR := $(OBJDIR)/core
CORE_OBJ := $(patsubst src/%.c, $(CORE_OBJDIR)/%.o, $(SRC))

CORE_DEPDIR := $(DEPDIR)/core
CORE_DEP := $(patsubst $(CORE_OBJDIR)/%.o, $(CORE_DEPDIR)/%.d, $(CORE_OBJ))

$(CORE_OBJDIR)/%.o: src/%.c
	@mkdir -p $(dir $@) $(dir $(CORE_DEPDIR)/$*.d)
	$(CC) $(CPPFLAGS) $(CFLAGS) \
		-MF $(CORE_DEPDIR)/$*.d -c -o $@ $<

# --- NORMAL BUILD ---
APP_INC := $(addprefix -iquote , $(sort $(shell find app -type d)))

APP := $(sort $(shell find app -name '*.c'))

APP_OBJDIR := $(OBJDIR)/app
APP_OBJ := $(patsubst app/%.c, $(APP_OBJDIR)/%.o, $(APP))

APP_DEPDIR := $(DEPDIR)/app
APP_DEP := $(patsubst $(APP_OBJDIR)/%.o, $(APP_DEPDIR)/%.d, $(APP_OBJ))

$(APP_OBJDIR)/%.o: CPPFLAGS += $(APP_INC)

$(APP_OBJDIR)/%.o: app/%.c
	@mkdir -p $(dir $@) $(dir $(APP_DEPDIR)/$*.d)
	$(CC) $(CPPFLAGS) $(CFLAGS) \
		-MF $(APP_DEPDIR)/$*.d -c -o $@ $<

# Create the executable
BIN := bin/prog

prog: $(BIN)
.PHONY: prog

$(BIN): $(CORE_OBJ) $(APP_OBJ) | bin/
	$(CC) -o $@ $^

# --- UNIT TEST BUILD ---
TEST_INC := $(addprefix -iquote , $(sort $(shell find test -type d)))

TEST := $(sort $(shell find test -name '*.c'))
LIB_OBJ := $(CORE_OBJ)
LIB_ARCHIVE := $(BUILDDIR)/$(BUILDTYPE)/libprog.a

$(LIB_ARCHIVE): $(LIB_OBJ) | $(dir $(LIB_ARCHIVE))
	ar rcs $@ $^

TEST_OBJDIR := $(OBJDIR)/test
TEST_OBJ := $(patsubst test/%.c, $(TEST_OBJDIR)/%.o, $(TEST))

TEST_DEPDIR := $(DEPDIR)/test
TEST_DEP := $(patsubst $(TEST_OBJDIR)/%.o, $(TEST_DEPDIR)/%.d, $(TEST_OBJ))

$(TEST_OBJDIR)/%.o: CPPFLAGS += $(TEST_INC)

$(TEST_OBJDIR)/%.o: test/%.c
	@mkdir -p $(dir $@) $(dir $(TEST_DEPDIR)/$*.d)
	$(CC) $(CPPFLAGS) $(CFLAGS) \
		-MF $(TEST_DEPDIR)/$*.d -c -o $@ $<

TEST_BIN := bin/prog_test

$(TEST_BIN): $(TEST_OBJ) $(LIB_ARCHIVE) | bin/
	$(CC) -o $@ $^

# Include generated dependencies
-include $(CORE_DEP) $(APP_DEP) $(TEST_DEP)

.PHONY: clean run release debug valgrind test test_valgrind

clean:
	$(RM) -r $(BUILDDIR) bin

run: $(BIN)
	./$(BIN)

release:
	$(MAKE) BUILDTYPE=release prog
	./$(BIN)

debug:
	$(MAKE) BUILDTYPE=debug DEBUG=1 NO_COLOR=1 prog
	./$(BIN) one two three

valgrind: $(BIN)
	valgrind --leak-check=full \
			 --track-origins=yes \
			 --show-leak-kinds=all \
			 ./$(BIN)

test:
	$(MAKE) BUILDTYPE=test DEBUG=1 $(TEST_BIN)
	./$(TEST_BIN)

test_valgrind: $(TEST_BIN)
	valgrind --leak-check=full \
			 --track-origins=yes \
			 --show-leak-kinds=all \
			 ./$(TEST_BIN)