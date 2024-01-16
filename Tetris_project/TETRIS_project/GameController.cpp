#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include "GameController.h"


bool GameController::play(BlockFall& game, const std::string& commands_file) {
    std::ifstream file(commands_file);

    if (!file.is_open()) {
        std::cerr << "File open error" << std::endl;
        return false;
    }

    std::string rowString;

    while (std::getline(file, rowString)) {
        std::vector<std::vector<int>> copyGrid = game.grid;

        if (rowString == "PRINT_GRID") {

            int blockRows = game.active_rotation->shape.size();
            int blockCols = game.active_rotation->shape[0].size();

            for (int i = 0; i < blockRows; ++i) {
                for (int j = 0; j < blockCols; ++j) {

                    copyGrid[i + game.active_rotation->rowCoor][j + game.active_rotation->colCoor] += game.active_rotation->shape[i][j] ;
                }
            }

            print_grid(copyGrid,game,false);

        }
        else if (rowString == "DROP") {

            int maxFallSteps = calculateMaxFallSteps(game, game.active_rotation);
            for (int step = 0; step < maxFallSteps; ++step) {
                if (isCollision(game, game.active_rotation, game.active_rotation->colCoor, game.active_rotation->rowCoor  + 1)) {
                    break;
                }
                game.active_rotation->rowCoor++;
            }

            game.current_score+=game.active_rotation->rowCoor*game.active_rotation->totalOnesCounter();


            bool e=overlapCellController(game,game.active_rotation);

            if(e){
                applyBlockToGrid(game, game.active_rotation);
            }else{
                BlockFilledTop= true;
                continue;
            }


            gravityFunc(game);
            if(containsSubVector(game.grid,game.power_up)){

                resultStr+="Before clearing:\n";
                //cout<<"Before clearing:\n";
                print_grid(game.grid,game, true);
                game.current_score+= clearGridAndRetOnesCount(game) + 1000;

            }

            if(rowFullChecker(game)){

                resultStr+="Before clearing:\n";
                //cout<<"Before clearing:\n";
                print_grid(game.grid,game,true);
                while(rowFullChecker(game)){
                    fullRowDeletetor(game);
                }
            }

            if( game.active_rotation->next_block!= nullptr){
                game.active_rotation=game.active_rotation->next_block;
                bool e=overlapCellController(game,game.active_rotation);
                if(! e){

                    resultStr+="GAME OVER!\nNext block that couldn't fit:\n";
                    resultStr+=game.active_rotation->block_print() + "Final grid and score:\n\n";
                    print_grid(game.grid,game, false);

                    auto now = std::chrono::system_clock::now();

                    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

                    LeaderboardEntry* l1= new LeaderboardEntry(game.current_score,currentTime,game.player_name);
                    game.leaderboard.insert_new_entry(l1);

                    game.leaderboard.write_to_file(game.leaderboard_file_name);

                    allStepsPrinter();
                    game.leaderboard.print_leaderboard();

                    file.close();
                    return false;

                }

            }else{

                resultStr+="YOU WIN!\nNo more blocks.\nFinal grid and score:\n\n";
                print_grid(game.grid,game, false);

                auto now = std::chrono::system_clock::now();

                std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

                LeaderboardEntry* l1= new LeaderboardEntry(game.current_score,currentTime,game.player_name);
                game.leaderboard.insert_new_entry(l1);
                game.leaderboard.write_to_file(game.leaderboard_file_name);

                allStepsPrinter();
                game.leaderboard.print_leaderboard();

                file.close();
                return true;

            }

        }
        else if (rowString == "MOVE_RIGHT") {
            int newCol=game.active_rotation->colCoor +1;
            if (boundaryCollisionCheck(game, game.active_rotation,false) &&!isCollisionWithOtherBlock(game, game.active_rotation, newCol)) {

                game.active_rotation->colCoor = newCol;
            }

        }
        else if (rowString == "MOVE_LEFT") {
            int newCol=game.active_rotation->colCoor -1;
            if (boundaryCollisionCheck(game, game.active_rotation, true) &&!isCollisionWithOtherBlock(game, game.active_rotation, newCol)) {
                game.active_rotation->colCoor = newCol;
            }

        }
        else if (rowString == "GRAVITY_SWITCH") {
            game.gravityChange();
            gravityFunc(game);
            if(rowFullChecker(game)){

                while(rowFullChecker(game)){
                    fullRowDeletetor(game);
                }
            }

        }
        else if (rowString == "ROTATE_RIGHT") {

            Block* tempBlock=game.active_rotation;
            game.active_rotation=game.active_rotation->right_rotation;
            game.active_rotation->colCoor=tempBlock->colCoor;
            game.active_rotation->rowCoor=tempBlock->rowCoor;
            if( ! rotateColliCheck(game,game.active_rotation)){
                game.active_rotation=tempBlock;
            }

        }
        else if (rowString == "ROTATE_LEFT") {
            Block* tempBlock=game.active_rotation;
            game.active_rotation=game.active_rotation->left_rotation;
            game.active_rotation->colCoor=tempBlock->colCoor;
            game.active_rotation->rowCoor=tempBlock->rowCoor;
            if( !rotateColliCheck(game,game.active_rotation)){

                game.active_rotation=tempBlock;
            }

        }
        else {

            resultStr+="Unknown command: "+rowString+"\n";
        }
    }


    resultStr += "GAME FINISHED!\nNo more commands.\nFinal grid and score:\n\n";
    print_grid(game.grid,game, false);

    auto now = std::chrono::system_clock::now();

    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    LeaderboardEntry* l1= new LeaderboardEntry(game.current_score,currentTime,game.player_name);
    game.leaderboard.insert_new_entry(l1);
    game.leaderboard.write_to_file(game.leaderboard_file_name);

    allStepsPrinter();
    game.leaderboard.print_leaderboard();

    file.close();
    return true;
}

void GameController::fullRowDeletetor(BlockFall &game) {
    int numRows = game.grid.size();
    int numCols = game.grid[0].size();

    for (int i = 0; i < numRows; ++i) {
        bool isRowOne = true;

        for (int j = 0; j < numCols; ++j) {
            if (game.grid[i][j] != 1) {
                isRowOne = false;
                break;
            }
        }

        if (isRowOne) {
            for (int j = 0; j < numCols; ++j) {
                game.grid[i][j] = 0;
            }
            game.current_score+=numCols;

            for (int k = i; k > 0; --k) {
                std::swap(game.grid[k], game.grid[k - 1]);
            }
        }
    }

}


int GameController::clearGridAndRetOnesCount(BlockFall &game) {
    vector<vector<int> > grid=game.grid;
    int rows = grid.size();
    int cols = grid[0].size();
    int onesCount = 0;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == 1) {
                onesCount++;
                grid[i][j] = 0;

            }
        }
    }
    game.grid=grid;
    return onesCount;
}

bool GameController::rowFullChecker(BlockFall &game) {
    for (const auto& row : game.grid) {
        bool allOnes = true;
        for (int element : row) {
            if (element != 1) {
                allOnes = false;
                break;
            }
        }
        if (allOnes) {
            return true;
        }
    }
    return false;

}


bool GameController::containsSubVector(const std::vector<std::vector<int>>& bigVector, const std::vector<std::vector<bool>>& smallVector) {
    for (size_t i = 0; i <= bigVector.size() - smallVector.size(); ++i) {
        for (size_t j = 0; j <= bigVector[i].size() - smallVector[0].size(); ++j) {
            bool found = true;

            for (size_t k = 0; k < smallVector.size(); ++k) {
                for (size_t l = 0; l < smallVector[k].size(); ++l) {
                    if (bigVector[i + k][j + l] != (smallVector[k][l] ? 1 : 0)) {
                        found = false;
                        break;
                    }
                }
                if (!found) {
                    break;
                }
            }

            if (found) {
                return true;
            }
        }
    }

    return false;
}



bool GameController::rotateColliCheck(BlockFall &game, const Block *block) {
        return block->shape[0].size() + block->colCoor -1 <game.grid[0].size();
}

void GameController::gravityFunc(BlockFall& game) {
    if (game.gravity_mode_on) {
        for (int col = 0; col < game.cols; ++col) {
            for (int row = game.rows - 1; row >= 0; --row) {
                if (game.grid[row][col] == 1) {
                    int fallToRow = row + 1;
                    while (fallToRow < game.rows && game.grid[fallToRow][col] == 0) {
                        ++fallToRow;
                    }

                    if (fallToRow - 1 != row) {
                        game.grid[fallToRow - 1][col] = 1;
                        game.grid[row][col] = 0;
                    }
                }
            }
        }
    }
}




int GameController::calculateMaxFallSteps(const BlockFall& game, const Block* block) const {
    int maxFallSteps = game.rows - block->rowCoor - block->shape.size();
    return maxFallSteps;
}

bool GameController::overlapCellController(BlockFall &game, const Block *block) {

    for (size_t i = 0; i < block->shape.size(); ++i) {
        for (size_t j = 0; j < block->shape[i].size(); ++j) {
            if (block->shape[i][j] == 1 && game.grid[block->rowCoor + i][block->colCoor + j] == 1) {
                return false;
            }
        }
    }
    return true;
}

void GameController::applyBlockToGrid(BlockFall& game, const Block* block) {
    for (size_t i = 0; i < block->shape.size(); ++i) {
        for (size_t j = 0; j < block->shape[i].size(); ++j) {
            if (block->shape[i][j] == 1) {
                game.grid[block->rowCoor + i][block->colCoor + j] = 1;
            }
        }
    }

}

bool GameController::isCollision(const BlockFall& game, const Block* block, int col, int row) const {
    for (size_t i = 0; i < block->shape.size(); ++i) {
        for (size_t j = 0; j < block->shape[i].size(); ++j) {
            if (block->shape[i][j] == 1) {
                if (row + i >= game.rows || col + j < 0 || col + j >= game.cols) {

                    return true;
                }
                if (game.grid[row + i][col + j] == 1) {
                    return true;
                }
            }
        }
    }
    return false;
}


bool GameController::boundaryCollisionCheck(BlockFall& game, Block* block, bool moveLeft) {
    int blockCols = block->shape[0].size();

    if (moveLeft) {
        return block->colCoor > 0;
    } else {

        return block->colCoor + blockCols < game.cols;
    }
}

void GameController::print_grid(const std::vector<std::vector<int>>& grid,BlockFall& game,bool isbeforeClearing) {
    if( !isbeforeClearing){
        resultStr+= "Score: " ;

        resultStr+=std::to_string(game.current_score);
        resultStr+="\n";
        if(game.leaderboard.head_leaderboard_entry == nullptr){
            resultStr+= "High Score: 0";

        }else{
            resultStr+= "High Score: "+   std::to_string(game.leaderboard.head_leaderboard_entry->score);

        }
        resultStr+="\n";
    }

    
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[0].size(); ++j) {
            if(grid[i][j]==1){
                resultStr+= occupiedCellChar ;

            }else{
                resultStr+= unoccupiedCellChar ;

            }

        }

        resultStr+="\n";
    }
    //cout<<endl<<endl;
    resultStr+="\n\n";
}

void GameController::toTXTFile(std::string fileName) {
    std::ios_base::openmode mod = std::ios_base::out;
    std::ofstream dosya(fileName, mod);

    if (dosya.is_open()) {
        dosya << resultStr << std::endl;

        dosya.close();

    }  else {
        std::cerr << "Dosya açılamadı veya yazma hatası oluştu." << std::endl;
        if (dosya.fail()) {
            std::cerr << "Dosya durumu: HATA" << std::endl;
        }
    }
}

bool GameController::isCollisionWithOtherBlock(BlockFall& game, Block* block, int newCol) const {
    for (size_t i = 0; i < block->shape.size(); ++i) {
        for (size_t j = 0; j < block->shape[i].size(); ++j) {
            if (block->shape[i][j] == 1) {
                if (newCol + j < 0 || newCol + j >= game.cols || game.grid[block->rowCoor + i][newCol + j] == 1) {

                    return true;
                }
            }
        }
    }
    return false;
}



void GameController::allStepsPrinter() {
    cout<<resultStr<<endl;
}
