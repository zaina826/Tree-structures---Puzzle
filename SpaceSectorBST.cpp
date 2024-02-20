#include "SpaceSectorBST.h"

using namespace std;
#include <fstream>


SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

void SpaceSectorBST::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the BST sector map
    // according to the given comparison critera based on the sector coordinates.

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
}

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {

    Sector *sector = new Sector(x, y, z);
    sectorMap[sector->sector_code] = sector;
    //First for root node:
    if (root==nullptr){
        root = sector;
        return;}
    if (sector->SectorCompare(*root)==1){
        Sector *ogroot=root;
        root=root->right;
        delete(sector);
        insertSectorByCoordinates(x,y,z);
        ogroot->right=root;
        root=ogroot;
        return;
    }
    if (sector->SectorCompare(*root)==-1){
        Sector *ogroot=root;
        root=root->left;
        delete(sector);
        insertSectorByCoordinates(x,y,z);
        ogroot->left=root;
        root=ogroot;
        return;
    }
    // Instantiate and insert a new sector into the space sector BST map according to the 
    // coordinates-based comparison criteria.
}

//Finding the inorder Successor
Sector* SpaceSectorBST::inorderSuccessor(Sector* node) {
    Sector *succesor= node->right;
    while (succesor->left!=nullptr){
        succesor=succesor->left;
    }

    return(succesor);
}

Sector* SpaceSectorBST::deleteSuccesor(Sector* node){
    if (node==nullptr){
        return nullptr;
    }
    if (node->left==nullptr){
        Sector *temp= node->right;
        if (node!=nullptr){
        delete(node);}
        return temp;
    }
    node ->left= deleteSuccesor(node->left);
    return(node);
}



void SpaceSectorBST::deleteSector(const std::string& sector_code) {

    if (sectorMap.find(sector_code) != sectorMap.end()) {
    Sector *toBeDeleted= sectorMap[sector_code];
    
    if (toBeDeleted->SectorCompare(*root)==-1){
        Sector *ogroot=root;
        root=root->left;
        deleteSector(sector_code);
        ogroot->left=root;
        root=ogroot;
        return;
    }
    if (toBeDeleted->SectorCompare(*root)==1){
        Sector *ogroot=root;
        root=root->right;
        deleteSector(sector_code);
        ogroot->right=root;
        root=ogroot;
        return;
    }

    //Otherwise, this is where we do the deletion (base case):
 

    if (root->left==nullptr){
        Sector *temp=root->right;
        if (root!=nullptr){
        delete(root);}
        root=temp;
        return;
    }

    if (root->right==nullptr){
        Sector *temp=root->left;
        if (root!=nullptr){
        delete(root);
        // sectorMap.erase(sector_code);
}
        root=temp;
        return;
    }
    //The last case is where it has 2 children, we first find the inorder succesor, and replace it then delete:
    Sector* Succesor= inorderSuccessor(root);

    string code= Succesor->sector_code;
    double distance = Succesor->distance_from_earth;
    root->sector_code= code;
    root->distance_from_earth= distance;
    root->x=Succesor->x;
    root->y=Succesor->y;
    root->z=Succesor->z;

    sectorMap[root->sector_code] = root;

    root->right=deleteSuccesor(root->right);}
    else{
        return;
    }
    
    // TODO: Delete the sector given by its sector_code from the BST.
}


void SpaceSectorBST::displaySectorsInOrder() {
    std::cout<<"Space sectors inorder traversal:"<<std::endl;
    inorderRecur();
    std::cout<<""<<std::endl;
    // TODO: Traverse the space sector BST map in-order and print the sectors 
    // to STDOUT in the given format.
}
 

void SpaceSectorBST::inorderRecur(){
    if (root!=nullptr){
    Sector *ogroot=root;
    root=root->left;
    inorderRecur();
    root=ogroot;
    std::cout<<root->sector_code<<std::endl;
    root=root->right;
    inorderRecur();
    root=ogroot;
    }
}

void SpaceSectorBST::displaySectorsPreOrder() {
    std::cout<<"Space sectors preorder traversal:"<<std::endl;
    preorderRecur();
    std::cout<<""<<std::endl;

    // TODO: Traverse the space sector BST map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
}

void SpaceSectorBST::preorderRecur(){
    if (root!=nullptr){
    Sector *ogroot=root;
    std::cout<<root->sector_code<<std::endl;
    root=root->left;
    preorderRecur();
    root=ogroot;
    root=root->right;
    preorderRecur();
    root=ogroot;
    }
}

void SpaceSectorBST::displaySectorsPostOrder() {
    std::cout<<"Space sectors postorder traversal:"<<std::endl;
    postorderRecur();
    std::cout<<""<<std::endl;

    // TODO: Traverse the space sector BST map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
}

void SpaceSectorBST::postorderRecur(){
   if (root!=nullptr){
    Sector *ogroot=root;
    root=root->left;
    postorderRecur();
    root=ogroot;
    root=root->right;
    postorderRecur();
    root=ogroot;
    std::cout<<root->sector_code<<std::endl;

    }
}

std::vector<Sector*> SpaceSectorBST::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    Sector *temp = root;
    if (sectorMap.find(sector_code) == sectorMap.end()) {
        //If it's not in the dictionary
        return path;
    }
    Sector *destination = sectorMap[sector_code];
    while (temp != nullptr && temp != destination) {
        path.push_back(temp);
        if (destination->SectorCompare(*temp) == -1) {
            temp = temp->left;
        } else if (destination->SectorCompare(*temp) == 1) {
            temp = temp->right;
        }
    }
    path.push_back(temp);
    return path;
}

void SpaceSectorBST::printStellarPath(const std::vector<Sector*>& path) {
    if (path.empty()){
        std::cout<<"A path to Dr. Elara could not be found."<<std::endl;
        std::cout<<""<<std::endl;
        std::cout<<""<<std::endl;
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
    std::cout<<""<<std::endl;
    std::cout<<""<<std::endl;
    std::cout<<""<<std::endl;
    // TODO: Print the stellar path obtained from the getStellarPath() function 
    // to STDOUT in the given format.
}

SpaceSectorBST::~SpaceSectorBST() {
    if (root==nullptr){
        return;
    }
    while (root->right!=nullptr || root->left!=nullptr) {
        deleteSector(root->sector_code);
    }
    deleteSector(root->sector_code);
    // Free any dynamically allocated memory in this class.
}
