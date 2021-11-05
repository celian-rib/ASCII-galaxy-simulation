# Name of the main target (= executable name)
TARGET = galaxy

# Compiler command to use
CC = gcc
# Flags to add to the compiler command
CFLAGS = --std=c11 -g -Wall -Wextra -Wunused -Wunused -pedantic -D_XOPEN_SOURCE=700
# Ncurse specific flag
LDFLAGS=-lncurses

# Path to the project source
SOURCEDIR = ./source
# Subdirectories in the project source /!\ NOT WORKING RECURSIVELY
SUBDIR = $(shell find $(SOURCEDIR) -maxdepth 1 -type d)
# Directorie that will contains all objects (.o) files
DIR_OBJ = ./obj

# Find all .h files in the projects
HEADERS = $(wildcard *.h $(foreach fd, $(SUBDIR), $(fd)/*.h))
# Find all .c files in the projects
SOURCES = $(wildcard *.c $(foreach fd, $(SUBDIR), $(fd)/*.c))

# Create all objects path relative to the makefile
OBJECTS = $(addprefix $(DIR_OBJ)/, $(SOURCES:c=o))
# Add -I in front of all project subdirectories (-I = include in the compilation)
INC_DIRS = -I./ $(addprefix -I, $(SUBDIR))

PHONY := $(TARGET)

# Main target for all objects
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)

# Generic target for all .o files
$(DIR_OBJ)/%.o: %.c $(HEADERS)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ -c $< $(INC_DIRS)

PHONY += clean
# Clean target to remove all generated files
clean:
	rm -rf $(TARGET) $(DIR_OBJ)/*

# Debug target to check this makefile variables
PHONY += debug
debug:
	@echo "HEADERS: $(HEADERS)"
	@echo "SOURCES: $(SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"
	@echo "INC_DIRS: $(INC_DIRS)"
	@echo "SOURCEDDIR: $(SOURCEDIR)"
	@echo "SUBDIR: $(SUBDIR)"

.PHONY = $(PHONY)


#############################################################################
# Mostly made thanks to:
# https://yuukidach.github.io/p/makefile-for-projects-with-subdirectories/
#############################################################################
