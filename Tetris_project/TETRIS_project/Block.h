#ifndef PA2_BLOCK_H
#define PA2_BLOCK_H

#include <vector>
#include <iostream>

using namespace std;

class Block {
public:

    vector<vector<bool>> shape;
    Block * right_rotation = nullptr;
    Block * left_rotation = nullptr;
    Block * next_block = nullptr;
    int rowCoor=0;
    int colCoor=0;
    int totalOnesCount=0;

    int totalOnesCounter(){
        totalOnesCount=0;
        for (const auto &innerVector : shape) {
            for (bool value : innerVector) {
                totalOnesCount += value ? 1 : 0;
            }
        }
        return totalOnesCount;
    }

    bool operator==(const Block& other) const {
        // TODO: Overload the == operator to compare two blocks based on their shapes
        return this->shape == other.shape;
    }

    bool operator!=(const Block& other) const {
        // TODO: Overload the != operator to compare two blocks based on their shapes
        return !(*this == other);
    }
    string block_print(){
        string res;
        int rows=shape.size();
        int cols=shape[0].size();
        for (size_t i = 0; i < rows; ++i) {
             for (size_t j = 0; j < cols; ++j) {
                 if(shape[i][j]==1){
                     res+=occupiedCellChar;
                 }else{
                     res+=unoccupiedCellChar;
                 }

             }
             res+="\n";

        }
        res+="\n";
        return res;
    }
};


#endif //PA2_BLOCK_H
