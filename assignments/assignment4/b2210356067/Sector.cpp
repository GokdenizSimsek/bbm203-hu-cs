#include "Sector.h"
#include <cmath>
#include <iostream>

using namespace std;
// Constructor implementation

Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
        // TODO: Calculate the distance to the Earth, and generate the sector code
        distance_from_earth = std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
        int intDistance = static_cast<int>(distance_from_earth);
        string sectorString;
        sectorString += std::to_string(intDistance);
        if (x > 0) {
            sectorString += "R";
        } else if (x < 0){
            sectorString += "L";
        } else {
            sectorString += "S";
        }
        if (y > 0) {
            sectorString += "U";
        } else if (y < 0) {
            sectorString += "D";
        } else {
            sectorString += "S";
        }
        if (z > 0) {
            sectorString += "F";
        } else if (z < 0) {
            sectorString += "B";
        } else {
            sectorString += "S";
        }
        sector_code = sectorString;
}

Sector::~Sector() {
    // TODO: Free any dynamically allocated memory if necessary
}

Sector& Sector::operator=(const Sector& other) {
    // TODO: Overload the assignment operator
    return *this;
}

bool Sector::operator==(const Sector& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator!=(const Sector& other) const {
    return !(*this == other);
}