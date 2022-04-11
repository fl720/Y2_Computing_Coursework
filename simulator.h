#include <string>
#include <vector>
#include <fstream>

#include "3rdMatrix.h"

struct stage {
    FLOAT A ;                   //CROSS SECTIOANL AREA
    FLOAT mf_rate ;             //RATE OF FLOW OF THE FUEL MASS
    FLOAT ue ;                  //EXHUAST VELOCITY
    FLOAT mR ;                  //MASS OF ROCKET
    FLOAT m0 ;                  //MASS IN TOTAL
    FLOAT dt ;                  //STEP SIZE
    FLOAT mf ;                  //MASS OF FUEL
};

class simulator {

    public:

        simulator(std::string input_filename , std::string output_filename );   //INPUT&OUTPUT FILE NAME GIVEN 

        void run();            //SIMULATION CORE 
        
        FLOAT total_T ;        //TOTAL TIME OF THE FLIGHT
        FLOAT v_terminal ;     //TERMINAL VELOCITY REACHED DURING FALLING OF THE ROCKET
        FLOAT max_h ;          //MAXIMIUM HEIGHT GIVEN FROM THE SIMULATION

    private: 

        FLOAT h ;              //HEIGHT
        FLOAT v ;              //VELOCITY
        FLOAT m ;              //MASS

        FLOAT Cd ;             //DRAG COEFFICIENT
        FLOAT rho ;            //AIR DENSITY
        FLOAT g ;              //GRAVITATIONAL CONSTANT

        std::vector<stage> stages ;   //STAGES DATA

        std::ofstream ofs;            //OUTPUT FUNCTION

        void read_parameters(std::string input_filename) ;      //OPEN INPUT PARAMETERS FILE
        
};