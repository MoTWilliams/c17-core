CC := gcc
DEBUG ?= 0

# Include all subdirectories of src/ and test/
INCLUDE := $(sort $(shell find src test -type d))	# Build the list
CPPFLAGS += $(addprefix -I, $(INCLUDE)) -MMD -MP	# Turn into -I flags

# Enforce C89 standard, enable all common warnings, debug flag
CFLAGS += -std=c89 -pedantic -Wall -Wextra -g -DDEBUG=$(DEBUG)

# Link flags for including libraries later on
LDFLAGS +=
LDLIBS +=

# LIBRARY: Core source files--used for all builds
SRC := $(shell find src -name '*.c')
CORE_OBJ := $(SRC:.c=.o)	# Object files from .c files
CORE_DEP := $(OBJ:.o=.d)	# Dependency lists from object files

# Make object files for all .c files
# OBJ := $(SRC:.c=.o)

# Generate dependency lists
# DEP := $(OBJ:.o=.d)

# Generate object files (compile)
%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $< 

# Create the binary (link)
mash: $(CORE_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS) 

.PHONY: clean valgrind

# Clean directive
clean:
	$(RM) $(CORE_OBJ) $(CORE_DEP) mash

debug:
	$(MAKE) clean
	$(MAKE) DEBUG=1 mash

# Run the shell in Valgrind
valgrind: mash
	valgrind --leak-check=full \
			 --track-origins=yes \
			 --show-leak-kinds=all \
			 ./mash

-include $(CORE_DEP)