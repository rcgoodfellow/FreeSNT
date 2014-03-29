#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# FreeSNT - Free Synchrophasor Network Tools
#
# Copyright (c) The FreeSNT project. All rights reserved.
# BSD V3 License, see COPYRIGHT for details
#
# ~ry
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

CCFLAGS=-Wall -Wextra

.PHONY: all
all: libsntcommand.a command_tests

libsntcommand.a: command.o
	ar -rcs $@ $^

command.o: command.c command.h
	$(CC) $(CCFLAGS) -c $< -o $@

command_tests: command_tests.c command.h libsntcommand.a
	$(CC) $(CCFLAGS) $< -o $@ -L. -lsntcommand

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf *.a
	rm -rf command_tests

