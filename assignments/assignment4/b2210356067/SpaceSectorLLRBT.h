#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

class SpaceSectorLLRBT {
public:
    Sector* root;
    SpaceSectorLLRBT();
    ~SpaceSectorLLRBT();
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);
    void preorderTraversal(Sector *node);
    void inorderTraversal(Sector *node);
    void postorderTraversal(Sector *node);
    void clear(Sector *node);
    Sector *insertSectorByCoordinatesRec(Sector *node, Sector *parent, int x, int y, int z);
    Sector *fixViolations(Sector *node);
    bool isRed(Sector *node);
    Sector *rotateRight(Sector *node);
    void flipColors(Sector *node);
    Sector *rotateLeft(Sector *pSector);
    void containsSectorCode(Sector *node, const std::string &target_sector_code, int &sectorX, int &sectorY, int &sectorZ,
                       bool &hasSectorCode);
    void earthSectorCode(Sector *node, int sectorX, int sectorY, int sectorZ, Sector *&earth);
};

#endif // SPACESECTORLLRBT_H
