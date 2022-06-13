#include "Body.h"


Body::Body(String name, float size, float mass)
{
    // Constructor
    // Initialization
    this.name = name;
    this.size = size;
    this.mass = mass;
    this.speed = 0.0f //Default speed
}

Body::Body(String name, float size, float mass, float speed)
{
    // Constructor
    // Initialization
    this.name = name;
    this.size = size;
    this.mass = mass;
    this.speed = speed;
}