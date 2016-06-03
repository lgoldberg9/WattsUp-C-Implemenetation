# File:          Makefile
# Author:        Logan Goldberg
# Created:       15 April 2015
# Simplified:    15 April 2015
# Updated:       2 June 2016
# Acknowledgement: adapted from an example by Henry M. Walker
#----------------------------------------------------------------------------
# Use the gcc compiler
CC = gcc

# Set appropriate compiling flags
CFLAGS = -ansi -g -Wall -std=c11 -lm

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

WattsUp: Main.c WattsUp.c ArrayList.c
	$(CC) $(CFLAGS) -o WattsUp Main.c WattsUp.c ArrayList.c

Main.o: Main.c WattsUp.h ArrayList.h Utilities.h
	$(CC) $(CFLAGS) -o Main.c

WattsUp.o: WattsUp.c WattsUp.h ArrayList.h Utilities.h
	$(CC) $(CFLAGS) -o WattsUp.c ArrayList.c

ArrayList.o: ArrayList.c ArrayList.h
	$(CC) $(CFLAGS) -o ArrayList.c

#----------------------------------------------------------------------------
# cleanup rules: To invoke this command, type "make clean".
# Use this target to clean up your directory, deleting (without warning) 
#   object files, old emacs source versions, and core dumps.

clean:
	rm -f *.o *~ core*

clean-exe:
	rm -f wordboard
