/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		implements light_source.h

***********************************************************/

#include <cmath>
#include "light_source.h"

void PointLight::shade( Ray3D& ray ) {
	// TODO: implement this function to fill in values for ray.col
	// using phong shading.  Make sure your vectors are normalized, and
	// clamp colour values to 1.0.
	//
	// It is assumed at this point that the intersection information in ray
	// is available.  So be sure that traverseScene() is called on the ray
	// before this function.

   //intersection normal
	Vector3D N = ray.intersection.normal;
    N.normalize();

    //intersection point to light vector normal
    Vector3D V = _pos - ray.intersection.point;
    V.normalize();

    //reflection normal
    Vector3D R = 2 * V.dot(N) * N - V;
    R.normalize();

    //Viewing vector noraml
    Vector3D D = -ray.dir;
    D.normalize();

    //get the material variable
    //Material* M = ray.intersection.mat;

    Colour ambient = ray.intersection.mat->ambient *_col_ambient;

    Colour diffuse = ray.intersection.mat->diffuse * (std::max(0.0, N.dot(V)) * _col_diffuse);

    Colour specular = ray.intersection.mat->specular * (std::max(0.0, std::pow(D.dot(R),ray.intersection.mat->specular_exp)) * _col_specular);

    //signature
    //ray.col = diffuse;

    //diffuse and ambient
    //ray.col = diffuse + ambient;

    //Phong
    ray.col = diffuse + ambient + specular;

    ray.col.clamp();



}
