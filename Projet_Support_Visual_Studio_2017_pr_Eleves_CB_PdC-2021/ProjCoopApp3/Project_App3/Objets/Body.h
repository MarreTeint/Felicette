#include <string>
using namespace std;

class Corp
{
private:
    String name;
    float size;
    float mass;
    float speed;

public:
    String getName() const {return name;}
    float getSize() const {return size;}
    float getMass() const {return mass;}
    float getSpeed() {return speed;}
    void setSpeed(float x) {speed = x;}
};
