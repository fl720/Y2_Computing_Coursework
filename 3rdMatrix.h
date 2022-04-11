#include <iostream>
typedef double FLOAT; // double is alternative

class vector3d {
    public:

        vector3d() ; 
        vector3d(FLOAT a, FLOAT b, FLOAT c) ; 
        
        FLOAT data[3];

        vector3d operator + (vector3d b) ;

        vector3d operator * (FLOAT d) ; 

        FLOAT& operator [] (unsigned long i) ; 

        void print()
        {
            std::cout << data[0] << " " << data[1] << " " << data[2] << " " << std::endl;
        }
};

class matrix3d {
    public:

        matrix3d() ; 

        FLOAT data[3][3];

        vector3d operator * (vector3d v) ;

        matrix3d operator * (FLOAT f);
};
