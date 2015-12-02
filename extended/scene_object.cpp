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
			Vector3D norm(0., 0., 1.);
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

	float t;

	// negative discriminant --> ray doesn't intersect
	if (d < 0) return false;
	else if (d == 0) t = -b / a; // one intersection only; ray grazes sphere
  else {
		float lambda0 = (-b + sqrt(d))/ a;
	  float lambda1 = (-b - sqrt(d))/ a;
		t = fmin(lambda0, lambda1); // two intersection
	}

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
	// TODO: CYLINDER

	Ray3D newRay;

	newRay.origin = worldToModel * ray.origin;
	newRay.dir = worldToModel * ray.dir;

	Point3D centre(0.,0.,0.);
	Vector3D distance = newRay.origin - centre;

	// r = 1
	float a = newRay.dir[0] * newRay.dir[0] + newRay.dir[1] * newRay.dir[1];
	float b = distance[0] * newRay.dir[0] + distance[1] * newRay.dir[1];
	float c = distance[0] * distance[0] + distance[1] * distance[1] - 1;
	float d = b * b - a * c;


  /*Find intersection with ”quadratic wall,” ignoring constraints on z,
	   e.g. using x^2 + y^2 − (1/4) (1 − z^2) = 0.
	   Then test the z component of p ̄(λ∗) against the constraint on z,
	   e.g. z ≤ 1 or z < 1.*/
	float t;
	float lambda0;
	float lambda1;

	// negative discriminant --> ray doesn't intersect
	if (d <= 0 || a == 0) return false;
 	else {
		lambda0 = (-b + sqrt(d))/ a;
	  lambda1 = (-b - sqrt(d))/ a;
		t = fmin(lambda0, lambda1);
	}

	if (ray.intersection.none || ray.intersection.t_value > t) {
		Point3D point = newRay.origin + t * newRay.dir;
		if ( (point[2] > -1) && (point[2] < 1)){
			Vector3D newNormal = Vector3D(point[0], point[1], 0);
			newNormal.normalize();

			//update the ray
			ray.intersection.none = false;
			ray.intersection.t_value = t;
			ray.intersection.point = modelToWorld * point;
			ray.intersection.normal = transNorm(worldToModel, newNormal);
			return true;
		}
	}


	/*Intersect the ray with the planes containing the base or cap (e.g. z = 1
		for the cylinder). Then test the x and y components of p ̄(λ∗) to see if they
		satisfy interior constraints (e.g. for the cylinder).

		If there are multiple intersections, then take the intersection with the
		smallest positive λ (i.e., closest to the start of the ray).*/

	// t1, t2, t3, t4 refers to http://www.irisa.fr/prive/kadi/Master_Recherche/cours_CTR/RayTracing.pdf

	lambda0 = (1 - distance[2]) / newRay.dir[2]; // t1
	lambda1 = -distance[2] / newRay.dir[2]; // t2

	// https://www.cl.cam.ac.uk/teaching/1999/AGraphHCI/SMAG/node2.html
	// z = 0
	bool outside = (lambda0 > 0) && (lambda1 > 0); // ray is outside caps
	bool inside = ((lambda0 > 0) && (lambda1 < 0)) || ((lambda0 < 0) && (lambda1 > 0)); // ray is inside caps

	if (outside) t = fmin(lambda0, lambda1); // pick smaller lambda --> t3
	else t = fmax(lambda0, lambda1); // pick larger lambsa --> t4

	// find intersection with ray and cyl
	Point3D cap_intersect = newRay.origin + t * newRay.dir;

	//  x^2 + y^2 < 1
	float x2y2 = cap_intersect[0] * cap_intersect[0] + cap_intersect[1] * cap_intersect[1];;

	// doesn't intersect a cap
	if (!ray.intersection.none && (ray.intersection.t_value <= t)) return false;

	// determine normal vector based on where intersects
	// z=-1 for bottom cap, z=1 for top cap
	Vector3D normal = Vector3D(0., 0., 1.);
	if(cap_intersect[2] < 0.5) normal[2] = -1;

	if(x2y2 < 1){
		Vector3D newNormal = transNorm(worldToModel, normal);
		newNormal.normalize();
		ray.intersection.none = false;
		ray.intersection.t_value = t;
		ray.intersection.point = modelToWorld * cap_intersect;
		ray.intersection.normal = newNormal;
		return true;
	}

	// after all that checking, nothing's been returned --> no hit
	return false;

}
