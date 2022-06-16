#include "Body.h"
#include "../forms.h"
#include <vector>
#include <iostream>


Body::Body(string name, float mass, double radius, Vector acc, Vector speed, Point pos)
{

	//Sphere attribute
	//Sphere(radius);
	getAnim().setAccel(acc);
	getAnim().setSpeed(speed);
	

    // Constructor
    // Initialization
    setName(name);
    setMass(mass);
}

void Body::update(double delta_t, vector<Body> bodies)
{
	Sphere::update(delta_t);

	//Martin code l� si il a besoin de la masse
	//Calcul acceleration
	// get all object Body
	
	/*for (int i = 0; i < bodies.size(); i++) {
		//Calcul distance

		//Calcul angle
		
		//Calcul acceleration

		//Calcul speed
		
		//Calcul position
	}*/
	
}