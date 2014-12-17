#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <cstring>

class scene {
public:
    scene();
    ~scene();
    void Init();
    void Forward(float, float);
    void Left(float, float);
    void Right(float, float);
    void Backward(float, float);
    void TurnLeft(float, float, float);
    void TurnRight(float, float, float);
    void TurnUp(float, float, float, float);
    void TurnDown(float, float, float, float);
    float eye[3], vat[3], vup[3], fovy, dnear, dfar, viewport[4];
};
class BALL {
public:
    void init();
    double r, g, b;
    double x, y, z;
    double v[3];
    double radius;
};
class PLANE
{
public:
    void init();
    double x, y, z; // center
    double r, g, b, alpha;
    double loc[4][3]; // [vertex_num][x,y,z]; // 0: x  1: y  2: z
    double u_norm[3]; // unit norm // [x,y,z]; // 0: x  1: y  2: z
    double area;
    void collapse(BALL &rhs);
};
