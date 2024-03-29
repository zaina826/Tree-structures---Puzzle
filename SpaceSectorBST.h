#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>
#include <unordered_map>

#include "Sector.h"

class SpaceSectorBST {
private:
    std::unordered_map<std::string, Sector*> sectorMap;

public:
    Sector *root;
    SpaceSectorBST();
    ~SpaceSectorBST();
    void readSectorsFromFile(const std::string& filename); 
    void insertSectorByCoordinates(int x, int y, int z);
    void deleteSector(const std::string& sector_code);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);
    Sector* inorderSuccessor(Sector* node);
    Sector* deleteSuccesor(Sector* node);
    void inorderRecur();
    void preorderRecur();
    void postorderRecur();
    Sector* deleteNode(Sector* root, const std::string& sector_code) ;

};

#endif // SPACESECTORBST_H
