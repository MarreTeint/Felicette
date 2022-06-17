#include "Body.h"
#include "../forms.h"
#include <vector>
#include <iostream>
#include <math.h>

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
	
	
		Vector vIni = getAnim().getSpeed();
		Point posIni = getAnim().getPos();
		const float G = 6.67408*pow(10, -11);
		vector<Vector> vectors;
		for (int j = 0; j < bodies.size(); j++) {
			Point contact = bodies.at(j).getAnim().getPos();
			if (posIni.x != contact.x && posIni.y != contact.y && posIni.z != contact.z) {
				//Calculer la distance
				double dist = distance(posIni, contact);//sqrt(pow((contact.x-posIni.x),2) + pow((contact.y - posIni.y),2) + pow((contact.z - posIni.z),2));
				//Calculer la gravitée
				float grav = (G* getMass()*bodies.at(j).getMass()) / dist;
				Vector vgrav = Vector(grav*(contact.x-posIni.x)/dist, grav*(contact.y - posIni.y) / dist, grav*(contact.z - posIni.z) / dist);
				vectors.push_back(vgrav);
			}
		}
		//Donner un vecteur gravité global
		Vector graGlobal;
		for (int k = 0; k < vectors.size(); k++) {
			graGlobal = graGlobal + vectors.at(k);
		}
		//Calculer new position
		Point newPos = (graGlobal.x*pow(delta_t, 2) + vIni.x*delta_t + posIni.x, graGlobal.y*pow(delta_t, 2) + vIni.y*delta_t + posIni.y, graGlobal.z*pow(delta_t, 2) + vIni.z*delta_t + posIni.z);
		//setter
		getAnim().setPos(newPos);
	
}