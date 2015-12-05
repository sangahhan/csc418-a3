/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

        Implementations the main function which
        specifies the scene to be rendered.

***********************************************************/
#include "raytracer.h"
#include "bmp_io.h"

bool ANTIALIAS = false;
bool GLOSSY_REFLECT = false;
bool REFLECT = false;
bool SHADOW = false;
bool SOFT_SHADOW = false;
bool MAPPING = false;

//global mapping buffer
unsigned char* rbuffer;
unsigned char* gbuffer;
unsigned char* bbuffer;
int width;
int height;



int main(int argc, char* argv[])
{
	// Build your scene and setup your camera here, by calling
	// functions from Raytracer.  The code here sets up an example
	// scene and renders it from two different view points, DO NOT
	// change this if you're just implementing part one of the
	// assignment.
	Raytracer raytracer;
	width = 320;
	height = 240;


	const char* view1_file = NULL;
	const char* view2_file = NULL;
	const char* view3_file = NULL;

	const char* syntax = "Syntax: ./raytracer antialias|glossyreflect|reflect|softshadow|shadow|mapping|all [width height]";

	if (argc == 2 || argc == 4) {
		char* feature = argv[1];
		if (strcmp("antialias", feature) == 0){
			ANTIALIAS = true;
			view1_file = "render_antialias1.bmp";
			view2_file = "render_antialias2.bmp";
			view3_file = "render_antialias3.bmp";
		} else if (strcmp("glossyreflect", feature) == 0) {
			GLOSSY_REFLECT = true;
			view1_file = "render_glossyreflect1.bmp";
			view2_file = "render_glossyreflect2.bmp";
			view3_file = "render_glossyreflect3.bmp";
		} else if (strcmp("reflect", feature) == 0) {
			REFLECT = true;
			view1_file = "render_reflect1.bmp";
			view2_file = "render_reflect2.bmp";
			view3_file = "render_reflect3.bmp";
		} else if (strcmp("softshadow", feature) == 0) {
			SOFT_SHADOW = true;
			view1_file = "render_softshadow1.bmp";
			view2_file = "render_softshadow2.bmp";
			view3_file = "render_softshadow3.bmp";
		} else if (strcmp("shadow", feature) == 0) {
			SHADOW = true;
			view1_file = "render_shadow1.bmp";
			view2_file = "render_shadow2.bmp";
			view3_file = "render_shadow3.bmp";
		} else if (strcmp("mapping", feature) == 0) {
			MAPPING = true;
			view1_file = "mapping1.bmp";
			view2_file = "mapping2.bmp";
			view3_file = "mapping3.bmp";
		} else if (strcmp("all", feature) == 0) {
			ANTIALIAS = true;
			GLOSSY_REFLECT = true;
			REFLECT = false;
			SOFT_SHADOW = true;
			SHADOW = false;
			view1_file = "render_all1.bmp";
			view2_file = "render_all2.bmp";
			view3_file = "render_all3.bmp";
		} else {
			std::cout << syntax << std::endl;
			exit(1);
		}
		if (argc == 4) {
			width = atoi(argv[2]);
			height = atoi(argv[3]);
		}
	} else if (argc == 1) {
		std::cout << syntax << std::endl;
		exit(1);
	}


	// Camera parameters.
	Point3D eye(0., 0., 1.);
	Vector3D view(0., 0., -1.);
	//Vector3D view(0., 0., -0.8);
	Vector3D up(0., 1., 0.);
	double fov = 60;
    
    int map = 0;
    if (MAPPING){
    	map = 2;
    }
	

	
    Material::Ptr gold = std::make_shared<Material>( Colour(0.3, 0.3, 0.3), Colour(0.75164, 0.60648, 0.22648),
			Colour(0.628281, 0.555802, 0.366065),
			51.2, 1, 0.1, 0);
    
    Material::Ptr jade = std::make_shared<Material>( Colour(0, 0, 0), Colour(0.54, 0.89, 0.63),
			Colour(0.316228, 0.316228, 0.316228),
			12.8, 0, 1, 0);
    Material::Ptr colorful = std::make_shared<Material>( Colour(0, 0, 0), Colour(0., 0., 0.),
			Colour(0., 0., 0.),
			12.8, 0, 1, map);
    
    
    

	// Defines a point light source.
	raytracer.addLightSource( std::make_shared<PointLight>(Point3D(0., 0., 5.),
				Colour(0.9, 0.9, 0.9) ) );
    



	// Add a unit square into the scene with material mat.
	
    SceneDagNode::Ptr sphere = raytracer.addObject( std::make_shared<UnitSphere>(), gold );
    
    
    SceneDagNode::Ptr plane = raytracer.addObject( std::make_shared<UnitSquare>(), jade );
    //SceneDagNode::Ptr plane = raytracer.addObject( std::make_shared<UnitSquare>(), colorful );
    
    //SceneDagNode::Ptr cylinder = raytracer.addObject( std::make_shared<UnitCylinder>(), gold );


	// Apply some transformations to the unit square.
	double factor1[3] = { 1.0, 2.0, 1.0 };
	double factor2[3] = { 14., 14., 14. };
	double factor3[3] = { 1.5, 1.5, 2 };
	
	raytracer.translate(sphere, Vector3D(0., 0., -6.));
	
	 //raytracer.rotate(sphere, 'x', -45);
	 //raytracer.rotate(sphere, 'z', 45);
	 //raytracer.scale(sphere, Point3D(0., 0., 0.), factor1);



	raytracer.translate(plane, Vector3D(0., 0., -7.));
	raytracer.rotate(plane, 'z', 45);
	raytracer.scale(plane, Point3D(0., 0., 0.), factor2);
	 


	//raytracer.translate(cylinder, Vector3D(0, 0, -6));
	//raytracer.scale(cylinder, Point3D(0, 0, 0), factor3);
	
	// Render the scene, feel free to make the image smaller for
	// testing purposes.
	//Vector3D view(0.75,-1, -6.);
	raytracer.render(width, height, eye, view, up, fov, view1_file);
	//std::cout <<" test" << std::endl; 


	// Render it from a different point of view.
	Point3D eye2(6., 1., -1.);
	Vector3D view2(-4., -2., -6.);
	raytracer.render(width, height, eye2, view2, up, fov, view2_file);

	Point3D eye3(2., 2, -1);
	Vector3D view3(-1., -2.5, -6.);
	raytracer.render(width, height, eye3, view3, -1 * up, fov, view3_file);

	return 0;
}

