
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

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cstring>
#include <ios>
#include <cmath>
#include <set>
#include <sstream>
#include <iomanip>
#include <string>
#include <zip.h>
#include <errno.h>

using std::string;
using std::setprecision;
using std::ostringstream;
using std::istringstream;
using std::set;
using std::sqrt;
using std::memcpy;
using std::vector;
using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::ios_base;
using std::hex;
using std::strerror;

#include "vertex_3.h"
#include "triangle.h"


bool load_from_stl(vector<triangle> &triangles, const char* const file_name)
{
	triangles.clear();

	// Read from file.
	ifstream in(file_name, ios_base::binary);

	if (in.fail())
		return false;

	const size_t header_size = 80;
	vector<char> buffer(header_size, 0);
	unsigned int num_triangles = 0; // Must be 4-byte unsigned int.

	// Read header.
	in.read(reinterpret_cast<char *>(&(buffer[0])), header_size);

    if (header_size != in.gcount())
		return false;

	// If ASCII text file header, abort
	if ('s' == tolower(buffer[0]) &&
		'o' == tolower(buffer[1]) &&
		'l' == tolower(buffer[2]) &&
		'i' == tolower(buffer[3]) &&
		'd' == tolower(buffer[4]))
	{
		cout << "Encountered ASCII STL file header -- aborting" << endl;
		return false;
	}



	// Read number of triangles.
	in.read(reinterpret_cast<char *>(&num_triangles), sizeof(unsigned int));

	if (sizeof(unsigned int) != in.gcount())
		return false;

	triangles.resize(num_triangles);

	// Enough bytes for twelve 4-byte floats plus one 2-byte integer, per triangle.
	const size_t data_size = (12 * sizeof(float)+sizeof(short unsigned int)) * num_triangles;
	buffer.resize(data_size, 0);

	in.read(reinterpret_cast<char *>(&buffer[0]), data_size);

	if (data_size != in.gcount())
		return false;

	// Use a pointer to assist with the copying.
	// Should probably use std::copy() instead, but memcpy() does the trick, so whatever...
	char *cp = &buffer[0];

	for (vector<triangle>::iterator i = triangles.begin(); i != triangles.end(); i++)
	{
		// Skip face normal.
		cp += 3 * sizeof(float);

		// Get vertices.
		memcpy(&i->vertex[0].x, cp, sizeof(float)); cp += sizeof(float);
		memcpy(&i->vertex[0].y, cp, sizeof(float)); cp += sizeof(float);
		memcpy(&i->vertex[0].z, cp, sizeof(float)); cp += sizeof(float);
		memcpy(&i->vertex[1].x, cp, sizeof(float)); cp += sizeof(float);
		memcpy(&i->vertex[1].y, cp, sizeof(float)); cp += sizeof(float);
		memcpy(&i->vertex[1].z, cp, sizeof(float)); cp += sizeof(float);
		memcpy(&i->vertex[2].x, cp, sizeof(float)); cp += sizeof(float);
		memcpy(&i->vertex[2].y, cp, sizeof(float)); cp += sizeof(float);
		memcpy(&i->vertex[2].z, cp, sizeof(float)); cp += sizeof(float);

		// Skip attribute.
		cp += sizeof(short unsigned int);
	}

	in.close();

	return true;
}

    
bool save_to_ASCII_stereo_lithography_file(const vector<triangle> &triangles, const char *const file_name)
{
    if(0 == triangles.size())
        return false;
    
    // Write to file.
    ofstream out(file_name);
    
    if(out.fail())
        return false;
   
    out << "solid triangles" << endl;
    out << setprecision(16);
    
    for(size_t i = 0; i < triangles.size(); i++)
    {
        vertex_3 a = triangles[i].vertex[2] - triangles[i].vertex[0];
        vertex_3 b = triangles[i].vertex[1] - triangles[i].vertex[0];
        vertex_3 normal = a.cross(b);
        normal.normalize();
        
        out << "  facet normal " << normal.x << ", " << normal.y << ", " << normal.z << endl;
        out << "    outer loop" << endl;
        
        out << "      vertex " << triangles[i].vertex[0].x << ' ' << triangles[i].vertex[0].y << ' ' << triangles[i].vertex[0].z << endl;
        out << "      vertex " << triangles[i].vertex[1].x << ' ' << triangles[i].vertex[1].y << ' ' << triangles[i].vertex[1].z << endl;
        out << "      vertex " << triangles[i].vertex[2].x << ' ' << triangles[i].vertex[2].y << ' ' << triangles[i].vertex[2].z << endl;
        
        out << "    endloop" << endl;
        out << "  endfacet" << endl;
    }
    
    out << "endsolid" << endl;
    
    
    return true;
}


bool save_to_binary_stereo_lithography_file(const vector<triangle> &triangles, const char *const file_name, const size_t buffer_width)
{
	if(0 == triangles.size())
		return false;

	// Write to file.
	ofstream out(file_name, ios_base::binary);

	if(out.fail())
		return false;

	const size_t header_size = 80;
	vector<char> buffer(header_size, 0);
	const unsigned int num_triangles = triangles.size(); // Must be 4-byte unsigned int.
	vertex_3 normal;

	// Write blank header.
	out.write(reinterpret_cast<const char *>(&(buffer[0])), header_size);

	// Write number of triangles.
	out.write(reinterpret_cast<const char *>(&num_triangles), sizeof(unsigned int));

	// Enough bytes for twelve 4-byte floats plus one 2-byte integer, per triangle.
	const size_t per_triangle_data_size = (12*sizeof(float) + sizeof(short unsigned int));
	const size_t buffer_size = per_triangle_data_size * buffer_width;
	buffer.resize(buffer_size, 0);

	// Use a pointer to assist with the copying.
	// Should probably use std::copy() instead, but memcpy() does the trick, so whatever...
	char *cp = &buffer[0];
	size_t buffer_count = 0;
        float written_mb = (per_triangle_data_size * triangles.size()) / 1048576.0;

	cout << "Writing " << written_mb << " MB of data to disk" << endl;

	for(size_t i = 0; i < triangles.size(); i++)
	{
        vertex_3 a = triangles[i].vertex[2] - triangles[i].vertex[0];
        vertex_3 b = triangles[i].vertex[1] - triangles[i].vertex[0];
        vertex_3 normal = a.cross(b);
        normal.normalize();
        
		memcpy(cp, &normal.x, sizeof(float)); cp += sizeof(float);
		memcpy(cp, &normal.y, sizeof(float)); cp += sizeof(float);
		memcpy(cp, &normal.z, sizeof(float)); cp += sizeof(float);

		memcpy(cp, &triangles[i].vertex[0].x, sizeof(float)); cp += sizeof(float);
		memcpy(cp, &triangles[i].vertex[0].y, sizeof(float)); cp += sizeof(float);
		memcpy(cp, &triangles[i].vertex[0].z, sizeof(float)); cp += sizeof(float);
		memcpy(cp, &triangles[i].vertex[1].x, sizeof(float)); cp += sizeof(float);
		memcpy(cp, &triangles[i].vertex[1].y, sizeof(float)); cp += sizeof(float);
		memcpy(cp, &triangles[i].vertex[1].z, sizeof(float)); cp += sizeof(float);
		memcpy(cp, &triangles[i].vertex[2].x, sizeof(float)); cp += sizeof(float);
		memcpy(cp, &triangles[i].vertex[2].y, sizeof(float)); cp += sizeof(float);
		memcpy(cp, &triangles[i].vertex[2].z, sizeof(float)); cp += sizeof(float);

		cp += sizeof(short unsigned int);

		buffer_count++;

		// If buffer is full, write triangles in buffer to disk.
		if(buffer_count == buffer_width)
		{
			out.write(reinterpret_cast<const char *>(&buffer[0]), buffer_size);

			if(out.fail())
				return false;

			buffer_count = 0;
			cp = &buffer[0];
		}
	}

	// Write any remaining triangles in buffer to disk.
	// This will occur whenever triangles.size() % buffer_width != 0
	// (ie. when triangle count is not a multiple of buffer_width, which should happen almost all of the time).
	if(buffer_count > 0)
	{
		out.write(reinterpret_cast<const char *>(&buffer[0]), per_triangle_data_size*buffer_count);

		if(out.fail())
			return false;
	}

	out.close();

	return true;
}




// http://stackoverflow.com/questions/10440113/simple-way-to-unzip-a-zip-file-using-zlib
char* libzip_zip_extract_archive_file_to_heap_terminate_with_null(const char* zip_filename, const char* compressed_file_name, size_t* file_size, size_t flags)
{
    //Open the ZIP archive
//    int err = 0;
    int zip_error = 0;
    int sys_error = 0;
    char err_str[80];

    memset(err_str,0,sizeof(err_str));
    zip *z = zip_open(zip_filename, 0, &zip_error);
    if(0 == z)
    {
       if(errno != 0)
          strncpy(err_str,strerror(errno),sizeof(err_str)-1);
       else
          zip_error_to_str(err_str,sizeof(err_str)-1,zip_error,sys_error);
       cout << "Error on " << "[" << zip_filename << "]" << " : " << err_str << endl;
        return 0;
    }
    
    //Search for the file of given name
    const char *name = compressed_file_name;
    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(z, name, 0, &st);
    
    //Alloc memory for its uncompressed contents
    char *contents = new char[st.size + 1];
    *file_size = st.size + 1;
    
    //Read the compressed file
    zip_file *f = zip_fopen(z, compressed_file_name, 0);
    
    if(0 == f)
    {
         zip_error_get(z,&zip_error,&sys_error);
         if (sys_error != 0)
            strncpy(err_str,strerror(sys_error),sizeof(err_str)-1);
         else
	    zip_error_to_str(err_str,sizeof(err_str)-1,zip_error,sys_error);
        cout << "Compressed file trouble: " << "[" << compressed_file_name << "]" << err_str  << endl;
        delete [] contents;
        zip_close(z);
        
        return 0;
    }
    
    zip_fread(f, contents, st.size);
    zip_fclose(f);
    
    //And close the archive
    zip_close(z);
    
    contents[st.size] = '\0';
    
    return contents;
}




