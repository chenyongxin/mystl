#include <iostream>
#include <fstream>
#include "Array.h"
#include "vec.h"
#include "stl.h"

using namespace std;


int main(){
  int N;         // Number of triangles	
  array2 v0, v1, v2, normals;
	
  get_stl("part1.stl", N, v0, v1, v2, normals);
	
  const int nx = 41, ny = 21, nz = 21;
  double dx = 1.2 /(nx-1);
  double dy = 0.2 /(ny-1);
  double dz = 0.62/(nz-1);
  double ox = -0.1, oy = -0.05, oz = -0.36;
  Vec ppp;
  
  std::ofstream out;

  out.open("x");
  for(int i=0; i<nx; i++) out << ox+i*dx << endl;
  out.clear();
  out.close(); 
	
  out.open("y");
  for(int j=0; j<ny; j++) out << oy+j*dy << endl;
  out.clear();
  out.close(); 

  out.open("z");
  for(int k=0; k<nz; k++) out << oz+k*dz << endl;
  out.clear();
  out.close(); 

  out.open("data");
  for(int k=0; k<nz; k++){
    for(int j=0; j<ny; j++){
      for(int i=0; i<nx; i++){
      	double x = ox+i*dx;
      	double y = oy+j*dy;
      	double z = oz+k*dz;
      	Vec p(x,y,z);
      	out << distance_from_point_to_stl(v0, v1, v2, normals, N, p, ppp) << endl;
      }
    }
  }
  out.clear();
  out.close(); 
}
