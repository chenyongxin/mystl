#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include "stl.h"
#include "vec.h"
#include "Array.h"

using namespace std;

#define EPS 1e-9

void get_stl(const string fname, int& N, array2& v0, array2& v1, array2& v2, array2& normals){
	ifstream in;
	in.open(fname);
	string word;
	N = 0;
	while(!in.eof()){
		in >> word;
		if(word=="facet") N++;
	}	

	v0.Allocate(N, 3);
	v1.Allocate(N, 3);
	v2.Allocate(N, 3);
	normals.Allocate(N, 3);

	in.clear();
	in.seekg(0);

	int i = -1;
	int n =  0;
	while(!in.eof()){
		in >> word;
		if(word == "facet" ) i++;
		if(word == "normal"){for(int j=0; j<3; j++) in >> normals(i,j);}
		if(word == "vertex"){
			for(int j=0; j<3; j++) {
				if(n==0)      in >> v0(i, j);
				else if(n==1) in >> v1(i, j);
				else          in >> v2(i, j);
			}
			if(++n==3) n = 0;	
		}
	}
}


bool inside(const Vec& v0, const Vec& v1, const Vec& v2, const Vec& n, const Vec& p){
	Vec u = v1 - v0;
	Vec v = v2 - v0;
	Vec w =  p - v0;
	double gamma = u.cross(w).dot(n);
	double beta  = w.cross(v).dot(n);
	double alpha = 1 - gamma - beta;
	return (0<=alpha) & (alpha<=1) & (0<=beta) & (beta<=1) & (0<=gamma) & (gamma<=1);
}


double project_distance(const Vec& v, const Vec& n, const Vec& p){
	return abs((p-v).dot(n));
}


Vec projected_point(const Vec& v, const Vec& n, const Vec& p){
	return p-n*(p-v).dot(n);}


bool ray_intersects_triangle(const Vec& v0, const Vec& v1, const Vec& v2, 
														 const Vec& o, const Vec& d, Vec& intersection){
	Vec edge1 = v1 - v0;
	Vec edge2 = v2 - v0;
	Vec h = d.cross(edge2);
	double a = edge1.dot(h);
	if(abs(a)<EPS) return false;
	double f = 1./a;
	Vec s = o - v0;
	double u = f*s.dot(h);
	if((u<0) | (u>1)) return false;
	Vec q = s.cross(edge1);
	double v = f*d.dot(q);
	if((v<0) | (u+v>1)) return false;
	double t = f*edge2.dot(q);
	if(t>EPS){
		intersection = o+d*t;
		return true;
	}
	else
		return false;
}


double distance_from_point_to_segment(const Vec& v1, const Vec& v2, const Vec& p, 
		const Vec& pp, Vec& ppp){

	Vec v1v2 = v2 - v1;
	Vec ppv2 = v2 - pp;
	Vec ppv1 = v1 - pp;

	Vec vd        = ppv2.cross(ppv1).cross(v1v2);   // direction vector of pp and ppp
	double lppv1  = ppv1.norm2();                   // length of ppv1
	double lvd    = vd.norm2();
	if(lvd > EPS){
		double cosr   = ppv1.dot(vd) / (lppv1*lvd);
		double lppppp = abs(lppv1*cosr);              // length of pp->ppp
		Vec ppppp     = vd*(lppppp/lvd);              // vector pp->ppp
		ppp           = pp + ppppp;                   // temporary closest point 
	}
	else{    // collinear
		ppp = pp;
	}
	Vec v1ppp = ppp-v1;

	// get collinear factor, be careful to 0 entry
	double t;
	if(v1v2.x != 0){
		t = v1ppp.x/v1v2.x;
	}
	else if(v1v2.y != 0){
		t = v1ppp.y/v1v2.y;
	}
	else{
		t = v1ppp.z/v1v2.z;
	}

	// correct closest point
	if(t<0)	ppp = v1;
	else if(t>1) ppp = v2;

	// return distance
	return (ppp-p).norm2();
}


double distance_from_point_to_triangle(
	const Vec& v0, const Vec& v1, const Vec& v2, 
	const Vec& n,  const Vec& p,  Vec& ppp){
	Vec pp = projected_point(v0, n, p);      // project point to triangle plane
	if(inside(v0, v1, v2, n, p)){
		ppp = pp;
		return project_distance(v0, n, p);
	}
	else{
		double d = 1e9, d1;
		Vec temp;
		d1 = distance_from_point_to_segment(v0, v1, p, pp, temp);
		if(d1<d){
			d = d1;
			ppp = temp;
		}
		d1 = distance_from_point_to_segment(v1, v2, p, pp, temp);
		if(d1<d){
			d = d1;
			ppp = temp;
		}
		d1 = distance_from_point_to_segment(v2, v0, p, pp, temp);
		if(d1<d){
			d = d1;
			ppp = temp;
		}
		return d;
	}
}

double distance_from_point_to_stl(const array2& v0, const array2& v1, const array2& v2,
	const array2& normals, const int N, const Vec& p, Vec& ppp){
	int n_intersects = 0;
	double dis = 1e9;
	Vec dir(0,0,1);
	Vec temp;
	std::vector<Vec> intersections;

	for(int i=0; i<N; i++){

		// Get vertices and normal vector
		Vec p0(v0(i,0), v0(i,1), v0(i,2));
		Vec p1(v1(i,0), v1(i,1), v1(i,2));
		Vec p2(v2(i,0), v2(i,1), v2(i,2));
		Vec  n(normals(i,0), normals(i,1), normals(i,2));

		double d = distance_from_point_to_triangle(p0, p1, p2, n, p, temp);
		if(d < dis){
			dis = d;
			ppp = temp;
		}

		Vec intersection;
		if(ray_intersects_triangle(p0, p1, p2, p, dir, intersection)){
			intersections.push_back(intersection);
		}
	}

	
	for(int i=0; i<intersections.size(); i++){
		bool distinct = true;
		for(int j=0; j<i; j++){
			distinct = (intersections[i]-intersections[j]).norm2() > EPS;
			if(!distinct) break;
		}
	  if(distinct) n_intersects += 1;
	}
	int coeff = n_intersects%2==0?1:-1;
	return coeff*dis;
}
