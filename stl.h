#ifndef STL_H
#define STL_H

#include <string>
#include "Array.h"
#include "vec.h"

using namespace std;

typedef Array::array2<double> array2;

// Get STL info.
// fname: file name
// N: number of triangles
// v0, v1, v2: vertices
// normals: unit normal vectors
void get_stl(const string fname, int& N, array2& v0, array2& v1, array2& v2, 
		array2& normals);

// Test if a point p is inside a triangle surrounded by v0, v1 and v2 with normal 
// vector n.
bool inside(const Vec& v0, const Vec& v1, const Vec& v2, const Vec& n, const Vec& p);

// Distance of projected point p to point v with normal vector n.
double project_distance(const Vec& v, const Vec& p, const Vec& n);

// Möller–Trumbore intersection algorithm
// v0, v1, v2 are vertices of the triangle
// o is origin point
// d is direction vector
// intersection is the intersection point coordinate
bool ray_intersects_triangle(const Vec& v0, const Vec& v1, const Vec& v2, 
	const Vec& o, const Vec& d, Vec& intersection);

// Project a point p to a triangle plane with a vertex point v and normal vector n.
// v is the coordinate of any point on the plane.
Vec projected_point(const Vec& v, const Vec& n, const Vec& p);


// Compute distance from a point p to a segment v1-v2.
// Point pp is projected point from point p onto the plane of v1-v2.
// Closest point from a segment to the projected point pp is either vertex or edge.
// ppp is the closest point on the segment.
double distance_from_point_to_segment(const Vec& v1, const Vec& v2, 
																			const Vec& p,  const Vec& pp, Vec& ppp);


// Compute distance from a point p to triangle surrounded by v0, v1 and v2, with 
// unit normal vector n. Also returns the closest point ppp.
double distance_from_point_to_triangle(const Vec& v0, const Vec& v1, const Vec& v2, 
																			 const Vec& n,  const Vec& p,  Vec& ppp);

// Signed distance function from a point to STL.
double distance_from_point_to_stl(const array2& v0, const array2& v1, const array2& v2,
																	const array2& normals, const int N, const Vec& p, Vec& ppp);

#endif // STL_H