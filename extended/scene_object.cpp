/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		implements scene_object.h

***********************************************************/

#include <cmath>
#include <iostream>
#include "scene_object.h"

bool UnitSquare::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld ) {
	// TODO: implement intersection code for UnitSquare, which is
	// defined on the xy-plane, with vertices (0.5, 0.5, 0),
	// (-0.5, 0.5, 0), (-0.5, -0.5, 0), (0.5, -0.5, 0), and normal
	// (0, 0, 1).
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point,
	// intersection.normal, intersection.none, intersection.t_value.
	//
	// HINT: Remember to first transform the ray into object space
	// to simplify the intersection test.
	Ray3D newRay;

	newRay.origin = worldToModel * ray.origin;
	newRay.dir = worldToModel *ray.dir;
	float t = -(newRay.origin[2]) / (newRay.dir[2]);
	if (t <= 0){ // intersection is behind
		return false;
	}
	float xCheck = newRay.origin[0] + (t * newRay.dir[0]);
	float yCheck = newRay.origin[1] + (t * newRay.dir[1]);

	bool xCheckRange = xCheck >= -0.5 && xCheck <= 0.5;
	bool yCheckRange = yCheck >= -0.5 && yCheck <= 0.5;

	if (xCheckRange && yCheckRange){
		// then there is an intersection
		if (ray.intersection.none || ray.intersection.t_value > t){
			// set normals, set point in world coords as ray's intersection
			Point3D point(xCheck, yCheck, 0.0);
			Vector3D norm(0.0f, 0.0f, 1.0f);
			norm.normalize();
			Vector3D newNormal = transNorm(worldToModel, norm);
			ray.intersection.t_value = t;
			ray.intersection.point = modelToWorld * point;
			ray.intersection.normal = newNormal;
			ray.intersection.none = false;

			return true;
		}

	}
	return false;
}

bool UnitSphere::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld ) {
	// TODO: implement intersection code for UnitSphere, which is centred
	// on the origin.

	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point,
	// intersection.normal, intersection.none, intersection.t_value.
	//
	// HINT: Remember to first transform the ray into object space
	// to simplify the intersection test.

	Ray3D newRay;

	newRay.origin = worldToModel * ray.origin;
	newRay.dir = worldToModel * ray.dir;

	Point3D centre(0.0f,0.0f,0.0f);
	Vector3D distance = newRay.origin - centre;

	// Quadratic formula
	double a = newRay.dir.dot(newRay.dir);
	double b = distance.dot(newRay.dir);
	double c = distance.dot(distance) - 1;

	double d = b * b - a * c;

	float t = -1;

	// negative discriminant --> ray doesn't intersect
	if (d < 0) return false;
	else if (d == 0) t = -b / a; // one intersection only; ray grazes sphere
  else {
		float lambda0 = (-b + sqrt(d))/ a;
	  float lambda1 = (-b - sqrt(d))/ a;
		t = fmin(lambda0, lambda1); // two intersection
	}

	if (t <= 0) return false;
			// update the given ray
			if (ray.intersection.none || ray.intersection.t_value > t) {
				// set normals, set point in world coords as ray's intersection
					Point3D point = newRay.origin + t * newRay.dir;
					Vector3D norm = 2 * (point - centre);
					Vector3D newNormal = transNorm(worldToModel, norm);
					newNormal.normalize();
					ray.intersection.t_value = t;
					ray.intersection.point = modelToWorld * point;
					ray.intersection.normal = newNormal;
					ray.intersection.none = false;
					return true;
			}

	return false;
}

bool UnitCylinder::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld ) {
	
	
	Ray3D newRay;
	newRay.origin = worldToModel*ray.origin;
    newRay.dir = worldToModel*ray.dir;
	
	Point3D centre(0.0f,0.0f,0.0f);


	// Quadratic formula
	
	double lambda1;
	double lambda2;
	double lambda3;
	double lambda4;

	Point3D itsPoint;
	Vector3D nor1;
	Vector3D nor2;

	double a = pow(newRay.dir[0],2) + pow(newRay.dir[1],2);
	double b = newRay.origin[0]*newRay.dir[0] + newRay.origin[1] * newRay.dir[1];
	double c = pow(newRay.origin[0],2) + pow(newRay.origin[1],2) - 1;

	
	
	double d = b*b - a*c;
    if (d<0) return false;
    lambda1 = -b/a + sqrt(d) / a;
	lambda2 = -b/a - sqrt(d) / a;
	if (lambda1 < 0 && lambda2 < 0) return false;
	else if (lambda1 > 0 && lambda2 < 0){
		lambda2 = lambda1;
	}else{
		lambda4 = lambda2;
	}
		
	
	lambda1 = -(0.5+newRay.origin[2])/newRay.dir[2];
	lambda2 = (0.5-newRay.origin[2])/newRay.dir[2];
	if (lambda1 < lambda2){
		Point3D temp(0,0,-1);
		lambda3 = lambda1;
		nor1 = temp - centre;
		nor1.normalize();
	}
	else{
		Point3D temp(0,0,1);
		lambda3 = lambda2;
		nor1 = temp - centre;
		nor1.normalize();
	}

	if (pow(lambda3,2) < 1/1000) return false;
	
	itsPoint = newRay.origin + lambda3 * newRay.dir;
	
	if (pow(itsPoint[0],2) + pow(itsPoint[1],2) <=  1){
		if (!ray.intersection.none > ray.intersection.t_value) return false;
		ray.intersection.t_value = lambda3;
		ray.intersection.point = itsPoint;
		ray.intersection.normal = nor1;
		ray.intersection.none = false;
		return true;
	}


	if (pow(lambda4 ,2) < 1/1000) return false;
	itsPoint = newRay.origin + lambda4 * newRay.dir;
	
	nor2[0] = itsPoint[0];
	nor2[1] = itsPoint[1];
	nor2[2] = 0;
	nor2.normalize();


   if (itsPoint[2] < 0.5 && itsPoint[2] > -0.5)
	{
		if (!ray.intersection.none > ray.intersection.t_value)
			return false;
		
		ray.intersection.point = modelToWorld * itsPoint;
		Point3D tmp1(itsPoint[0],itsPoint[1],0);
		ray.intersection.t_value = lambda4;
		ray.intersection.normal = modelToWorld * (tmp1 - centre);
		ray.intersection.none = false;
		return true;
	}
	return false;
}