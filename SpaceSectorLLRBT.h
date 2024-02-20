#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
class SpaceSectorLLRBT {
private:
    std::unordered_map<std::string, Sector*> sectorMap2;

public:
    Sector* root;
    SpaceSectorLLRBT();
    ~SpaceSectorLLRBT();
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    void inorderRecur();
    void preorderRecur();
    void postorderRecur(); 
    Sector* check(Sector* root);
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    bool top_root=true;
    void recurDeallocate(Sector* root);
    void printStellarPath(const std::vector<Sector*>& path);
    Sector* insert(Sector* root, Sector* sector);
};

#endif // SPACESECTORLLRBT_H
