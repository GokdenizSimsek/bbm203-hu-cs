#include <algorithm>
#include "SpaceSectorLLRBT.h"

using namespace std;

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the LLRBT sector map
    // according to the given comparison critera based on the sector coordinates.
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        getline(file, line);
        while (getline(file, line)) {
            istringstream iss(line);
            vector<std::string> coordinates;
            string coordinate;

            while (getline(iss, coordinate, ',')) {
                coordinates.push_back(coordinate);
            }
            insertSectorByCoordinates(std::stoi(coordinates[0]), std::stoi(coordinates[1]), std::stoi(coordinates[2]));
        }
    }
}

// Remember to handle memory deallocation properly in the destructor.
SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    // TODO: Free any dynamically allocated memory in this class.
    clear(root);
}

void SpaceSectorLLRBT::clear(Sector* node) {
    // Post-order traversal ile düğümleri sil.
    if (node) {
        // Sol alt ağacı temizle
        clear(node->left);

        // Sağ alt ağacı temizle
        clear(node->right);

        // Düğümü sil
        delete node;
    }
}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    // TODO: Instantiate and insert a new sector into the space sector LLRBT map 
    // according to the coordinates-based comparison criteria.
    root = insertSectorByCoordinatesRec(root, nullptr, x, y, z);
    if (root) {
        root->color = BLACK;
    }
}

Sector* SpaceSectorLLRBT::insertSectorByCoordinatesRec(Sector* node, Sector* parent, int x, int y, int z) {
    if (node == nullptr) {
        Sector *nodeX = new Sector(x, y, z);
        nodeX->parent = parent;
        return nodeX;
    }

    if (x < node->x || (x == node->x && (y < node->y || (y == node->y && z < node->z)))) {
        node->left = insertSectorByCoordinatesRec(node->left, node, x, y, z);
    } else if (x > node->x || (x == node->x && (y > node->y || (y == node->y && z > node->z)))) {
        node->right = insertSectorByCoordinatesRec(node->right, node, x, y, z);
    }

    // Fix violations after insertion
    node = fixViolations(node);

    return node;
}

Sector* SpaceSectorLLRBT::fixViolations(Sector* node) {
    // Sağ çocuk RED ise sola döndür
    if (!isRed(node->left) && isRed(node->right)) {
        node = rotateLeft(node);
    }

    // Sol çocuğu RED ve solun sol çocuğu da RED ise sağa döndür
    if (isRed(node->left) && isRed(node->left->left)) {
        node = rotateRight(node);
    }

    // Sol ve sağ çocuklar RED ise renkleri değiştir
    if (isRed(node->left) && isRed(node->right)) {
        flipColors(node);

    }

    return node;
}

bool SpaceSectorLLRBT::isRed(Sector* node) {
    return node && node->color;
}

Sector* SpaceSectorLLRBT::rotateRight(Sector* node) {
    Sector* newRoot = node->left;
    node->left = newRoot->right;
    if (newRoot->right) {
        newRoot->right->parent = node;
    }
    newRoot->right = node;
    newRoot->color = node->color;
    node->color = RED;

    newRoot->parent = node->parent;
    node->parent = newRoot;
    if (node->left) {
        node->left->parent = node;
    }

    return newRoot;
}

Sector *SpaceSectorLLRBT::rotateLeft(Sector *node) {
    Sector* newRoot = node->right;
    node->right = newRoot->left;
    if (newRoot->left) {
        newRoot->left->parent = node;
    }
    newRoot->left = node;
    newRoot->color = node->color;
    node->color = RED;

    newRoot->parent = node->parent;
    node->parent = newRoot;
    if (node->right) {
        node->right->parent = node;
    }

    return newRoot;
}

void SpaceSectorLLRBT::flipColors(Sector* node) {
    node->color = RED;
    node->left->color = BLACK;
    node->right->color = BLACK;
}

void SpaceSectorLLRBT::displaySectorsInOrder() {
    // TODO: Traverse the space sector LLRBT map in-order and print the sectors 
    // to STDOUT in the given format.
    cout << "Space sectors inorder traversal:" << endl;
    inorderTraversal(root);
    cout << endl;
}

void SpaceSectorLLRBT::displaySectorsPreOrder() {
    // TODO: Traverse the space sector LLRBT map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
    cout << "Space sectors preorder traversal:" << endl;
    preorderTraversal(root);
    cout << endl;
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    // TODO: Traverse the space sector LLRBT map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
    cout << "Space sectors postorder traversal:" << endl;
    postorderTraversal(root);
    cout << endl;
}

vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!
    int sectorX, sectorY, sectorZ;
    Sector *earth;
    bool hasSector = false;
    containsSectorCode(root, sector_code, sectorX, sectorY, sectorZ , hasSector);
    earthSectorCode(root, 0, 0, 0, earth);
    if (hasSector) {
        hasSector = false;
        while (true) {
            containsSectorCode(earth, sector_code, sectorX, sectorY, sectorZ , hasSector);
            if (hasSector) {
                Sector *node = earth;
                while (true) {
                    if (sectorX > node->x) {
                        path.push_back(node);
                        node = node->right;
                    } else if (sectorX < node->x) {
                        path.push_back(node);
                        node = node->left;
                    } else {
                        if (sectorY > node->y) {
                            path.push_back(node);
                            node = node->right;
                        } else if (sectorY < node->y) {
                            path.push_back(node);
                            node = node->left;
                        } else {
                            if (sectorZ > node->z) {
                                path.push_back(node);
                                node = node->right;
                            } else if (sectorZ < node->z) {
                                path.push_back(node);
                                node = node->left;
                            } else {
                                path.push_back(node);
                                break;
                            }
                        }
                    }
                }
                break;
            } else {
                if (earth->parent != nullptr) {
                    path.push_back(earth);
                    earth = earth->parent;
                } else {break;}
            }
        }

    }

    return path;
}

void SpaceSectorLLRBT::containsSectorCode(Sector* node, const std::string& target_sector_code, int &sectorX, int &sectorY, int &sectorZ, bool &hasSectorCode) {
    if (node == nullptr) {
        return;
    }

    if (node->sector_code == target_sector_code) {
        sectorX = node->x;
        sectorY = node->y;
        sectorZ = node->z;
        hasSectorCode = true;
        return;
    }

    containsSectorCode(node->left, target_sector_code, sectorX, sectorY, sectorZ, hasSectorCode);
    containsSectorCode(node->right, target_sector_code, sectorX, sectorY, sectorZ, hasSectorCode);
}

void SpaceSectorLLRBT::earthSectorCode(Sector* node, int earthX, int earthY, int earthZ, Sector*& earth) {
    if (node == nullptr) {
        return;
    }
    if (earthX < node->x || (earthX == node->x && (earthY < node->y || (earthY == node->y && earthZ < node->z)))) {
        earthSectorCode(node->left, earthX, earthY, earthZ, earth);
    } else if (earthX > node->x || (earthX == node->x && (earthY > node->y || (earthY == node->y && earthZ > node->z)))) {
        earthSectorCode(node->right, earthX, earthY, earthZ, earth);
    } else {
        earth = node;
        return;
    }
}

void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
    // TODO: Print the stellar path obtained from the getStellarPath() function 
    // to STDOUT in the given format.
    if (!path.empty()) {
        cout << "The stellar path to Dr. Elara: ";
        for (int i = 0; i < path.size(); ++i) {
            if (i != path.size() - 1) {
                cout << path[i]->sector_code << "->";
            } else {
                cout << path[i]->sector_code << endl;
            }
        }
    } else {
        cout << "A path to Dr. Elara could not be found." << endl;
    }
}

void SpaceSectorLLRBT::preorderTraversal(Sector* node) {
    if (node) {
        // print parent
        if (node->color) {
            cout << "RED sector: ";
        } else {
            cout << "BLACK sector: ";
        }
        cout << node->sector_code << endl;

        // print left child
        preorderTraversal(node->left);

        // print right child
        preorderTraversal(node->right);
    }
}

void SpaceSectorLLRBT::inorderTraversal(Sector* node) {
    if (node) {
        // print left child
        inorderTraversal(node->left);

        // print parent
        if (node->color) {
            cout << "RED sector: ";
        } else {
            cout << "BLACK sector: ";
        }
        cout << node->sector_code << endl;

        // print right child
        inorderTraversal(node->right);
    }
}

void SpaceSectorLLRBT::postorderTraversal(Sector* node) {
    if (node) {
        // print left child
        postorderTraversal(node->left);

        // print right child
        postorderTraversal(node->right);

        // print parent
        if (node->color) {
            cout << "RED sector: ";
        } else {
            cout << "BLACK sector: ";
        }
        cout << node->sector_code << endl;
    }
}
