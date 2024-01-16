#include "BlockFall.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>


BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}

void BlockFall::read_blocks(const string &input_file) {
    std::ifstream file(input_file);

    if (!file.is_open()) {
        std::cerr << "File open error" <<input_file<< std::endl;
        return;
    }

    std::string rowString;
    std::vector<std::vector<bool>> blockArray;

    while (std::getline(file, rowString)) {
        std::stringstream ss(rowString);

        std::vector<bool> row;

        if (rowString.empty()) {
            blockArray.clear();
            continue;
        }

        if (rowString.find('[') != std::string::npos && rowString.find(']') != std::string::npos) {
            char c;
            while (ss >> c) {
                if (c == '0' || c == '1') {
                    row.push_back(c - '0');
                }
            }
            blockArray.push_back(row);

            Block* block = new Block();

            block->shape = blockArray;
            active_rotation=block;
            for(int i=0; i<3; i++){
                Block* rotaBlock = new Block();

                active_rotation->right_rotation=rotaBlock;
                rotaBlock->left_rotation=active_rotation;
                rotaBlock->shape= rotateClockwise(active_rotation->shape);
                active_rotation=rotaBlock;
            }
            active_rotation->right_rotation=block;
            block->left_rotation=active_rotation;
            active_rotation=active_rotation->right_rotation;

            if (initial_block == nullptr) {
                initial_block = block;
            } else {
                Block* current_block = initial_block;
                while (current_block->next_block != nullptr) {
                    current_block = current_block->next_block;
                }
                current_block->next_block = block;
                current_block->right_rotation->next_block=block;
                current_block->right_rotation->right_rotation->next_block=block;
                current_block->left_rotation->next_block=block;
            }

            blockArray.clear();
            continue;
        }

        else if (rowString.find('[')!= std::string::npos ) {
            char c;
            while (ss >> c) {
                if (c == '0' || c == '1') {
                    row.push_back(c - '0');
                }
            }
            blockArray.push_back(row);

        }
        else if (rowString.find('[') == std::string::npos && rowString.find(']') == std::string::npos  ) {
            char c;
            while (ss >> c) {
                if (c == '0' || c == '1') {
                    row.push_back(c - '0');
                }
            }
            blockArray.push_back(row);

        }
        else if (rowString.find(']') != std::string::npos ) {
            char c;
            while (ss >> c) {
                if (c == '0' || c == '1') {
                    row.push_back(c - '0');
                }
            }
            blockArray.push_back(row);

            Block* block = new Block();


            block->shape = blockArray;
            active_rotation=block;
            for(int i=0; i<3; i++){
                Block* rotaBlock = new Block();


                active_rotation->right_rotation=rotaBlock;
                rotaBlock->left_rotation=active_rotation;
                rotaBlock->shape= rotateClockwise(active_rotation->shape);
                active_rotation=rotaBlock;
            }
            active_rotation->right_rotation=block;
            block->left_rotation=active_rotation;
            active_rotation=active_rotation->right_rotation;

            if (initial_block == nullptr) {
                initial_block = block;
            } else {
                Block* current_block = initial_block;
                while (current_block->next_block != nullptr) {
                    current_block = current_block->next_block;
                }
                current_block->next_block = block;
                current_block->right_rotation->next_block=block;
                current_block->right_rotation->right_rotation->next_block=block;
                current_block->left_rotation->next_block=block;
            }

            blockArray.clear();
            continue;
        }
    }
    active_rotation=initial_block;
    powerUpSeparator();

    file.close();
}

void BlockFall::powerUpSeparator() {

    Block* temp=initial_block;
    while(temp->next_block->next_block!= nullptr){
        temp=temp->next_block;
    }
    power_up= temp->next_block->shape;

    Block* lookhead=temp->next_block;
    for(int i=0; i<3; i++){
        Block* deletor=lookhead;
        lookhead=lookhead->right_rotation;
        delete deletor;
    }
    delete lookhead;
    temp->next_block= nullptr;
    temp->right_rotation->next_block= nullptr;
    temp->left_rotation->next_block= nullptr;
    temp->right_rotation->right_rotation->next_block= nullptr;

}


std::vector<vector<bool>> BlockFall::rotateClockwise(const vector<vector<bool>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    vector<vector<bool>> rotatedMatrix(cols, vector<bool>(rows, 0));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            rotatedMatrix[j][rows - 1 - i] = matrix[i][j];
        }
    }

    return rotatedMatrix;
}
void BlockFall::initialize_grid(const string &input_file) {
    // TODO: Initialize "rows" and "cols" member variables
    // TODO: Initialize "grid" member variable using the command-line argument 1 in main
    std::ifstream file(input_file);

    if (!file.is_open()) {
        std::cerr << "File open error "  << std::endl;
        return;
    }

    std::string satir;
    while (std::getline(file, satir)) {
        std::vector<int> satirVerisi;
        std::istringstream stream(satir);
        int deger;

        while (stream >> deger) {
            satirVerisi.push_back(deger);
        }

        grid.push_back(satirVerisi);
    }

    file.close();

    rows = grid.size();
    cols = (rows > 0) ? grid[0].size() : 0;

}

void BlockFall::gravityChange() {
    if(!gravity_mode_on){
        gravity_mode_on= true;
    }
    else{
        gravity_mode_on= false;
    }
}


BlockFall::~BlockFall() {
    // TODO: Free dynamically allocated memory used for storing game blocks

    Block* currentBlock = initial_block;

    while (currentBlock != nullptr) {
        for (int i = 0; i < 3; i++) {
            Block* temp = currentBlock;
            currentBlock = currentBlock->right_rotation;
            delete temp;
        }

        Block* temp = currentBlock;
        currentBlock = currentBlock->next_block;
        delete temp;
    }

    initial_block = nullptr;

}
