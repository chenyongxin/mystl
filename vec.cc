#include <iostream>
#include <cmath>
#include "vec.h"

using namespace std;

Vec::Vec(){}

Vec::Vec(double a, double b, double c):x(a), y(b), z(c){}

double Vec::dot(const Vec& b) const {return x*b.x + y*b.y + z*b.z;}

Vec Vec::cross(const Vec& b) const {return Vec(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);}

Vec Vec::normal(){
	double s = 1./sqrt(x*x+y*y+z*z);
	return Vec(x*s, y*s, z*s);
}

double Vec::norm2() const {return sqrt(x*x+y*y+z*z);}
    
Vec Vec::operator+(const Vec& b) const {return Vec(x+b.x, y+b.y, z+b.z);}

void Vec::operator+=(const Vec& b){*this = Vec(x+b.x, y+b.y, z+b.z);}

Vec Vec::operator-(const Vec& b) const {return Vec(x-b.x, y-b.y, z-b.z);}

void Vec::operator-=(const Vec& b){*this = Vec(x-b.x, y-b.y, z-b.z);}

Vec Vec::operator*(const Vec& b) const {return Vec(x*b.x, y*b.y, z*b.z);}

void Vec::operator*=(const Vec& b){*this = Vec(x*b.x, y*b.y, z*b.z);}

Vec Vec::operator*(const double b) const {return Vec(x*b, y*b, z*b);}

void Vec::operator*=(const double b){*this = Vec(x*b, y*b, z*b);}

Vec Vec::operator/(const Vec& b) const {return Vec(x/b.x, y/b.y, z/b.z);}

void Vec::operator/=(const Vec& b){*this = Vec(x/b.x, y/b.y, z/b.z);}

void Vec::print(){cout << x << " " << y << " " << z << endl;}
