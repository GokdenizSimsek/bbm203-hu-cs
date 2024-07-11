#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

#include "Sector.h"

class SpaceSectorBST {
  
public:
    Sector *root;
    Sector *traverseNode;
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
    void preorderTraversal(Sector* root);
    void inorderTraversal(Sector* root);
    void postorderTraversal(Sector* root);
    void containsSectorCode(Sector* node, const std::string& target_sector_code, int &sectorX, int &sectorY, int &sectorZ, bool &hasSectorCode);
    Sector *findMin(Sector *node);
    Sector *deleteNode(Sector *root, int x, int y, int z);
    Sector *insertSectorByCoordinatesRec(Sector *node, int x, int y, int z);

    void clear(Sector *node);
};

#endif // SPACESECTORBST_H
