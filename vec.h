#ifndef VEC_H
#define VEC_H

class Vec{
public:
	double x, y, z;

	Vec(double, double, double);
	Vec();

	double dot(const Vec& b) const;
	Vec cross(const Vec& b) const;
	Vec normal();                              // normalise
	double norm2() const;                      // 2nd norm
	Vec operator+(const Vec& b) const;         
	void operator+=(const Vec& b);            
	Vec operator-(const Vec& b) const;
	void operator-=(const Vec& b);
	Vec operator*(const Vec& b) const;
	void operator*=(const Vec& b);
	Vec operator*(const double) const;
	void operator*=(const double);
	Vec operator/(const Vec& b) const;
	void operator/=(const Vec& b);
	void print();

};

#endif // VEC_H