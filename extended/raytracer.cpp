/***********************************************************
Starter code for Assignment 3

This code was originally written by Jack Wang for
CSC418, SPRING 2005

Implementations of functions in raytracer.h

***********************************************************/


#include "raytracer.h"
#include "bmp_io.h"
#include <cmath>
#include <iostream>
#include <cstdlib>






Raytracer::Raytracer() : _lightSource(nullptr) {
	_root = std::make_shared<SceneDagNode>();
}

Raytracer::~Raytracer() {
}

SceneDagNode::Ptr Raytracer::addObject( SceneDagNode::Ptr parent,
	SceneObject::Ptr obj, Material::Ptr mat ) {
		SceneDagNode::Ptr node = std::make_shared<SceneDagNode>( obj, mat );
		node->parent = parent;
		node->next = nullptr;
		node->child = nullptr;

	// Add the object to the parent's child list, this means
	// whatever transformation applied to the parent will also
	// be applied to the child.
	if (parent->child == nullptr) {
		parent->child = node;
	}
	else {
		parent = parent->child;
		while (parent->next != nullptr) {
			parent = parent->next;
		}
		parent->next = node;
	}

	return node;
}

LightListNode::Ptr Raytracer::addLightSource( LightSource::Ptr light ) {
	LightListNode::Ptr tmp = _lightSource;
	_lightSource = std::make_shared<LightListNode>( light, tmp );
	return _lightSource;
}

void Raytracer::rotate( SceneDagNode::Ptr node, char axis, double angle ) {
	Matrix4x4 rotation;
	double toRadian = 2*M_PI/360.0;
	int i;

	for (i = 0; i < 2; i++) {
		switch(axis) {
			case 'x':
			rotation[0][0] = 1;
			rotation[1][1] = cos(angle*toRadian);
			rotation[1][2] = -sin(angle*toRadian);
			rotation[2][1] = sin(angle*toRadian);
			rotation[2][2] = cos(angle*toRadian);
			rotation[3][3] = 1;
			break;
			case 'y':
			rotation[0][0] = cos(angle*toRadian);
			rotation[0][2] = sin(angle*toRadian);
			rotation[1][1] = 1;
			rotation[2][0] = -sin(angle*toRadian);
			rotation[2][2] = cos(angle*toRadian);
			rotation[3][3] = 1;
			break;
			case 'z':
			rotation[0][0] = cos(angle*toRadian);
			rotation[0][1] = -sin(angle*toRadian);
			rotation[1][0] = sin(angle*toRadian);
			rotation[1][1] = cos(angle*toRadian);
			rotation[2][2] = 1;
			rotation[3][3] = 1;
			break;
		}
		if (i == 0) {
			node->trans = node->trans*rotation;
			angle = -angle;
		}
		else {
			node->invtrans = rotation*node->invtrans;
		}
	}
}

void Raytracer::translate( SceneDagNode::Ptr node, Vector3D trans ) {
	Matrix4x4 translation;

	translation[0][3] = trans[0];
	translation[1][3] = trans[1];
	translation[2][3] = trans[2];
	node->trans = node->trans*translation;
	translation[0][3] = -trans[0];
	translation[1][3] = -trans[1];
	translation[2][3] = -trans[2];
	node->invtrans = translation*node->invtrans;
}

void Raytracer::scale( SceneDagNode::Ptr node, Point3D origin, double factor[3] ) {
	Matrix4x4 scale;

	scale[0][0] = factor[0];
	scale[0][3] = origin[0] - factor[0] * origin[0];
	scale[1][1] = factor[1];
	scale[1][3] = origin[1] - factor[1] * origin[1];
	scale[2][2] = factor[2];
	scale[2][3] = origin[2] - factor[2] * origin[2];
	node->trans = node->trans*scale;
	scale[0][0] = 1/factor[0];
	scale[0][3] = origin[0] - 1/factor[0] * origin[0];
	scale[1][1] = 1/factor[1];
	scale[1][3] = origin[1] - 1/factor[1] * origin[1];
	scale[2][2] = 1/factor[2];
	scale[2][3] = origin[2] - 1/factor[2] * origin[2];
	node->invtrans = scale*node->invtrans;
}

Matrix4x4 Raytracer::initInvViewMatrix( Point3D eye, Vector3D view,
	Vector3D up ) {
		Matrix4x4 mat;
		Vector3D w;
		view.normalize();
		up = up - up.dot(view)*view;
		up.normalize();
		w = view.cross(up);

	mat[0][0] = w[0];
	mat[1][0] = w[1];
	mat[2][0] = w[2];
	mat[0][1] = up[0];
	mat[1][1] = up[1];
	mat[2][1] = up[2];
	mat[0][2] = -view[0];
	mat[1][2] = -view[1];
	mat[2][2] = -view[2];
	mat[0][3] = eye[0];
	mat[1][3] = eye[1];
	mat[2][3] = eye[2];

	return mat;
}


void Raytracer::computeTransforms( SceneDagNode::Ptr node )
{
	SceneDagNode::Ptr childPtr;
	if (node->parent != nullptr)
	{
		node->modelToWorld = node->parent->modelToWorld*node->trans;
		node->worldToModel = node->invtrans*node->parent->worldToModel;
	}
	else
	{
		node->modelToWorld = node->trans;
		node->worldToModel = node->invtrans;
	}
	// Traverse the children.
	childPtr = node->child;
	while (childPtr != NULL) {
		computeTransforms(childPtr);
		childPtr = childPtr->next;
	}



}

void Raytracer::traverseScene( SceneDagNode::Ptr node, Ray3D& ray ) {
	SceneDagNode::Ptr childPtr;

	// Applies transformation of the current node to the global
	// transformation matrices.
	if (node->obj) {
		// Perform intersection.
		if (node->obj->intersect(ray, node->worldToModel, node->modelToWorld)) {
			ray.intersection.mat = node->mat;
		}
	}
	// Traverse the children.
	childPtr = node->child;
	while (childPtr != nullptr) {
		traverseScene(childPtr, ray);
		childPtr = childPtr->next;
	}

}



void Raytracer::computeShading( Ray3D& ray ) {
	LightListNode::Ptr curLight = _lightSource;

	for (;;) {
		if (curLight == nullptr) break;
		// Each lightSource provides its own shading function.

		// Implement shadows here if needed.

		/* FROM TUTORIAL:
		make a ray with origin ray.intersection.point + 0.01 * dir
		and dir (light.position - ray.intersection.point).normalize;
		get intersections -> object in shadow if intersection occurs
		before the light*/
		/* FROM TEXTBOOK:
		function raycolor( ray e + td, real t0, real t1 )
		hit-record rec, srec
		if (scene→hit(e + td, t0, t1, rec)) then
		p = e + (rec.t) d
		color c = rec.ka Ia
		if (not scene→hit(p + sl, ε, ∞, srec)) then
		vector3 h = normalized(normalized(l) + normalized(−d))
		c = c + rec.kd I max (0, rec.n · l) + (rec.ks) I (rec.n · h)rec.p
		return c
		else
		return background-color*/
		// TODO: SHADOWS
		// set up shadow ray
		if (SHADOW) {
			Ray3D newRay;
			newRay.dir = curLight->light->get_position() - ray.intersection.point;
			double t = newRay.dir.length();
			newRay.dir.normalize();
			newRay.origin = ray.intersection.point + 0.01 * newRay.dir;

			traverseScene(_root, newRay);

			curLight->light->shade(ray);
			if (!newRay.intersection.none && t >= newRay.intersection.t_value){
				ray.col = 0.5 * ray.col; // if in shadow, darken
			}
		} else if (SOFT_SHADOW) {
			// TODO: SOFT SHADOW
			int n = 30;
			//Colour sum(0., 0., 0.);
			for (int i = 0; i < n; i++) {
				// set up sample shadow rays
				double jitter1 = (double)rand() / (double) RAND_MAX;
				double jitter2 = (double)rand() / (double) RAND_MAX;
				Vector3D axis1 (0., 3., 0.);
				Vector3D axis2 (0., 0., 3.);

				Ray3D newRay;
				Point3D lightPos = curLight->light->get_position() + jitter1 * axis1 + jitter2 * axis2;
				newRay.dir = lightPos - ray.intersection.point;
				newRay.dir.normalize();
				// avoid intersection with some object
				newRay.origin = ray.intersection.point + 0.01 * newRay.dir;

				traverseScene(_root, newRay);
				curLight->light->shade(ray);
				// if sample shadow rays intersect, shade the original ray
				if (!newRay.intersection.none) {
					ray.col =  0.5 * ray.col;
				}
			}
		}  else {
			curLight->light->shade(ray);
		}
		ray.col.clamp();
		curLight = curLight->next;

	}
}

void Raytracer::initPixelBuffer() {
	int numbytes = _scrWidth * _scrHeight * sizeof(unsigned char);
	_rbuffer = new unsigned char[numbytes];
	std::fill_n(_rbuffer, numbytes,0);
	_gbuffer = new unsigned char[numbytes];
	std::fill_n(_gbuffer, numbytes,0);
	_bbuffer = new unsigned char[numbytes];
	std::fill_n(_bbuffer, numbytes,0);
}

void Raytracer::flushPixelBuffer( std::string file_name ) {
	bmp_write( file_name.c_str(), _scrWidth, _scrHeight, _rbuffer, _gbuffer, _bbuffer );
	delete _rbuffer;
	delete _gbuffer;
	delete _bbuffer;
}

Colour Raytracer::shadeRay( Ray3D& ray ) {
	Colour col(0.0, 0.0, 0.0);
	traverseScene(_root, ray);

	// Don't bother shading if the ray didn't hit
	// anything.
	if (!ray.intersection.none) {
		computeShading(ray);
		col = ray.col;
		// You'll want to call shadeRay recursively (with a different ray,
		// of course) here to implement reflection/refraction effects.


		if(ray.intersection.mat->specular_exp > 0){
			//TODO: REFLECTION
			// FROM TUTORIAL:
			/* if we bounce the ray off the surface and see what it hits, we
			have a reflection. the new ray's origin is ray.intersection.point
			and direction =
			(2 * (ray.dir . ray.intersection.normal) * ray.intersetion.normal - ray.dir).normalize()
			// from textbook: d−2(d·n)n
			new colour = colour + shadeRay(newray) * damp factor
			where the damp factor is based on distance
			only bounce if the matieral is reflective and don't bounce infinitely*/
			if (REFLECT) {
				// set up reflection ray
				Point3D ray_intersect = ray.intersection.point;
				Vector3D ray_dir = ray.dir;
				Vector3D ray_norm = ray.intersection.normal;
				ray_norm.normalize();
				Vector3D reflection_dir = ray_dir - (2 * ray_dir.dot(ray_norm) * ray_norm);
				reflection_dir.normalize();

				Ray3D newRay;
				newRay.origin = ray_intersect;
				newRay.dir = reflection_dir;

				// calculate shade of reflected ray
				shadeRay(newRay);

				if (newRay.intersection.t_value > 0.0) {
					// the damp factor is based on distance
					float dampFactor = fabs(1 /newRay.intersection.t_value); // is this the right way to get damp factor?
					// side note: damp factor for perfectly reflective is 0, but we don't
					// have any perfectly reflective surfaces....
					if (dampFactor < 0) dampFactor = 0;
					if (dampFactor > 1) dampFactor = 1;
					col = col + dampFactor * newRay.col;
				}
			} else if (GLOSSY_REFLECT){
				//TODO: GLOSSY REFLECT
				int n = 4;
				int rays = 0;

				// set up reflection ray
				Vector3D ray_dir = ray.dir;
				Vector3D ray_norm = ray.intersection.normal;
				ray_norm.normalize();
				Vector3D reflection_dir = ray_dir - (2 * ray_dir.dot(ray_norm) * ray_norm);
				reflection_dir.normalize();
				Ray3D newRay;
				newRay.dir = reflection_dir;

				for (int i = 0; i < n; i++) {
					double jitter1 = (double)rand() / (double) RAND_MAX;
					double jitter2 = (double)rand() / (double) RAND_MAX;
					Vector3D axis1 (0., 1., 0.);
					Vector3D axis2 (0., 0., 1.);

					// jitter the reflection ray
					Point3D ray_intersect = ray.intersection.point + jitter1 * axis1 + jitter2 * axis2;
					newRay.origin = ray_intersect;

					shadeRay(newRay);
					if (newRay.intersection.t_value > 0.0) {
				    // the damp factor is based on distance
						float dampFactor = fabs(1 /newRay.intersection.t_value); // is this the right way to get damp factor?
						// side note: damp factor for perfectly reflective is 0, but we don't
						// have any perfectly reflective surfaces....
						if (dampFactor < 0) dampFactor = 0;
						if (dampFactor > 1) dampFactor = 1;
						rays ++;
						// result: col + a weighted average of dampened colours
						col = col + ((dampFactor/n) * newRay.col);
				  }
				}
			}
			col.clamp();
		}
	}
	return col;
}

Colour Raytracer::render_helper (Matrix4x4 viewToWorld, Point3D imagePlane){
	// Sets up ray origin and direction in view space, image plane is at z = -1.
	Point3D origin(0., 0., 0.);

	// TODO: Convert ray to world space and call
	// shadeRay(ray) to generate pixel colour.

	// ray -> world space
	Point3D rayOriginWorld = viewToWorld * imagePlane;
	Vector3D rayDir = imagePlane - origin;
	Vector3D rayDirWorld = viewToWorld * rayDir;

	Ray3D ray;

	ray.origin = rayOriginWorld;
	ray.dir = rayDirWorld;
	ray.dir.normalize();

	// generate pixel colour
	return shadeRay(ray);
}

void Raytracer::render( int width, int height, Point3D eye, Vector3D view,
	Vector3D up, double fov, std::string fileName ) {
	computeTransforms(_root);
	Matrix4x4 viewToWorld;
	_scrWidth = width;
	_scrHeight = height;
	double factor = (double(height)/2)/tan(fov*M_PI/360.0);

	initPixelBuffer();
	viewToWorld = initInvViewMatrix(eye, view, up);

	// Construct a ray for each pixel.
	for (int i = 0; i < _scrHeight; i++) {
		for (int j = 0; j < _scrWidth; j++) {
			//TODO: ANTIALIAS
			/*
			TEXTBOOK:
			for each pixel (i, j) do c=0
			for p = 0 to n − 1 do for q = 0 to n − 1 do
			c = c + ray-color(i + (p + 0.5)/n, j + (q + 0.5)/n) cij = c/n2
			*/
			Point3D imagePlane;
			if (ANTIALIAS){

				for(float p = i; p < i + 1.0f; p += 0.5f){
					for(float q = j; q < j + 1.0f; q += 0.5f){
						imagePlane[0] = (-double(width)/2 + 0.5 + q)/factor;
						imagePlane[1] = (-double(height)/2 + 0.5 + p)/factor;
						imagePlane[2] = -1;
						Colour col = render_helper(viewToWorld, imagePlane);
						// reduce contribution of rays to 1/4
						_rbuffer[i*width+j] += int(col[0]*255*0.25);
						_gbuffer[i*width+j] += int(col[1]*255*0.25);
						_bbuffer[i*width+j] += int(col[2]*255*0.25);
					}
				}
			} else {
				imagePlane[0] = (-double(width)/2 + 0.5 + j)/factor;
				imagePlane[1] = (-double(height)/2 + 0.5 + i)/factor;
				imagePlane[2] = -1;
				Colour col = render_helper(viewToWorld, imagePlane);
				_rbuffer[i*width+j] = int(col[0]*255);
				_gbuffer[i*width+j] = int(col[1]*255);
				_bbuffer[i*width+j] = int(col[2]*255);
			}
		}
	}
	flushPixelBuffer(fileName);
}
