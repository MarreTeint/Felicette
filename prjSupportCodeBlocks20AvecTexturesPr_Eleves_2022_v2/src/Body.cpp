
#include "forms.h"
#include <GL/GLU.h>
#include <vector>
#include <iostream>
#include <math.h>
#include "Body.h"

Body::Body(string name, float mass, double radius, Vector acc, Vector speed, Point pos)
{

	//Sphere attribute
	setRadius(radius);
	getAnim().setAccel(acc);
	getAnim().setSpeed(speed);


    // Constructor
    // Initialization
    setName(name);
    setMass(mass);
}
