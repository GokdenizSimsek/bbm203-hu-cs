#include "SpaceSectorBST.h"
#include <algorithm>

using namespace std;

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

SpaceSectorBST::~SpaceSectorBST() {
    // Free any dynamically allocated memory in this class.
    clear(root);
}

void SpaceSectorBST::clear(Sector* node) {
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

void SpaceSectorBST::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the BST sector map
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

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {
    // Instantiate and insert a new sector into the space sector BST map according to the 
    // coordinates-based comparison criteria.
    root = insertSectorByCoordinatesRec(root, x, y, z);
}

void SpaceSectorBST::deleteSector(const std::string& sector_code) {
    // TODO: Delete the sector given by its sector_code from the BST.
    int sectorX, sectorY, sectorZ;
    bool hasSector = false;
    containsSectorCode(root, sector_code, sectorX, sectorY, sectorZ , hasSector);
    if (hasSector) {
        root = deleteNode(root, sectorX, sectorY, sectorZ);
    }
}

void SpaceSectorBST::displaySectorsInOrder() {
    // TODO: Traverse the space sector BST map in-order and print the sectors 
    // to STDOUT in the given format.
    cout << "Space sectors inorder traversal:" << endl;
    inorderTraversal(root);
    cout << endl;
}

void SpaceSectorBST::displaySectorsPreOrder() {
    // TODO: Traverse the space sector BST map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
    cout << "Space sectors preorder traversal:" << endl;
    preorderTraversal(root);
    cout << endl;
}

void SpaceSectorBST::displaySectorsPostOrder() {
    // TODO: Traverse the space sector BST map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
    cout << "Space sectors postorder traversal:" << endl;
    postorderTraversal(root);
    cout << endl;
}

vector<Sector*> SpaceSectorBST::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!
    int sectorX, sectorY, sectorZ;
    bool hasSector = false;
    containsSectorCode(root, sector_code, sectorX, sectorY, sectorZ , hasSector);
    if (hasSector) {
        Sector *node = root;
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
    }
    return path;
}

void SpaceSectorBST::printStellarPath(const std::vector<Sector*>& path) {
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

Sector* SpaceSectorBST::insertSectorByCoordinatesRec(Sector* node, int x, int y, int z) {
    // Base case: Eğer düğüm boşsa, yeni bir düğüm oluştur ve değerleri ata.
    if (node == nullptr) {
        return new Sector(x, y, z);
    }

    // Recursive case: Koordinatları karşılaştır ve doğru alt ağaca git.
    if (x > node->x || (x == node->x && (y > node->y || (y == node->y && z > node->z)))) {
        node->right = insertSectorByCoordinatesRec(node->right, x, y, z);
    } else if (x < node->x || (x == node->x && (y < node->y || (y == node->y && z < node->z)))) {
        node->left = insertSectorByCoordinatesRec(node->left, x, y, z);
    }
    // Eğer x, y ve z değerleri mevcut düğümün değerleri ile aynıysa, hiçbir şey yapma (aynı düğümü tekrar eklememek için).

    return node;
}

Sector* SpaceSectorBST::deleteNode(Sector* root, int x, int y, int z) {
    if (root == nullptr) {
        return root;
    }

    // Aranan düğümü bul
    if (x < root->x) {
        root->left = deleteNode(root->left, x, y, z);
    } else if (x > root->x) {
        root->right = deleteNode(root->right, x, y, z);
    } else {
        if (y < root->y) {
            root->left = deleteNode(root->left, x, y, z);
        } else if (y > root->y) {
            root->right = deleteNode(root->right, x, y, z);
        } else {
            if (z < root->z) {
                root->left = deleteNode(root->left, x, y, z);
            } else if (z > root->z) {
                root->right = deleteNode(root->right, x, y, z);
            } else {
                // Durum 1: Leaf Node
                if (root->left == nullptr && root->right == nullptr) {
                    delete root;
                    return nullptr;
                }

                // Durum 2: Tek Çocuklu Node
                if (root->left == nullptr) {
                    Sector* temp = root->right;
                    delete root;
                    return temp;
                } else if (root->right == nullptr) {
                    Sector* temp = root->left;
                    delete root;
                    return temp;
                }

                // Durum 3: İki Çocuklu Node
                Sector* successor = findMin(root->right); // In-order successor
                root->x = successor->x;
                root->y = successor->y;
                root->z = successor->z;
                root->distance_from_earth = successor->distance_from_earth;
                root->sector_code = successor->sector_code;
                root->right = deleteNode(root->right, successor->x, successor->y, successor->z);
            }
        }
    }
    return root;
}

Sector* SpaceSectorBST::findMin(Sector* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

void SpaceSectorBST::containsSectorCode(Sector* node, const std::string& target_sector_code, int &sectorX, int &sectorY, int &sectorZ, bool &hasSectorCode) {
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

void SpaceSectorBST::preorderTraversal(Sector* node) {
    if (node) {
        // print parent
        std::cout << node->sector_code << endl;

        // print left child
        preorderTraversal(node->left);

        // print right child
        preorderTraversal(node->right);
    }
}

void SpaceSectorBST::inorderTraversal(Sector* node) {
    if (node) {
        // print left child
        inorderTraversal(node->left);

        // print parent
        cout << node->sector_code << endl;

        // print right child
        inorderTraversal(node->right);
    }
}

void SpaceSectorBST::postorderTraversal(Sector* node) {
    if (node) {
        // print left child
        postorderTraversal(node->left);

        // print right child
        postorderTraversal(node->right);

        // print parent
        std::cout << node->sector_code << endl;
    }
}