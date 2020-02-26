#include <fstream> 
#include <string>  
#include <iostream> 

int main() 
{ 
    std::ifstream file( "bufferGPS" ); 
    if ( !file ) 
    { 
        std::cerr << "Erreur d'ouverture\n"; 
        return 1; 
    } 
  
    std::string line; 
    if ( ! ( file >> line ) ) 
    { 
        std::cerr << "Erreur de lecture\n"; 
        return 2; 
    } 
    std::cout << "Ligne lue : " << line; 
} 
