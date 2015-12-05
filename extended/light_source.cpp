/***********************************************************
Starter code for Assignment 3

This code was originally written by Jack Wang for
CSC418, SPRING 2005

implements light_source.h

***********************************************************/

#include <cmath>
#include "light_source.h"

#define RADIUS 1 // sphere radius

void PointLight::shade( Ray3D& ray ) {
  shade(ray, false);
}
void PointLight::shade( Ray3D& ray, bool shadow ) {
  // TODO: implement this function to fill in values for ray.col
  // using phong shading.  Make sure your vectors are normalized, and
  // clamp colour values to 1.0.
  //
  // It is assumed at this point that the intersection information in ray
  // is available.  So be sure that traverseScene() is called on the ray
  // before this function.

  // intersection normal
  if(!ray.intersection.none) {
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

    //TODO: TEXTURE
    if (TEXTURE && ray.intersection.mat->texture) {
      // The square is textured
      // Convert to pixels
      int x = ray.intersection.point[0];
      int y = ray.intersection.point[1];
      int i = y * ray.intersection.mat->texture_width + x;
      int r = (int) *(ray.intersection.mat->rarray + i);
      int g = (int) *(ray.intersection.mat->garray + i);
      int b = (int) *(ray.intersection.mat->barray + i);
      ray.col = Colour(r / 255.0, g / 255.0, b / 255.0);
    } else {
      Colour ambient = ray.intersection.mat->ambient *_col_ambient;

      Colour diffuse = ray.intersection.mat->diffuse * (std::max(0.0, N.dot(L)) * _col_diffuse);

      double intensity = std::max(0.0, std::pow(D.dot(R),ray.intersection.mat->specular_exp));
      Colour specular = ray.intersection.mat->specular * (intensity * _col_specular);
      //omit diffuse and specular terms from the local Phong model
      if (shadow) ray.col = ambient;
      //Phong
      else ray.col = diffuse + ambient + specular;
    }
    ray.col.clamp();
  }


}
