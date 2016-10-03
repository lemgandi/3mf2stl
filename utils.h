// #include <sys/stat.h>
#ifndef UTILS_H
#define UTILS_H
/*
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

 */
#include <vector>

using std::vector;

#include "triangle.h"
#include "vertex_3.h"

bool load_from_stl(vector<triangle>, const char* const);

    
bool save_to_ASCII_stereo_lithography_file(const vector<triangle> &, const char *const);


bool save_to_binary_stereo_lithography_file(const vector<triangle> &, const char *const, const size_t);


// http://stackoverflow.com/questions/10440113/simple-way-to-unzip-a-zip-file-using-zlib
char* libzip_zip_extract_archive_file_to_heap_terminate_with_null(const char*, const char*, size_t*, size_t);

#endif


