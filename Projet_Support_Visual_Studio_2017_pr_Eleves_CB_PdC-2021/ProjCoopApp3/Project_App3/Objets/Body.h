#include <string>
#include "../geometry.h"
#include "../forms.h"
#include <vector>

using namespace std;

class Body : public Sphere
{
private:
    string name;
    float mass;
	void setName(string x) { name = x; }
	void setMass(float x) { mass = x; }

public:
	string getName() const {return name;}
    float getMass() const {return mass;}
	Body(string name, float mass, double radius, Vector acc, Vector speed, Point pos);
	void update(double delta_t, vector<Body> bodies);
};

class Camera : public Form
{
private:
	Point lookTarget;
	Vector offset;

public:
	Point getLookTarget() const { return lookTarget; }
	void setLookTarget(Point target) { lookTarget = target; }
	Vector getOffset() const { return offset; }
	void setOffset(Vector os) { offset = os; }

	Camera(Point lTarget, Vector os);

	void update(double delta_t);
	void render();
};