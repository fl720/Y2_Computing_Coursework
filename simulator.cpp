#include "simulator.h"
#include <cmath>
#include <iostream> 

simulator::simulator( std::string input_filename , std::string output_filename )
{
    read_parameters(input_filename) ; 
    
    ofs.open(output_filename) ; 
}

/*
Format:
ρ g CD v0 h0
m0 mR A m ̇f ue ∆t
m0 mR A m ̇f ue ∆t
m0 mR A m ̇f ue ∆t
...
*/
void simulator::read_parameters(std::string input_filename) 
{
    std::ifstream ifs(input_filename);
    if ( !ifs.is_open() )
    {
        return ; 
    }
    
    ifs >> rho ; 
    ifs >> g ; 
    ifs >> Cd ; 
    ifs >> v ; 
    ifs >> h ; 

    m = 0 ;                                 //TOTAL MASS INITIALTION 

    while( !ifs.eof() )
    {
        stage para; 
        ifs >> para.m0 ; 
        ifs >> para.mR ; 
        ifs >> para.A ; 
        ifs >> para.mf_rate ; 
        ifs >> para.ue ; 
        ifs >> para.dt ; 

        m       += para.m0 ;                //SUM OF THE TOTAL MASS
        para.mf =  para.m0 - para.mR ;      //CALCULATION OF FUEL AT EACH STAGE

        stages.push_back(para) ;         
    }
    
    return ; 
}

void simulator::run()
{
    total_T     = 0 ;               //TOTAL TIME OF FLIGHT 
    max_h       = h ;               //MAXIMIUM OF THE HEIGHT FROM THE SIMULATION
    v_terminal  = 0 ;               //TERMINAL VELOCITY

    vector3d x(h , v , m) ;         //INPUT VALUES FOR VECOTR X 
    vector3d k1 ;                   
    vector3d k2 ;
    vector3d k3 ;
    vector3d k4 ;   

    /*
        generating matrix of M(x) in the eqaution
        | 0 1    0 |
        | 0 f(x) 0 |
        | 0 0    0 |  
    */
    matrix3d M ;     
    M.data[0][0] = 0 ;  
    M.data[0][1] = 1 ; 
    M.data[2][2] = 0 ; 

    FLOAT &h_ = x[0] ;                  //REFERENCEING HEIGHT AT CURRENT TIME
    FLOAT &v_ = x[1] ;                  //REFERENCEING VELOCITY AT CURRENT TIME 
    FLOAT &m_ = x[2] ;                  //REFERENCEING MASS AT CURRENT TIME 


    int final = 0 ;                     //RECORD OF FINAL STAGE FOR FREE FALL
    bool islanded = false ;             //LANDED MARKER

    FLOAT v_last = x[1] ;               //RECORD OF PERVIOUS VELOCITY
    bool v_terminal_reached = false ;   //TERMINAL VELOCITY MARKER

//LOOP UNTIL ALL STAGE AND ALL FUEL HAVE RUN OUT. 
    for(size_t i = 0 ; i < stages.size() ; i++)
    { 
        auto f = [&](vector3d x) -> FLOAT // lambda
        {
            return ( 2 * stages[i].mf_rate * stages[i].ue - rho * Cd * stages[i].A * x[1] * x[1] ) / ( 2 * x[2] * fabs(x[1]) ) ; 
        };

        vector3d B(0 , -g , -stages[i].mf_rate ) ;   
        for(unsigned int j = 0 ; j < (stages[i].mf / stages[i].mf_rate) / stages[i].dt ; j++  )
        {
            //OUTPUT DATA DURING ROCKET BURNING
            ofs << total_T << " " << h_ << " " << v_ << " " << m_ << std::endl;
            
            v_last = x[1] ;                 //LAST VELOCITY RECORD
            total_T += stages[i].dt ;       //FLIGHT TIME RECORD


            //k1, k2, k3, k4 :  
            M.data[1][1] = f(x);           
            k1 = M * x + B ; 

            M.data[1][1] = f(x + k1 * (stages[i].dt/2.0) ); 
            k2 = M * (x + k1 * (stages[i].dt/2.0) )+ B ; 

            M.data[1][1] = f(x + k2 * (stages[i].dt/2.0) ); 
            k3 = M * (x + k2 * (stages[i].dt/2.0) )+ B;
            
            M.data[1][1] = f(x + k3 * stages[i].dt); 
            k4 = M * (x + k3 * stages[i].dt )+ B; 

            //x_next :
            x = x + (k1 + k2 * 2 + k3 * 2 + k4 ) * (stages[i].dt/6) ; 

            // x.print();
            // exit(0);

            max_h = max_h < h_ ? h_ : max_h;     //MAX HEIGHT EXAM

            if( h_ < 0 )                         //LANDED EXAM
            {
                islanded = true ;                //LANDED COMFIRM 
                break ;                          //MISSION ACCOMPULISHED
            }

            if( v_ < 0 && !v_terminal_reached && fabs(v_ - v_last)  < 1e-5 )        //TERMINAL VELOCITY REACHED EXAM
            {
                v_terminal = v_last ;           //TERMINAL VELOCITY RECORD
                v_terminal_reached = true ;     //TERMINAL VELOCITY COMFIRED
            }
        }

        if(i < stages.size() -  1) // NOT LAST STAGE
            m_ -= stages[i].mR ; 

        final = i ;              //FINAL STAGE DATA MARKER

        if (islanded == true)    //LANDED EXAM
        {
            break ;              //MISSION ACCOMPULISHED COMFIRMED
        }
    }

//LOOP OF FREE FALL. 
    while( !islanded )          //when rocket is still above the 0 meter after mf reached 0
    {

        auto f = [&](vector3d x) -> FLOAT
        {
            return ( - rho * Cd * stages[final].A * x[1] * x[1] ) / ( 2 * x[2] * fabs(x[1]) ) ; 
        };

        vector3d B(0 , -g , 0 ) ;  
 
        //OUTPUT DATA DURING FREE FALL
        ofs << total_T << " " << h_ << " " << v_ << " " << m_ << std::endl;
        
        v_last = x[1] ;                     //LAST VELOCITY RECORD
        total_T += stages[final].dt ;       //FLIGHT TIME RECORD

        //k1, k2, k3, k4 :  
        M.data[1][1] = f(x); 
        k1 = M * x + B ; 

        M.data[1][1] = f(x + k1 * (stages[final].dt/2.0)); 
        k2 = M * (x + k1 * (stages[final].dt/2.0) )+ B ; 

        M.data[1][1] = f(x + k2 * (stages[final].dt/2.0)); 
        k3 = M * (x + k2 * (stages[final].dt/2.0) )+ B;

        M.data[1][1] = f(x + k3 * stages[final].dt); 
        k4 = M * (x + k3 * stages[final].dt )+ B; 

        //x_next :
        x = x + (k1 + k2 * 2 + k3 * 2 + k4 ) * (stages[final].dt/6) ; 

        max_h = max_h < h_ ? h_ : max_h;     //MAX HEIGHT EXAM

        if( h_ < 0 )                         //LANDED EXAM
        {
            islanded = true ;                //LANDED COMFIRM 
            break ;                          //MISSION ACCOMPULISHED
        } 

        if( v_ < 0 && !v_terminal_reached && fabs(v_ - v_last)  < 1e-5 )        //TERMINAL VELOCITY REACHED EXAM
        {
            v_terminal = v_last ;            //TERMINAL VELOCITY RECORD
            v_terminal_reached = true ;      //TERMINAL VELOCITY COMFIRMED
        }
    }

    //LAST SET OF OUTPUT DATA    
    ofs << total_T << " " << h_ << " " << v_ << " " << m_ << std::endl;   


//=======DISPLAYING_TOTAL_TIME_MAXIMIUM_HEIGHT_TERMINAL_VELOCITY========================
    std::cout << "Total time of estimation is :" << total_T << " s." << std::endl ;  
    std::cout << "Maximium height is :" << max_h << "m." <<std::endl ;  
    if( v_terminal_reached )
    {
        std::cout << "Terminal velocity is :" << v_terminal << "m/s." << std::endl ;  
    } else 
    {
        std::cout << "No ternimal velocity has reached. " << std::endl ;
    }

    return ;
}
