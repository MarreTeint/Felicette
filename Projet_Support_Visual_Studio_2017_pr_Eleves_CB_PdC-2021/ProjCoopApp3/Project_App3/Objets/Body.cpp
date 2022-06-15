#include "Body.h"


Body::Body(string name, float mass, double radius, Vector acc, Vector speed, Point pos)
{

	//Sphere attribute
	Sphere(radius);
	getAnim().setAccel(acc);
	getAnim().setSpeed(speed);
	

    // Constructor
    // Initialization
    setName(name);
    setMass(mass);
}

void Body::update(double delta_t)
{
	Sphere::update(delta_t);

	//Martin code l� si il a besoin de la masse
}