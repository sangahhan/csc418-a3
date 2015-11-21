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
			// set normals, set point in world coords
			Point3D point(xCheck, yCheck, 0.0);
			Vector3D norm(0.0f, 0.0f, 1.0f);
			norm.normalize();
			Vector3D newNormal = transNorm(worldToModel, norm);
			//newNormal.normalize();
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
	else if (d == 0) t = -b / a;
  else {
		float lambda0 = (-b + sqrt(d))/ a;
	  float lambda1 = (-b - sqrt(d))/ a;
		t = fmin(lambda0, lambda1);
	}

	if (t <= 0) return false;
	// update the given ray
	if (ray.intersection.none || ray.intersection.t_value > t) {
			Point3D point = newRay.origin + t * newRay.dir;
			Vector3D norm = 2 * (point - centre);
			//norm.normalize();
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
