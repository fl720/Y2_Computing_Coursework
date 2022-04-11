#include "simulator.h"

int main()
{
    std::string input_file("parameters.txt");  //NAME  INPUT FILE NAME
    std::string output_file("output.txt");     //NAME OUTPUT FILE NAME

    simulator handle(input_file, output_file); 
    handle.run() ; 
    
    return 0;
}