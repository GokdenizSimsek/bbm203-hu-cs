#include "BlockFall.h"
#include <fstream>
#include <sstream>
#include <iostream>

BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}

vector<vector<bool>> BlockFall::right_rotate(vector<vector<bool>> shape) {
    size_t block_rows = shape.size();
    size_t block_cols = shape[0].size();

    std::vector<std::vector<bool>> rightRotatedMatrix(block_cols, std::vector<bool>(block_rows));
    for (size_t i = 0; i < block_rows; ++i) {
        for (size_t j = 0; j < block_cols; ++j) {
            rightRotatedMatrix[j][block_rows - 1 - i] = shape[i][j];
        }
    }
    return rightRotatedMatrix;
}

void BlockFall::read_blocks(const string &input_file) {
    ifstream file(input_file);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            // Skip empty lines
            if (line.empty()) {
                continue;
            }

            Block * new_block = new Block;

            // Parse the line to create a new block and add it to the linked list
            do {
                std::vector<bool> row;
                if (line.empty()) {
                    continue;
                }
                for (char c : line) {
                    if (c == '0') {
                        row.push_back(false);
                    } else if (c == '1') {
                        row.push_back(true);
                    }
                }
                new_block->shape.push_back(row);
            } while (not line.empty() and getline(file, line));

            if (prevBlock != nullptr) {
                prevBlock->next_block = new_block;
                prevBlock->left_rotation->next_block = new_block;
                prevBlock->left_rotation->left_rotation->next_block = new_block;
                prevBlock->left_rotation->left_rotation->left_rotation->next_block = new_block;
            } else {
                initial_block = new_block;
                active_rotation = new_block;
            }

            if (new_block->shape == right_rotate(new_block->shape)) {
                new_block->right_rotation = new_block;
                new_block->left_rotation = new_block;

            } else if (new_block->shape == right_rotate(right_rotate(new_block->shape))) {
                Block * rightRotation1 = new Block;
                rightRotation1->shape = right_rotate(new_block->shape);
                new_block->right_rotation = rightRotation1;
                new_block->left_rotation = rightRotation1;

                new_block->left_rotation->left_rotation = new_block;
                new_block->right_rotation->right_rotation = new_block;

            } else {
                Block * rightRotation1 = new Block;
                rightRotation1->shape = right_rotate(new_block->shape);

                Block * rightRotation2 = new Block;
                rightRotation2->shape = right_rotate(right_rotate(new_block->shape));

                Block * rightRotation3 = new Block;
                rightRotation3->shape = right_rotate(right_rotate(right_rotate(new_block->shape)));

                new_block->right_rotation = rightRotation1;
                new_block->left_rotation = rightRotation3;

                rightRotation1->right_rotation = rightRotation2;
                rightRotation1->left_rotation = new_block;

                rightRotation2->right_rotation = rightRotation3;
                rightRotation2->left_rotation = rightRotation1;

                rightRotation3->right_rotation = new_block;
                rightRotation3->left_rotation = rightRotation2;
            }

            prevBlock = new_block;
        }

        // Close the file
        file.close();
    } else {
        cerr << "Error: Unable to open file " << input_file << endl;
    }

    Block * currentBlock = initial_block;

    do {
        currentBlock = currentBlock->next_block;
        if (currentBlock->next_block->next_block == nullptr) {
            power_up = currentBlock->next_block->shape;
            delete currentBlock->next_block;

            currentBlock->next_block = nullptr;
            currentBlock->left_rotation->next_block = nullptr;
            currentBlock->left_rotation->left_rotation->next_block = nullptr;
            currentBlock->left_rotation->left_rotation->left_rotation->next_block = nullptr;
        }
    } while (currentBlock->next_block != nullptr && currentBlock->next_block->next_block != nullptr);
}

void BlockFall::initialize_grid(const string &input_file) {
    std::ifstream file(input_file); // Replace "matrix.txt" with your file name
    std::vector<std::vector<int>> matrix;

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::vector<int> row;
            std::istringstream iss(line);
            int num;

            while (iss >> num) {
                row.push_back(num);
            }

            matrix.push_back(row);
        }

        file.close();

        rows = matrix.size();
        cols = matrix[0].size();
        grid = matrix;

    } else {
        std::cerr << "Unable to open file." << std::endl;
    }
}

BlockFall::~BlockFall() {
    // TODO: Free dynamically allocated memory used for storing game blocks
    Block *current_block = initial_block;

    while (current_block != nullptr) {
        Block *block_to_del = current_block;
        current_block = current_block->next_block;
        if (block_to_del->left_rotation != block_to_del->right_rotation) {
            delete block_to_del->right_rotation->right_rotation;
            delete block_to_del->right_rotation;
            delete block_to_del->left_rotation;
        } else if (block_to_del != block_to_del->right_rotation) {
            delete block_to_del->right_rotation;
        }
        delete block_to_del;
    }
}