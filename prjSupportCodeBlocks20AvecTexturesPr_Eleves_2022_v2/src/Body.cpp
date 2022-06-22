#include "Body.h"
#include "../forms.h"
#include <vector>
#include <iostream>


Body::Body(string name, float mass, double radius, Vector acc, Vector speed, Point pos)
{

	setRadius(radius);
	getAnim().setAccel(acc);
	getAnim().setSpeed(speed);
	getAnim().setPos(pos);
	

    // Constructor
    // Initialization
    setName(name);
    setMass(mass);
}

void Body::update(double delta_t, vector<Body> bodies)
{
	1;
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

void Camera::update(double delta_t)
{
	
}

void Camera::render()
{
	Form::render();
}

Camera::Camera(Point lTarget, Point pos)
{
	//Form attribute

	//Camera attribute
	setLookTarget(lTarget);
	anim.setPos(pos);
}

void Camera::rotAround(double angle, Vector v)
{
	Vector posVect(anim.getPos().x, anim.getPos().y, anim.getPos().z);
	Vector newPosVect = rodriguesRot(posVect, 1 / v.norm() * v, angle);
	anim.setPos(Point(newPosVect.x, newPosVect.y, newPosVect.z));
}