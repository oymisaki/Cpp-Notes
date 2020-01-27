#include <math.h>

using namespace std;

const float eps = 0.00001f;

// f'(x) = 2x
// x1 = x0 - (x0^2 - a / 2x0)
float sqrt(float x)
{
    float val = x;
    float last;
    do
    {
        last = val;
        val = (val + x/val) / 2;
    } while (fabsf(val - last) < eps);
    
    return val;
} 