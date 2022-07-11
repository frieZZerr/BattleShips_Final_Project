# Makefile
# Compiler g++.
CC = g++
# Possible new variables
# LIBS = -ll -lm
LIBS =
DIR = `basename $(CURDIR)`
CFLAGS = -Wall -std=c++11
# Object files - created from *.cpp files.
OBJECTS1 = battleships.o
# Header files - attached to *.cpp files.
HFILES = battleships.h
# Executable file.
EXEC1 = battleships.x

# Default rule template:
# $< - first dependence file name.
# $@ - final file name.
# Every object file is dependent on header file.
%.o : %.cpp $(HFILES) Makefile
	$(CC) $(CFLAGS) -c $< -o $@

game: $(OBJECTS1)
	$(CC) $(CFLAGS) $(LIBS) $(OBJECTS1) -o $(EXEC1)
	./$(EXEC1)

# Declaring PHONIES
.PHONY : clean

clean :
	$(RM) $(TARGET) *.out *.o *.x core

# Archiving and compression
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
