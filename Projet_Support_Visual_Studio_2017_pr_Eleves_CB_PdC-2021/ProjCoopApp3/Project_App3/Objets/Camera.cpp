#include "Camera.h"

void Camera::update(double delta_t)
{
	Form::update(delta_t);
}

void Camera::render()
{
	Form::render();
}

Camera::Camera(Point lTarget, Vector os, Form& posTarget)
{
	//Form attribute

	//Camera attribute
	setLookTarget(lTarget);
	setOffset(os);
	setPositionTarget(posTarget);
}
