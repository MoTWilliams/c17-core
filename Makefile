# Use GCC compiler
CC := gcc

# Set default value of the debug flag
DEFAULT ?= 0
BUILDTYPE := $(if $(filter 1 true yes on,$(DEBUG)),debug,release)

# Enforce strict C89 standard, enable all common warnings, and set debug flag
CFLAGS += -std=c89 -pedantic -Wall -Wextra -g -DDEBUG=$(DEBUG) -MMD -MP

# Construct build trees
BUILDDIR := build
OBJDIR := $(BUILDDIR)/$(BUILDTYPE)/obj
DEPDIR := $(BUILDDIR)/$(BUILDTYPE)/dep

bin:
	@mkdir -p $@
.PHONY: bin

# --- ALL BUILDS ---
INCLUDE := $(addprefix -iquote , $(sort $(shell find src -type d)))
CPPFLAGS += $(INCLUDE)

SRC := $(sort $(shell find src -name '*.c'))

CORE_OBJDIR := $(OBJDIR)/core
CORE_OBJ := $(patsubst src/%.c, $(CORE_OBJDIR)/%.o, $(SRC))

CORE_DEPDIR := $(DEPDIR)/core
CORE_DEP := $(patsubst $(CORE_OBJDIR)/%.o, $(CORE_DEPDIR)/%.d, $(CORE_OBJ))

$(CORE_OBJDIR)/%.o: src/%.c
	@mkdir -p $(dir $@) \
		$(dir $(patsubst $(CORE_OBJDIR)/%.o, $(CORE_DEPDIR)/%.d, $@))
	$(CC) $(CPPFLAGS) $(CFLAGS) \
		-MF $(patsubst $(CORE_OBJDIR)/%.o, $(CORE_DEPDIR)/%.d, $@) -c -o $@ $<

# --- NORMAL BUILD ---
APP_INC := $(addprefix -iquote , $(sort $(shell find app -type d)))

APP := $(sort $(shell find app -name '*.c'))

APP_OBJDIR := $(OBJDIR)/app
APP_OBJ := $(patsubst app/%.c, $(APP_OBJDIR)/%.o, $(APP))

APP_DEPDIR := $(DEPDIR)/app
APP_DEP := $(patsubst $(APP_OBJDIR)/%.o, $(APP_DEPDIR)/%.d, $(APP_OBJ))

$(APP_OBJDIR)/%.o: CPPFLAGS += $(APP_INC)

$(APP_OBJDIR)/%.o: app/%.c
	@mkdir -p $(dir $@) \
		$(dir $(patsubst $(APP_OBJDIR)/%.o, $(APP_DEPDIR)/%.d, $@))
	$(CC) $(CPPFLAGS) $(CFLAGS) \
		-MF $(patsubst $(APP_OBJDIR)/%.o, $(APP_DEPDIR)/%.d, $@) -c -o $@ $<

# Create the executable
mash: bin/mash
.PHONY: mash

bin/mash: $(CORE_OBJ) $(APP_OBJ) | bin
	$(CC) -o $@ $^

# Include generated dependencies
-include $(CORE_DEP) $(APP_DEP)

.PHONY: clean release debug

clean:
	$(RM) -r $(BUILDDIR) bin

release:
	$(MAKE) DEBUG=0 mash
	./bin/mash

debug:
	$(MAKE) DEBUG=1 mash
	./bin/mash