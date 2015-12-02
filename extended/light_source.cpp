/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		implements light_source.h

***********************************************************/

#include <cmath>
#include "light_source.h"

#define RADIUS 1 // sphere radius
extern int width;
extern int height;
extern unsigned char* rbuffer;
extern unsigned char* gbuffer;
extern unsigned char* bbuffer;

void PointLight::shade( Ray3D& ray ) {
	// TODO: implement this function to fill in values for ray.col
	// using phong shading.  Make sure your vectors are normalized, and
	// clamp colour values to 1.0.
	//
	// It is assumed at this point that the intersection information in ray
	// is available.  So be sure that traverseScene() is called on the ray
	// before this function.

  // intersection normal
	Vector3D N = ray.intersection.normal;
  N.normalize();

  // light direction
  Vector3D L = _pos - ray.intersection.point;
  L.normalize();

  Vector3D R = 2 * L.dot(N) * N - L;
  R.normalize();

  // view direction
  Vector3D D = -ray.dir;
  D.normalize();

  //image mapping
  if(ray.intersection.mat->imageMap == 3)
    {
      Point3D itsPoint = ray.intersection.point; 
      Point3D centre = ray.intersection.CenterPoint; 
      double theta = acos((itsPoint[2]-centre[2])/RADIUS); 
      double phi = atan2(itsPoint[1]-centre[1],itsPoint[0]-centre[0]); 
      double u = phi/(double)(2.0*M_PI);
      double v = (M_PI - theta)/(double)M_PI; 
      u += 0.5; 
      u *= width; 
      v *= height; 
      int temp = floor(v)*width+floor(u); 

      ray.col[0] += (rbuffer[temp])/(double)255 ;
      ray.col[1] += (gbuffer[temp])/(double)255 ;
      ray.col[2] += (bbuffer[temp])/(double)255 ;

    }
    
    else if (ray.intersection.mat->imageMap == 2){
      Point3D itsPoint = ray.intersection.point; 
     
      Point3D centre = ray.intersection.CenterPoint; 
      double mag = 0;
      mag += itsPoint[0];
      mag = (mag * M_PI);
      double tanV = tan(mag); 

      double color = 30; 
      if (tanV >= 0) color = 1; 
      ray.col[0] = (color);
      mag = itsPoint[1];
      mag = (mag * M_PI);
      tanV = tan(mag); 

       color = 10; 
      if (tanV >= 0) color = 1;
      ray.col[1] = (color);
      mag = itsPoint[2];
      mag = (mag * M_PI); 

       tanV = tan(mag); 

       color = 20; 
      if (tanV >= 0) color = 1; 
      ray.col[2] = (color);
    }
    else{


  Colour ambient = ray.intersection.mat->ambient *_col_ambient;

  Colour diffuse = ray.intersection.mat->diffuse * (std::max(0.0, N.dot(L)) * _col_diffuse);

	double intensity = std::max(0.0, std::pow(D.dot(R),ray.intersection.mat->specular_exp));
  Colour specular = ray.intersection.mat->specular * (intensity * _col_specular);

  //signature
  //ray.col = ray.intersection.mat->diffuse;

  //diffuse and ambient
  //ray.col = diffuse + ambient;

  //Phong
  ray.col = diffuse + ambient + specular;
}

  ray.col.clamp();



}
