#include <cmath>
double project_len(double x,double y,double z,
                   double on_x,double on_y,double on_z)
{
    return (x*on_x+y*on_y+z*on_z)/sqrt(on_x*on_x+on_y*on_y+on_z*on_z);
}
double dot(double x,double y,double z,
           double on_x,double on_y,double on_z)
{
    return (x*on_x+y*on_y+z*on_z);
}