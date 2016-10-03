#ifndef VERTEX_3_H

#define VERTEX_3_H
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

class vertex_3
{
public:
	float x;
	float y;
	float z;

	vertex_3(void);
// Add a pair of vertex_3 objects
	const vertex_3& operator+(const vertex_3 &) const;

// Is this equal to that?
	bool operator==(const vertex_3 &) const;
// Subtract vertex_3 from vertex_3
        const vertex_3& operator-(const vertex_3 &) const;
// Is this smaller than that?
	bool operator<(const vertex_3 &) const;
//Multiply a pair of vertex_3s together, I thinks.
	 const float dot(vertex_3 &) const;
// Square myself
	 const float self_dot(void) const;
//Sqrt myself squared ?
	 const float length(void) const;

// Divide by x,y,z by length(void).
	 void normalize(void);

// Define a crossing vertex 3 between the other guy and me?
	 const vertex_3& cross(const vertex_3 &) const;


};

#endif
