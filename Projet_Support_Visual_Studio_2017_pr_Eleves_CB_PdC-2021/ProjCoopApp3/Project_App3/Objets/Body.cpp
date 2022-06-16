#include "Body.h"
#include "../forms.h"
#include <vector>


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

void Body::update(double delta_t, int max_form,Form* forms_list[])
{
	Sphere::update(delta_t);

	//Martin code l� si il a besoin de la masse
	//Calcul acceleration
	// get all object Body
	
	for (int i = 0; i < max_form; i++) {
		forms_list[i].name();
	}
	
	//vitesse
	
	//position
	
}