
#   This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.


# This makefile runs correctly on Debian 3.16.7 with gcc 4.9.2 and libzip-dev installed.
# To install libzip dev on debian systems:
# su
# <enter password>
# apt-cache update
# <Lots of stuff follows>
# apt-get install libzip-dev
# To make the executable:
# Copy this file into the directory you unzipped the "cp13.zip" source code into
# At the command line in this directory, type "make".  It should compile without errors
# and leave you with a "3mf2stl" executable. Use "make test" to verify that it works.

# Charles Shapiro 3 Oct 2016
#

GCC=gcc
GLN=g++
EXEC=3mf2stl
TESTOUT=Lion_Cookie_Cutter.stl

top: $(EXEC)

test: $(EXEC) ./convert_to_stl.sh Lion_Cookie_Cutter.3mf
	./convert_to_stl.sh Lion_Cookie_Cutter.3mf 



clean:
	rm -f $(EXEC) *.o $(TESTOUT)

$(EXEC) : main.o tinyxml2.o triangle.o vertex_3.o utils.o
	$(GLN) -o $(EXEC) -lzip main.o tinyxml2.o triangle.o vertex_3.o utils.o

main.o: main.cpp 
	$(GCC) -c  -o main.o main.cpp

tinyxml2.o : tinyxml2.cpp
	$(GCC) -c -o tinyxml2.o tinyxml2.cpp


vertex_3.o : vertex_3.cpp vertex_3.h
	$(GCC) -c -o vertex_3.o vertex_3.cpp

triangle.o : triangle.cpp triangle.h
	$(GCC) -c -o triangle.o triangle.cpp

utils.o : utils.cpp utils.h
	$(GCC) -c -o utils.o utils.cpp