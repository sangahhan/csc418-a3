/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

        Implementations the main function which
        specifies the scene to be rendered.

***********************************************************/
#include "raytracer.h"
int main(int argc, char* argv[])
{
	// Build your scene and setup your camera here, by calling
	// functions from Raytracer.  The code here sets up an example
	// scene and renders it from two different view points, DO NOT
	// change this if you're just implementing part one of the
	// assignment.
	Raytracer raytracer;
	int width = 400;
	int height = 400;

	if (argc == 3) {
		width = atoi(argv[1]);
		height = atoi(argv[2]);
	}

	// Camera parameters.
	Point3D eye(0., 0., 1.);
	Vector3D view(0., 0., -1.);
	Vector3D up(0., 1., 0.);
	Vector3D side(0., 0., 1.);

	double fov = 60;

	// Defines a material for shading.
    Material::Ptr gold = std::make_shared<Material>( Colour(0.3, 0.3, 0.3), Colour(0.75164, 0.60648, 0.22648),
			Colour(0.628281, 0.555802, 0.366065),
			51.2 );
    Material::Ptr jade = std::make_shared<Material>( Colour(0, 0, 0), Colour(0.54, 0.89, 0.63),
			Colour(0.316228, 0.316228, 0.316228),
			12.8 );
    Material::Ptr glass = std::make_shared<Material>( Colour(0.001, 0.001, 0.001), Colour(0., 0., 0.),
			Colour(0.999, 0.999, 0.999),
			10000 );

	// Defines a point light source.
	raytracer.addLightSource( std::make_shared<PointLight>(Point3D(0., 0., 5.),
				Colour(0.9, 0.9, 0.9) ) );

	// Add a unit square into the scene with material mat.
    SceneDagNode::Ptr sphere = raytracer.addObject( std::make_shared<UnitSphere>(), glass );
		//SceneDagNode::Ptr cylinder = raytracer.addObject( std::make_shared<UnitCylinder>(), gold );
		//SceneDagNode::Ptr plane2 = raytracer.addObject( std::make_shared<UnitSquare>(), jade );
    SceneDagNode::Ptr plane = raytracer.addObject( std::make_shared<UnitSquare>(), jade );
    //SceneDagNode::Ptr glassSphere = raytracer.addObject( std::make_shared<UnitSphere>(), glass );

    
	// Apply some transformations to the unit square.
	double factor1[3] = { 1.0, 2.0, 1.0 };
	double factor2[3] = { 7.5, 7.5, 7.5 };
	double factor3[3] = { 2.0, 2.0, 2.0 };
	//raytracer.translate(sphere, Vector3D(0., 0., 0.));
	//raytracer.rotate(sphere, 'x', -45);
	//raytracer.rotate(sphere, 'z', 45);
	//raytracer.scale(sphere, Point3D(0., 0., 0.), factor1);

	//raytracer.translate(glassSphere, Vector3D(1, -0.9, -0.5));
	//raytracer.scale(glassSphere, Point3D(0, 0, 0), factor1);



	raytracer.translate(cylinder, Vector3D(0., 5., -5.));
	raytracer.rotate(cylinder, 'x', -90);
    raytracer.rotate(cylinder, 'z', 90);
	raytracer.scale(cylinder, Point3D(0., 0., 0.), factor1);

	// Apply some transformations to the unit square.
	raytracer.translate(sphere, Vector3D(0., 0., -5.));
	//raytracer.rotate(sphere, 'x', -45);
	//raytracer.rotate(sphere, 'z', 45);
	//raytracer.scale(sphere, Point3D(0., 0., 0.), factor1);

	raytracer.translate(plane, Vector3D(0., 0., -7.));
	raytracer.rotate(plane, 'z', 45);
	raytracer.scale(plane, Point3D(0., 0., 0.), factor2);

		// Render the scene, feel free to make the image smaller for
		// testing purposes.
		raytracer.render(width, height, eye, view, up, fov, "view1.bmp");

		// Render it from a different point of view.
		Point3D eye2(4., 2., 1.);
		Vector3D view2(-4., -2., -6.);
		raytracer.render(width, height, eye2, view2, up, fov, "view2.bmp");

	Point3D eye3(4., 4., 3.);
	Vector3D view3(-4., -3., -5.);
	raytracer.render(width, height, eye3, view3, side, fov, "view3.bmp");

	return 0;
}
