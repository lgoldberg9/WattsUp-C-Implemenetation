# File:          Makefile
# Author:        Logan Goldberg
# Created:       15 April 2015
# Simplified:    15 April 2015
# Updated:       5 June 2016
# Acknowledgement: adapted from an example by Henry M. Walker
#----------------------------------------------------------------------------
# Use the gcc compiler
CC = gcc

# Set appropriate compiling flags
CFLAGS = -ansi -g -Wall -std=gnu99 -lm

# Source directory
SOURCE=./src/

# Binary directory
BIN=./bin/

#----------------------------------------------------------------------------
# build rules:
#
# Each rule takes the following form  (Note there MUST be a tab,
# as opposed to several spaces, preceeding each command.
#
# target_name:  dependency_list
#	command(s)

all: WattsUp

# List program components, what they depend on, and how to compile each

WattsUp: $(SOURCE)Main.c $(SOURCE)WattsUp.c $(SOURCE)Utilities.c
	$(CC) $(CFLAGS) -o $(BIN)WattsUp $(SOURCE)Main.c $(SOURCE)WattsUp.c $(SOURCE)Utilities.c

Main.o: Main.c WattsUp.h Utilities.h
	$(CC) $(CFLAGS) -o $(SOURCE)Main.c

WattsUp.o: WattsUp.c WattsUp.h Utilities.h
	$(CC) $(CFLAGS) -o $(SOURCE)WattsUp.c

Utilities.o: Utilities.c Utilities.h
	$(CC) $(CFLAGS) -o $(SOURCE)Utilities.c

#----------------------------------------------------------------------------
# cleanup rules: To invoke this command, type "make clean".
# Use this target to clean up your directory, deleting (without warning) 
#   object files, old emacs source versions, and core dumps.

clean:
	rm -f *.o *~ core*

clean-exe:
	rm -f wordboard
