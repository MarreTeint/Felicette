#include <string>
#include <geometry.h>
#include <forms.h>
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
	void update(double delta_t);
};
