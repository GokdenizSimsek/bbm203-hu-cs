#include "GameController.h"
#include <fstream>
#include <chrono>
#include <iostream>

bool GameController::play(BlockFall& game, const string& commands_file){

    // TODO: Implement the gameplay here while reading the commands from the input file given as the 3rd command-line
    //       argument. The return value represents if the gameplay was successful or not: false if game over,
    //       true otherwise.


    int x = 0;
    int y_off = 0;
    int pieces = 0;

    ifstream file(commands_file);
    if (file.is_open()) {
        string line;

        bool gridIsFull = false;
        for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
            for (int j = 0; j < game.active_rotation->shape[0].size(); ++j) {
                if (game.grid[i][j] == 1) {
                    gridIsFull = true;
                }
            }
        }
        if (gridIsFull) {
            cout << "GAME FINISHED!" << endl << "Next block that couldn't fit:" << endl;
            for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                for (int j = 0; j < game.active_rotation->shape[0].size(); ++j) {
                    if (game.active_rotation->shape[i][j] == 0) {
                        cout << unoccupiedCellChar;
                    } else {
                        cout << occupiedCellChar;
                    }
                }
                cout << endl;
            }
            return false;
        } else {
            while (getline(file, line)) {

                if (line.find("PRINT_GRID") != std::string::npos) {
                    for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                        for (int j = 0; j < game.active_rotation->shape[0].size(); ++j) {
                            game.grid[i][j + x] = game.active_rotation->shape[i][j];
                        }
                    }

                    cout << "Score: " << game.current_score << endl << "High Score: " << game.high_score << endl;
                    for (int i = 0; i < game.rows; ++i) {
                        for (int j = 0; j < game.cols; ++j) {
                            if (game.grid[i][j] == 0) {
                                cout << unoccupiedCellChar;
                            } else {
                                cout << occupiedCellChar;
                            }
                        }
                        cout << endl;
                    }
                    cout << endl;

                    for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                        for (int j = 0; j < game.active_rotation->shape[0].size(); ++j) {
                            game.grid[i][j + x] = 0;
                        }
                    }

                } else if (line.find("ROTATE_RIGHT") != std::string::npos) {
                    bool rotateRight_ava = true;

                    if (game.active_rotation->right_rotation->shape[0].size() + x > game.cols) {
                        rotateRight_ava = false;
                    } else {
                        for (int i = 0; i < game.active_rotation->right_rotation->shape.size(); ++i) {
                            for (int j = 0; j < game.active_rotation->right_rotation->shape[0].size(); ++j) {
                                if (game.grid[i][j + x + 1] == 1 && game.active_rotation->right_rotation->shape[i][j] == 1) {
                                    rotateRight_ava = false;
                                }
                            }
                        }
                    }

                    if (rotateRight_ava) {
                        game.active_rotation = game.active_rotation->right_rotation;
                    }

                } else if (line.find("ROTATE_LEFT") != std::string::npos) {
                    bool rotateLeft_ava = true;

                    if (game.active_rotation->left_rotation->shape[0].size() + x > game.cols) {
                        rotateLeft_ava = false;
                    } else {
                        for (int i = 0; i < game.active_rotation->left_rotation->shape.size(); ++i) {
                            for (int j = 0; j < game.active_rotation->left_rotation->shape[0].size(); ++j) {
                                if (game.grid[i][j + x + 1] == 1 &&
                                    game.active_rotation->left_rotation->shape[i][j] == 1) {
                                    rotateLeft_ava = false;
                                }
                            }
                        }
                    }

                    if (rotateLeft_ava) {
                        game.active_rotation = game.active_rotation->left_rotation;
                    }

                } else if (line.find("MOVE_RIGHT") != std::string::npos) {
                    bool moveRight_ava = true;

                    if (game.active_rotation->shape[0].size() + x >= game.cols) {
                        moveRight_ava = false;
                    } else {
                        for (int j = game.active_rotation->shape[0].size() - 1; j >= 0; --j) {
                            for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                                if (game.grid[i][j + x + 1] == 1 && game.active_rotation->shape[i][j] == 1) {
                                    moveRight_ava = false;
                                }
                            }
                            if (moveRight_ava) {
                                for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                                    game.grid[i][j + x] = 0;
                                }
                            }
                        }
                    }

                    if (moveRight_ava) {
                        x++;
                    }

                } else if (line.find("MOVE_LEFT") != std::string::npos) {
                    bool moveLeft_ava = true;

                    if (x == 0) {
                        moveLeft_ava = false;
                    } else {
                        for (int j = 0; j < game.active_rotation->shape[0].size(); ++j) {
                            for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                                if (game.grid[i][j + x - 1] == 1 && game.active_rotation->shape[i][j] == 1) {
                                    moveLeft_ava = false;
                                }
                            }
                            if (moveLeft_ava) {
                                for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                                    game.grid[i][j + x] = 0;
                                }
                            }
                        }
                    }

                    if (moveLeft_ava) {
                        x--;
                    }

                } else if (line.find("DROP") != std::string::npos) {
                    y_off = 0;
                    pieces = 0;

                    for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                        for (int j = 0; j < game.active_rotation->shape[0].size(); ++j) {
                            if (game.active_rotation->shape[i][j] == 1) {
                                pieces++;
                            }
                        }
                    }
                    bool moveDown_ava = true;

                    while (moveDown_ava) {
                        if (game.active_rotation->shape.size() + y_off >= game.rows) {
                            moveDown_ava = false;
                        } else {
                            for (int i = game.active_rotation->shape.size() - 1; i >= 0; --i) {
                                for (int j = 0; j < game.active_rotation->shape[0].size(); ++j) {
                                    if (game.grid[i + y_off + 1][j + x] == 1 && game.active_rotation->shape[i][j] == 1) {
                                        moveDown_ava = false;
                                    }
                                }
                                if (moveDown_ava) {
                                    for (int j = 0; j < game.active_rotation->shape[0].size(); ++j) {
                                        if (game.active_rotation->shape[i][j] == 1) {
                                            game.grid[i + y_off][j + x] = 0;
                                        }
                                    }
                                }
                            }
                        }
                        if (moveDown_ava) {
                            y_off++;
                        }

                    }
                    game.current_score += y_off * pieces;


                    for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                        for (int j = 0; j < game.active_rotation->shape[0].size(); ++j) {
                            if (game.active_rotation->shape[i][j] == 1) {
                                game.grid[i + y_off][j + x] = game.active_rotation->shape[i][j];
                            }
                        }
                    }

                    if (game.gravity_mode_on) {
                        int j = game.cols - 1;
                        while (j >= 0) {
                            int count = 0;
                            for (int i = 0; i < game.rows; ++i) {
                                if (game.grid[i][j] == 1) {
                                    count++;
                                }
                            }
                            for (int i = game.rows - 1; i >= 0; --i) {
                                if (count > 0) {
                                    game.grid[i][j] = 1;
                                    count--;
                                } else {
                                    game.grid[i][j] = 0;
                                }
                            }
                            j--;
                        }
                    }

                    bool powerUp_detected = false;

                    for (int i = 0; i <= game.grid.size() - game.power_up.size() ; ++i) {
                        for (int j = 0; j <= game.grid[0].size() - game.power_up[0].size(); ++j) {
                            std::vector<std::vector<bool>> subMatrix;
                            for (int z = i; z < i + game.power_up.size(); ++z) {
                                std::vector<bool> sub_row;
                                for (int y = j; y < j + game.power_up[0].size(); ++y) {
                                    sub_row.push_back(game.grid[z][y]);
                                }
                                subMatrix.push_back(sub_row);
                            }
                            if (game.power_up == subMatrix) {
                                powerUp_detected = true;
                            }
                        }
                    }
                    pieces = 0;
                    if (powerUp_detected) {
                        cout << "Before clearing:" << endl;
                        for (int k = 0; k < game.grid.size(); ++k) {
                            for (int l = 0; l < game.grid[0].size(); ++l) {
                                if (game.grid[k][l] == 0) {
                                    cout << unoccupiedCellChar;
                                } else {
                                    cout << occupiedCellChar;
                                }
                            }
                            cout << endl;
                        }
                        for (int i = 0; i < game.rows; ++i) {
                            for (int j = 0; j < game.cols; ++j) {
                                if (game.grid[i][j] == 1) {
                                    pieces++;
                                    game.grid[i][j] = 0;
                                }
                            }
                        }
                        game.current_score += 1000 + pieces;

                    } else {
                        bool write_already = false;
                        int i = game.rows - 1;
                        while (i >= 0) {
                            bool all_isOne = true;
                            for (int j = 0; j < game.cols; ++j) {
                                if (game.grid[i][j] == 0) {
                                    all_isOne = false;
                                }
                            }
                            if (all_isOne) {
                                if (not write_already) {
                                    cout << "Before clearing:" << endl;
                                    for (int k = 0; k < game.grid.size(); ++k) {
                                        for (int l = 0; l < game.grid[0].size(); ++l) {
                                            if (game.grid[k][l] == 0) {
                                                cout << unoccupiedCellChar;
                                            } else {
                                                cout << occupiedCellChar;
                                            }
                                        }
                                        cout << endl;
                                    }
                                }
                                write_already = true;
                                game.current_score += game.cols;

                                for (int k = i; k > 0; --k) {
                                    game.grid[k] = game.grid[k - 1];
                                }
                                for (int j = 0; j < game.cols; ++j) {
                                    game.grid[0][j] = 0;
                                }
                                continue;
                            }
                            i--;
                        }
                    }
                    if (game.active_rotation->next_block != nullptr) {
                        game.active_rotation = game.active_rotation->next_block;
                        x = 0;
                        gridIsFull = false;

                        for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                            for (int j = 0; j < game.active_rotation->shape[0].size(); ++j) {
                                if (game.grid[i][j] == 1 && game.active_rotation->shape[i][j] == 1) {
                                    gridIsFull = true;
                                }
                            }
                        }
                        if (gridIsFull) {
                            cout << "GAME FINISHED!" << endl << "Next block that couldn't fit:" << endl;
                            for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                                for (int j = 0; j < game.active_rotation->shape[0].size(); ++j) {
                                    if (game.active_rotation->shape[i][j] == 0) {
                                        cout << unoccupiedCellChar;
                                    } else {
                                        cout << occupiedCellChar;
                                    }
                                }
                                cout << endl;
                            }
                            cout << "\nFinal grid and score:\n" << endl << "Score: " << game.current_score
                                       << endl << "High Score: " << game.high_score << endl;
                            for (int i = 0; i < game.rows; ++i) {
                                for (int j = 0; j < game.cols; ++j) {
                                    if (game.grid[i][j] == 0) {
                                        cout << unoccupiedCellChar;
                                    } else {
                                        cout << occupiedCellChar;
                                    }
                                }
                                cout << endl;
                            }
                            cout << endl;

                            auto now = std::chrono::system_clock::now();
                            time_t currentTime = std::chrono::system_clock::to_time_t(now);
                            auto * leaderboardEntry = new LeaderboardEntry(game.current_score,currentTime,game.player_name);

                            game.leaderboard.insert_new_entry(leaderboardEntry);
                            game.leaderboard.write_to_file(game.leaderboard_file_name);
                            game.leaderboard.print_leaderboard();
                            return false;
                        }
                    } else {
                    cout     << "YOU WIN!" << endl << "No more blocks." << endl;
                        cout << "Final grid and score:\n" << endl << "Score: " << game.current_score << endl << "High Score: " << game.high_score << endl;
                        for (int i = 0; i < game.rows; ++i) {
                            for (int j = 0; j < game.cols; ++j) {
                                if (game.grid[i][j] == 0) {
                                    cout << unoccupiedCellChar;
                                } else {
                                    cout << occupiedCellChar;
                                }
                            }
                            cout << endl;
                        }
                        cout << endl;
                        auto now = std::chrono::system_clock::now();
                        time_t currentTime = std::chrono::system_clock::to_time_t(now);
                        auto * leaderboardEntry = new LeaderboardEntry(game.current_score,currentTime,game.player_name);

                        game.leaderboard.insert_new_entry(leaderboardEntry);
                        game.leaderboard.write_to_file(game.leaderboard_file_name);
                        game.leaderboard.print_leaderboard();
                        return true;
                    }

                } else if (line.find("GRAVITY_SWITCH") != std::string::npos) {
                    if (game.gravity_mode_on) {
                        game.gravity_mode_on = false;
                    } else {
                        game.gravity_mode_on = true;

                        int k = game.cols - 1;
                        while (k >= 0) {
                            int count = 0;
                            for (int i = 0; i < game.rows; ++i) {
                                if (game.grid[i][k] == 1) {
                                    count++;
                                }
                            }
                            for (int i = game.rows - 1; i >= 0; --i) {
                                if (count > 0) {
                                    game.grid[i][k] = 1;
                                    count--;
                                } else {
                                    game.grid[i][k] = 0;
                                }
                            }
                            k--;
                        }
                    }
                    int i = game.rows - 1;
                    while (i >= 0) {
                        bool all_isOne = true;
                        for (int j = 0; j < game.cols; ++j) {
                            if (game.grid[i][j] == 0) {
                                all_isOne = false;
                            }
                        }
                        if (all_isOne) {
                            game.current_score += game.cols;
                            for (int k = i; k > 0; --k) {
                                game.grid[k] = game.grid[k - 1];
                            }
                            for (int j = 0; j < game.cols; ++j) {
                                game.grid[0][j] = 0;
                            }
                            continue;
                        }
                        i--;
                    }
                } else {
                    cout << "Undefined command!" << endl;

                }
            }
            if(file.eof()){
                cout << "GAME FINISHED!" << endl << "No more commands." << endl;
                cout << "Final grid and score:\n" << endl << "Score: " << game.current_score << endl << "High Score: " << game.high_score << endl;
                for (int i = 0; i < game.rows; ++i) {
                    for (int j = 0; j < game.cols; ++j) {
                        if (game.grid[i][j] == 0) {
                            cout << unoccupiedCellChar;
                        } else {
                            cout << occupiedCellChar;
                        }
                    }
                    cout << endl;
                }
                cout << endl;
                auto now = std::chrono::system_clock::now();
                time_t currentTime = std::chrono::system_clock::to_time_t(now);
                auto * leaderboardEntry = new LeaderboardEntry(game.current_score,currentTime,game.player_name);

                game.leaderboard.insert_new_entry(leaderboardEntry);
                game.leaderboard.write_to_file(game.leaderboard_file_name);
                game.leaderboard.print_leaderboard();
                return true;
            }
        }
    }
    return false;
}