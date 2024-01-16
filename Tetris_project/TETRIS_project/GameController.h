#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H

#include "BlockFall.h"

using namespace std;

class GameController {
public:

    string resultStr;
    bool BlockFilledTop= false;
    bool isNoMoreBlock= false;
    bool play(BlockFall &game, const string &commands_file);
    void print_grid(const std::vector<std::vector<int>>& grid,BlockFall& game,bool isbeforeClearing);
    bool boundaryCollisionCheck(BlockFall& game, Block* block, bool moveLeft);
    int calculateMaxFallSteps(const BlockFall& game, const Block* block) const;
    void applyBlockToGrid(BlockFall& game, const Block* block);
    bool isCollision(const BlockFall& game, const Block* block, int col, int row) const;
    bool rotateColliCheck(BlockFall& game, const Block* block);
    void gravityFunc(BlockFall& game);
    bool containsSubVector(const std::vector<std::vector<int>>& bigVector, const std::vector<std::vector<bool>>& smallVector);
    int clearGridAndRetOnesCount(BlockFall& game);
    bool rowFullChecker(BlockFall& game);
    void toTXTFile(string fileName);
    void fullRowDeletetor(BlockFall& game);
    bool overlapCellController(BlockFall& game, const Block* block);
    bool isCollisionWithOtherBlock(BlockFall& game, Block* block, int newCol) const;
    void allStepsPrinter();

};


#endif //PA2_GAMECONTROLLER_H
