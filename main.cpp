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
#include <getopt.h>

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
#include "version.h"



// g++ *.cpp -I/usr/local/Cellar/libzip/1.1.2/include -I/usr/local/Cellar/libzip/1.1.2/lib/libzip/include -L/usr/local/Cellar/libzip/1.1.2/lib -lzip




#include <zip.h>
#include "tinyxml2.h"
//#include "main.h"
#include "utils.h"
using namespace tinyxml2;

void usage(char *pname)
{
   cout << "Usage: " << pname << " [-a] -i infile -o outfile" << endl << endl;
   cout << "Convert the .3mf file infile to the stl file outfile." << endl;
   cout << "The optional -a flag will create an ASCII (human readable) STL file." << endl;
   cout << endl;
   cout << "Copyright Charles Shapiro,Shawn Halayka Oct 2016" << endl;
   cout << "Version " << Major_Version << "." << Point_Version << endl;
}

int main(int argc, char **argv)
{
   int getopt_continue = 0;
   int ascii_output_flag = 0;
   int longidx = 0;
   char infilename[4097];
   char outfilename[4097];

   memset(infilename,0,sizeof(infilename));
   memset(outfilename,0,sizeof(outfilename));

   static struct option longopts [] = {
      {"ascii",no_argument,&ascii_output_flag,1},
      {"infile",required_argument,0,'i'},
      {"outfile",required_argument,0,'o'},
      {0,0,0,0}
   };

      
   while ((getopt_continue = getopt_long(argc,argv,"ai:o:",longopts,&longidx)) != -1)
      {
         switch (getopt_continue) {
	 case 0:
            break;
	 case 'i':
	    strncpy(infilename,optarg,sizeof(infilename)-1);
	    break;
	 case 'o':
	    strncpy(outfilename,optarg,sizeof(outfilename)-1);
	    break;
	 default:
	    usage(argv[0]);
	    exit(-1);

	 }
      }
   if(0 == strlen(infilename))
   {
      usage(argv[0]);
      exit(-2);
   }
   if(0 == strlen(outfilename))
   {
      usage(argv[0]);
      exit(-2);
   }

   size_t s = 0;

   // get char array, automatically terminated with a \0.
   char *v = libzip_zip_extract_archive_file_to_heap_terminate_with_null(infilename, "3D/3dmodel.model", &s, 0);

	if(0 == v)
        return -1;

	// triangles for output to stl
	vector<triangle> triangles;
	
	// pass xml file as a \0 terminated c-string
	XMLDocument doc;
	doc.Parse(v);
	
	// Release memory related to \0 terminated c-string
    delete [] v;

	// Parse the XML file
	XMLElement* model = doc.FirstChildElement("model");

	if(model)
	{
		XMLElement* resources = model->FirstChildElement("resources");

		if(resources)
		{
			XMLElement* object = resources->FirstChildElement("object");

			while(object)
			{
				XMLElement *mesh = object->FirstChildElement("mesh");

				if(mesh)
				{
					vector<vertex_3> vertices;

					XMLElement *verts = mesh->FirstChildElement("vertices");

					if(verts)
					{
						XMLElement* vertex_child = verts->FirstChildElement("vertex");

						while(vertex_child)
						{
							vertex_3 vertex;

							istringstream iss;
                            iss.str(vertex_child->Attribute("x"));
							iss >> vertex.x;
							
							iss.clear();
							iss.str(vertex_child->Attribute("y"));
							iss >> vertex.y;
							
							iss.clear();
							iss.str(vertex_child->Attribute("z"));
							iss >> vertex.z;

							vertices.push_back(vertex);

							vertex_child = vertex_child->NextSiblingElement("vertex");
						}
					}

					XMLElement *tris = mesh->FirstChildElement("triangles");

					if(tris)
					{
						XMLElement* tris_child = tris->FirstChildElement("triangle");

						while(tris_child)
						{
							triangle tri;
							size_t index;

							istringstream iss;
							iss.str(tris_child->Attribute("v1"));
							iss >> index;
							tri.vertex[0] = vertices[index];

							iss.clear();
							iss.str(tris_child->Attribute("v2"));
							iss >> index;
							tri.vertex[1] = vertices[index];

							iss.clear();
							iss.str(tris_child->Attribute("v3"));
							iss >> index;
							tri.vertex[2] = vertices[index];

							triangles.push_back(tri);

							tris_child = tris_child->NextSiblingElement("triangle");
						}
					}
				}

				object = object->NextSiblingElement( "object" );
			}
		}
	}

	cout << "Read " << triangles.size() << " triangles..." << endl;

	// Write triangles to STL file...
      
  if(ascii_output_flag)
     save_to_ASCII_stereo_lithography_file(triangles, outfilename);
  else
     save_to_binary_stereo_lithography_file(triangles, outfilename, 65535);
 
    
    
	return 1;
}



