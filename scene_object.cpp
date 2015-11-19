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

	bool xCheckRange = xCheck >= -0.5 && xCheck >= 0.5;
	bool yCheckRange = yCheck >= -0.5 && yCheck >= 0.5;

	if (xCheckRange && yCheckRange){
		// then there is an intersection
		if (ray.intersection.none || ray.intersection.t_value > t){
			ray.intersection.t_value = t;
			// set normals, set point in world coords
			ray.intersection.point = modelToWorld * Point3D(xCheck, yCheck, 0.0);
			Vector3D newNormal = transNorm(worldToModel, Vector3D(0.0, 0.0, 1.0));
			newNormal.normalize();
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
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point,
	// intersection.normal, intersection.none, intersection.t_value.
	//
	// HINT: Remember to first transform the ray into object space
	// to simplify the intersection test.

	Ray3D newRay;

	newRay.origin = worldToModel * ray.origin;
	newRay.dir = worldToModel * ray.dir;

	Point3D origin = (0,0,0);
	Vector3D distance = newRay.origin - origin;

	// Quadratic formula
	float a = newRay.dir.dot(newRay.dir);
	float b = (newRay.dir.dot(newRay.origin - distance)) * 2;
	float c = distance.dot(distance) - 1

	double discrim = b * b - 4 * a * c;

	if (discrim < 0) return false;

	// calculate solutions
    t0 = (-B + sqrt(d))/(2.0 * A);
    t1 = (-B - sqrt(d))/(2.0 * A);

	return false;
}
