#include <fstream> 
  
void AjouterUneLigne() 
{ 
    // std::ios_base::out est automatiquement ajouté par le 
    // constructeur de std::ofstream 
    std::ofstream file( "comGPS.txt"); 
    file << "coordonées GPS\n"; 
} 
  
int main() 
{ 
    // création du fichier et écriture d'une ligne 
    AjouterUneLigne(); 
    // ouverture du fichier existant et rajout d'une nouvelle ligne 
    AjouterUneLigne(); 
    // "fichier.txt" contient 2 lignes 
}
