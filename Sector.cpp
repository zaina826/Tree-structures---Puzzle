#include "Sector.h"
#include <iostream>
#include <cmath>




int Sector::SectorCompare(const Sector& other){
    // if (sector_code==other.sector_code){
    //     return(0);
    // }
    if (x>other.x || (x==other.x && y>other.y) || (x==other.x && y==other.y && z>other.z) ){
        return (1);
    }
    if (x<other.x || (x==other.x && y<other.y) || (x==other.x && y==other.y && z<other.z) ){
        return(-1);
    }
    return(0);
}

std::string Sector::dirLetter(int coordinate, std::string dimension){
    if(coordinate == 0){
        return "S";
    }
    if (coordinate < 0){
        if (dimension == "x"){
            return "L";
        }
        if (dimension == "y"){
            return "D";
        }
        if (dimension == "z"){
            return "B";
        }
    }
    if (coordinate > 0){
        if (dimension == "x"){
            return "R";
        }
        if (dimension == "y"){
            return "U";
        }
        if (dimension == "z"){
            return "F";
        }
    }
    return "Invalid";
}

// Constructor implementation
Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
    //Since the earth's coordinates are (0,0,0) then the formula is the root of the sum of their squares:
    distance_from_earth= (sqrt(x*x+y*y+z*z));     //Note this is a double.
    sector_code.append(std::to_string(int(floor((distance_from_earth)))));
    sector_code.append(dirLetter(x,"x"));
    sector_code.append(dirLetter(y,"y"));
    sector_code.append(dirLetter(z,"z"));
    // TODO: Calculate the distance to the Earth, and generate the sector code
}

Sector::~Sector() {
    // TODO: Free any dynamically allocated memory if necessary
}

Sector& Sector::operator=(const Sector& other) {
    color= other.color;
    x= other.x;
    y= other.y;
    z= other.z;
    distance_from_earth= other.distance_from_earth;
    left = other.left;
    right = other.right;
    parent = other.parent;
    // TODO: Overload the assignment operator
    return *this;
}

bool Sector::operator==(const Sector& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator!=(const Sector& other) const {
    return !(*this == other);
}

