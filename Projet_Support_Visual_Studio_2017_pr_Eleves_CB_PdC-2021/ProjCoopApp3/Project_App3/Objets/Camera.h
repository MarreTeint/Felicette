#include <string>
#include <geometry.h>
#include "../forms.h"
using namespace std;

class Camera : public Form
{
private:
	Point lookTarget;
	Vector offset;
	Form& positionTarget;

public:
	Point getLookTarget() const { return lookTarget; }
	void setLookTarget(Point target) { lookTarget = target; }
	Vector getOffset() const { return offset; }
	void setOffset(Vector os) { offset = os; }
	Form& getPositionTarget() const { return positionTarget; }
	void setPositionTarget(Form& posTarget) { positionTarget = posTarget; }

	Camera(Point lTarget, Vector os, Form& posTarget);

	void update(double delta_t);
	void render();
};
