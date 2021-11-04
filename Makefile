CC = gcc
CFLAGS = --std=c11 -g -Wall -Wextra -Wunused -Wunused -pedantic -D_XOPEN_SOURCE=700
LDFLAGS=-lncurses

SRC := ./source
# OBJ := ./obj

# SOURCES = $(shell find $(SRCDIR) -type f -name "*.c*")
# HEADERS = $(shell find $(SRCDIR) -type f -name "*.h*")

SOURCES := $(wildcard $(SRC)/*.c)
OBJECTS := ./source/body/body.o ./source/simulation/simulation.o ./source/main.o

main: ./source/body/body.o ./source/simulation/simulation.o ./source/main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(OBJECTS)

source/body.o: ./source/body/body.c ./source/body/body.h
	$(CC) $(CFLAGS) $(LDFLAGS) -I./source/utils -c ./source/body/body.c

source/simulation.o: ./source/simulation/simulation.c ./source/simulation/simulation.h
	$(CC) $(CFLAGS) $(LDFLAGS) -I./source/utils -I./source/body -c ./source/simulation/simulation.c






# all: $(OBJECTS)
# 	$(CC) $^ -o $@

# $(OBJ)/%.o: $(SRC)/%.c
# 	$(CC) -I$(SRC) -c $< -o $@

# OBJECTS := $(patsubst $(SRCDIR)/%.c, %.o, $(SOURCES))

# all: $(OBJECTS)
# 	$(CC) $^ -o $@

# %.o: $(SRCDIR)/%.c
# 	$(CC) -I$(SRCDIR) -c $< -o $@

# debug:
# 	$(call echo, "SOURCES : $(SOURCES)")
# 	$(call echo, "HEADERS : $(HEADERS)")
# 	$(CC) -c $(SOURCES)

clean:
	rm -rf *.o main

# $(TARGET): $(BINDIR) $(BUILDDIR) $(OBJECTS)
# 	@$(CC) $(OBJECTS) -o $(TARGET) $(LIB)
# 	$(call print_green,"$(TARGET) has been created!")

# $(BUILDDIR) :
# 	mkdir $(BUILDDIR)

# $(BINDIR):
# 	mkdir $(BINDIR)
	
# $(BUILDDIR)/%.o: $(SRCDIR)/%.c*
# 	@mkdir -p $(dir $@)
# 	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
# 	@$(CC) $(CFLAGS) $(INC) -M $< -MT $@ > $(@:.o=.td)
# 	@cp $(@:.o=.td) $(@:.o=.d); 
# 	@sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
# 	-e '/^$$/ d' -e 's/$$/ :/' < $(@:.o=.td) >> $(@:.o=.d); 
# 	@rm -f $(@:.o=.td)