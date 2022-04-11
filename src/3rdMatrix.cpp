#include "3rdMatrix.h" 
#include <cstring>

matrix3d::matrix3d()                             //3X3 IDENTICAL MATRIX GENERATOR
{
    memset(data, 0, sizeof(data));
    data[0][0] = 1 ;
    data[1][1] = 1 ;
    data[2][2] = 1 ; 
}

matrix3d matrix3d::operator * (FLOAT f)          //MATRIX(3X3) * CONSTANT
{
    matrix3d ans ; 
    ans.data[0][0] = data[0][0] * f ; 
    ans.data[0][1] = data[0][1] * f ; 
    ans.data[0][2] = data[0][2] * f ; 
    ans.data[1][0] = data[1][0] * f ; 
    ans.data[1][1] = data[1][1] * f ; 
    ans.data[1][2] = data[1][2] * f ; 
    ans.data[2][0] = data[2][0] * f ; 
    ans.data[2][1] = data[2][1] * f ; 
    ans.data[2][2] = data[2][2] * f ; 

    return ans ; 
}

vector3d matrix3d::operator * (vector3d v)       //MATRIX(3X3) * VECTOR(1X3) 
{
    vector3d ans ; 
    ans[0] = data[0][0] * v[0] +  data[0][1] * v[1] + data[0][2] * v[2]  ;
    ans[1] = data[1][0] * v[0] +  data[1][1] * v[1] + data[1][2] * v[2]  ;
    ans[2] = data[2][0] * v[0] +  data[2][1] * v[1] + data[2][2] * v[2]  ;
    return ans;
}

vector3d::vector3d()                             //1X3 VECTOR GENERATOR
{
    memset(data, 0, sizeof(data));
}

vector3d::vector3d(FLOAT a, FLOAT b, FLOAT c)    //VECTOR FILLING 
{
    data[0] = a;
    data[1] = b;
    data[2] = c;
}

vector3d vector3d::operator + (vector3d e)      //VECTOR(1X3) + VECTOR(1X3)
{
    return vector3d( data[0]+e.data[0] , data[1]+e.data[1] , data[2]+e.data[2] ) ;
}

vector3d vector3d::operator * (FLOAT d)         //VECTOR(1X3) * CONSTANT
{   
    return vector3d(data[0]*d , data[1]*d , data[2]*d) ; 
}

FLOAT& vector3d::operator [] (unsigned long i)  //VECTOR RETRIEVE 
{
    return data[i];
}