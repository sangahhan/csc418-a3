/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		   light source classes

***********************************************************/

#include "util.h"


extern int width;
extern int height;
extern unsigned char* rbuffer;
extern unsigned char* gbuffer;
extern unsigned char* bbuffer;



// Base class for a light source.  You could define different types
// of lights here, but point light is sufficient for most scenes you
// might want to render.  Different light sources shade the ray
// differently.
class LightSource {
public:
    using Ptr = std::shared_ptr<LightSource>;
	virtual void shade( Ray3D& ) = 0;
	virtual void shade( Ray3D&, bool shadow ) = 0;
	virtual Point3D get_position() const = 0;
    virtual ~LightSource() {}
};

// A point light is defined by its position in world space and its
// colour.
class PointLight : public LightSource {
public:
	PointLight( Point3D pos, Colour col ) : _pos(pos), _col_ambient(col),
	_col_diffuse(col), _col_specular(col) {}
	PointLight( Point3D pos, Colour ambient, Colour diffuse, Colour specular )
	: _pos(pos), _col_ambient(ambient), _col_diffuse(diffuse),
	_col_specular(specular) {}
	void shade( Ray3D& ray );
	void shade( Ray3D& ray, bool shadow );
	Point3D get_position() const { return _pos; }

private:
	Point3D _pos;
	Colour _col_ambient;
	Colour _col_diffuse;
	Colour _col_specular;
};
