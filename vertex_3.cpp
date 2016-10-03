#include "vertex_3.h"
#include <cmath>
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

vertex_3::vertex_3(void)
{
   x = y = z = 0;
}

// Add to another vertex_3 object.
const vertex_3& vertex_3::operator+(const vertex_3 &right) const
{
		static vertex_3 temp;

		temp.x = this->x + right.x;
		temp.y = this->y + right.y;
		temp.z = this->z + right.z;

		return temp;
}

bool vertex_3::operator==(const vertex_3& right) const
{
	if (right.x == x && right.y == y && right.z == z)
			return true;
		else
			return false;
}

bool vertex_3::operator<(const vertex_3 & right) const
	{
		if(x < right.x)
			return true;
		else if(x > right.x)
			return false;

		if(y < right.y)
			return true;
		else if(y > right.y)
			return false;

		if(z < right.z)
			return true;
		else if(z > right.z)
			return false;

		return false;
	}


const vertex_3& vertex_3::operator-(const vertex_3 &right) const
	{
		static vertex_3 temp;

		temp.x = this->x - right.x;
		temp.y = this->y - right.y;
		temp.z = this->z - right.z;

		return temp;
	}

const float vertex_3::dot(vertex_3 &rhs) const
	{
		return x*rhs.x + y*rhs.y + z*rhs.z;
	}

const float vertex_3::self_dot(void) const
	{
		return x*x + y*y + z*z;
	}
const float vertex_3::length(void) const
	{
		return sqrt(self_dot());
	}


void vertex_3::normalize(void)
	{
		float len = length();

		if (0.0f != len)
		{
			x /= len;
			y /= len;
			z /= len;
		}
	}

const vertex_3& vertex_3::cross(const vertex_3 &right) const
	{
		static vertex_3 temp;

		temp.x = y*right.z - z*right.y;
		temp.y = z*right.x - x*right.z;
		temp.z = x*right.y - y*right.x;

		return temp;
	}


