#include "SpaceSectorLLRBT.h"

using namespace std;
#include <fstream>
#include <unordered_map>
#include <unordered_set>
SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
     //First reading the data:
    std::ifstream sectorsData(filename);
    if (!sectorsData.is_open()) {
        std::cerr << "Error while opening Sectors data file...";
        return;
    }
    
    std::string line;
    //Skip the first line (X, Y, Z)
    sectorsData>>line;

    //Now for every line:
    while (sectorsData >> line) {
        std::istringstream ss(line);
        std::string xStr, yStr, zStr;
        
        std::getline(ss, xStr, ','); 
        std::getline(ss, yStr, ','); 
        std::getline(ss, zStr);  
        
        int x=stoi(xStr);
        int y=stoi(yStr);
        int z=stoi(zStr);

        insertSectorByCoordinates(x,y,z);
    }
    // TODO: read the sectors from the input file and insert them into the LLRBT sector map
    // according to the given comparison critera based on the sector coordinates.
}

// Remember to handle memory deallocation properly in the destructor.
SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    recurDeallocate(root);
}

void SpaceSectorLLRBT::recurDeallocate(Sector* node) {
    if (node == nullptr) {
        return;
    }
    recurDeallocate(node->left);
    recurDeallocate(node->right);
    delete node;
}


void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    Sector *sector = new Sector(x, y, z);
    sectorMap2[sector->sector_code] = sector;
    if (root==nullptr){
        root = sector;
        if (!top_root){
            root->color=RED;
        }
        else{
            root->color=BLACK;
        }

        return;}
    if (sector->SectorCompare(*root)==1){
        root=check(root);
        top_root=false;
        Sector *ogroot=root;
        root=root->right;
        delete(sector);
        insertSectorByCoordinates(x,y,z);
        root=check(root);
        ogroot->right=root;
        root->parent=ogroot;
        root=ogroot;
        root=check(root);
        return;
    }
    if (sector->SectorCompare(*root)==-1){
        root=check(root);
        top_root=false;
        Sector *ogroot=root;
        root=root->left;
        delete(sector);
        insertSectorByCoordinates(x,y,z);
        root=check(root);
        ogroot->left=root;
        root->parent=ogroot;
        root=ogroot;
        root=check(root);
        return;
    }

}

Sector* SpaceSectorLLRBT::check(Sector* root){
    if (root == nullptr) {
        return nullptr;
    }

    if (root->right != nullptr && root->right->color && (root->left == nullptr || !root->left->color)) {
        Sector* X = root->right;
        root->right = X->left;
        if (X->left != nullptr) {
            X->left->parent = root;
        }
        X->left = root;
        X->parent = root->parent;
        root->parent = X;
        
        X->color = root->color;
        root->color = RED;

        return X;
    }

    if (root->left != nullptr && root->left->left != nullptr && root->left->color && root->left->left->color) {
        Sector* X = root->left;
        root->left = X->right;
        if (X->right != nullptr) {
            X->right->parent = root;
        }
        X->right = root;
        X->parent = root->parent;
        root->parent = X;

        X->color = root->color;
        root->color = RED;

        return X;
    }

    if (root->right != nullptr && root->left != nullptr && root->right->color && root->left->color) {
        //col flip
        root->color = RED;
        root->left->color = BLACK;
        root->right->color = BLACK;
        return root;
    }
    return root;
}



//We do the same thing we did in the regular BST:
void SpaceSectorLLRBT::displaySectorsInOrder() {
    std::cout<<"Space sectors inorder traversal:"<<std::endl;
    inorderRecur();
    std::cout<<""<<std::endl;

    // TODO: Traverse the space sector BST map in-order and print the sectors 
    // to STDOUT in the given format.
}

void SpaceSectorLLRBT::inorderRecur(){
    if (root!=nullptr){
    Sector *ogroot=root;
    root=root->left;
    inorderRecur();
    root=ogroot;
    std::string red=root->color?"RED ":"BLACK ";
    std::cout<<red<<"sector: "<<root->sector_code<<std::endl;
    root=root->right;
    inorderRecur();
    root=ogroot;
    }
}

void SpaceSectorLLRBT::displaySectorsPreOrder() {
    std::cout<<"Space sectors preorder traversal:"<<std::endl;
    preorderRecur();
    std::cout<<""<<std::endl;
    // TODO: Traverse the space sector BST map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
}

void SpaceSectorLLRBT::preorderRecur(){
    if (root!=nullptr){
    Sector *ogroot=root;
    std::string red=root->color?"RED ":"BLACK ";
    std::cout<<red<<"sector: "<<root->sector_code<<std::endl;
    root=root->left;
    preorderRecur();
    root=ogroot;
    root=root->right;
    preorderRecur();
    root=ogroot;
    }
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    std::cout<<"Space sectors postorder traversal:"<<std::endl;
    postorderRecur();
    std::cout<<""<<std::endl;

    // TODO: Traverse the space sector BST map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
}

void SpaceSectorLLRBT::postorderRecur(){
   if (root!=nullptr){
    Sector *ogroot=root;
    root=root->left;
    postorderRecur();
    root=ogroot;
    root=root->right;
    postorderRecur();
    root=ogroot;
    std::string red=root->color?"RED ":"BLACK ";
    std::cout<<red<<"sector: "<<root->sector_code<<std::endl;

    }
}
std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    if (sectorMap2.find(sector_code) == sectorMap2.end()) {
        return path;
    }
    Sector* Earth = sectorMap2["0SSS"];
    bool earthCol= Earth->color;
    Sector* Elara = sectorMap2[sector_code];
    std::unordered_set<Sector*> visited;

    // The lowest common parent 
    while (Earth != nullptr) {
        visited.insert(Earth);
        Earth = Earth->parent;
    }
    Sector* commonParent = nullptr;
    // Finding the path from that parent to Elara
    std::vector<Sector*> pathFromElaraToCommon;
    while (Elara != nullptr) {
        pathFromElaraToCommon.push_back(Elara);
        if (visited.find(Elara) != visited.end()) {
            commonParent = Elara;
            break;
        }
        Elara = Elara->parent;
    }

    if (commonParent != nullptr ) {
        std::vector<Sector*> earthToParent;  
        // from Earth to the common parent
        Earth = sectorMap2["0SSS"]; // Resetting Earth to its original state
        while (Earth != commonParent) {
            earthToParent.push_back(Earth);
            Earth = Earth->parent;
        }

//Black when it shoyld be red
        // Restoring colors
        Earth->color = BLACK;

        // Combining the paths
        path.insert(path.end(), earthToParent.begin(), earthToParent.end());
        std::reverse(pathFromElaraToCommon.begin(), pathFromElaraToCommon.end());
        path.insert(path.end(), pathFromElaraToCommon.begin(), pathFromElaraToCommon.end());
        
        return path;
    }
    
    // Case where commonParent is nullptr
    path.clear();
    return path;
}



void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
    if (path.empty()){
        std::cout<<"A path to Dr. Elara could not be found."<<std::endl;
        std::cout <<""<< std::endl;
        std::cout <<""<< std::endl;
        return;
    }
    std::cout<<"The stellar path to Dr. Elara: ";
    for (size_t i = 0; i < path.size(); ++i) {
    std::cout << path[i]->sector_code;
    //By knowing the path size, we can check if we have reached the final element, if we havent we print an arrow.
    if (i != path.size() - 1) {
        std::cout << "->";
    }
}
    std::cout <<""<< std::endl;
    std::cout <<""<< std::endl;
    std::cout <<""<< std::endl;

    // TODO: Print the stellar path obtained from the getStellarPath() function 
    // to STDOUT in the given format.
}


