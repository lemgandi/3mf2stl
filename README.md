= 3mf2stl

This is a simple command-line utility to convert 3d ".3mf" files to the ".stl" files used by most 3d printer slicing
software.

== Installing

You will need a working c++ development environment to make this code. You must also install 
a zip development library, usually available from your repository. For Debian users, that's the "libzip-dev" package.
If you wish to use this program successfully, you'll also need "p7zip" or "p7zip-full".

Unzip the source code to an empty directory and type "make".  It compiles without errors or warnings on Debian 3.2 ("Sarge") and Debian 8.0 ("Jessie").   This will produce an executable called "3mf2stl".

== Using

The current version of the Debian zip libraries won't decompress ".3mf" files, apparently because of a file format 
dispute between the 3mf folks and the Debian folks.  The current work-around is to unzip the .3mf file with p7zip, then
re-zip it with the same utility.  The "convert_to_stl.sh" script automates this process, albeit with minimal error
handling.  If you type "make test" at the command line, make(1) will run the convert_to_stl.sh script to produce 
"Lion_Cookie_Cutter.stl".  You should be able to import this stl file into the 3d software of your choice and 
see a funny cookie cutter thing.

== License

This code is based on the work of Shawn Haylaka, which was released to the public domain. 

This code is Copyright(c) Charles Shapiro and Shawn Haylaka, October 2016.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

The "tinyxml2.cpp" and "tinyxml2.h" files used in this program were written by Lee Thomason; they have their
own licenses which are in the referenced source files. 
