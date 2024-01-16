#ifndef BLOCKFALL_H
#define BLOCKFALL_H


#define occupiedCellChar "██"
#define unoccupiedCellChar "▒▒"

#include <vector>
#include <string>

#include "Block.h"
#include "LeaderboardEntry.h"
#include "Leaderboard.h"

using namespace std;

class BlockFall {
public:

    BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name,
              const string &player_name);
    virtual ~BlockFall();

    int rows;
    int cols;
    vector<vector<int> > grid;
    vector<vector<bool>> power_up;
    Block * initial_block = nullptr;
    Block * active_rotation = nullptr;
    bool gravity_mode_on = false;
    unsigned long current_score = 0;
    string leaderboard_file_name;
    string player_name;
    Leaderboard leaderboard;


    void powerUpSeparator();
    vector<vector<bool>> rotateClockwise(const vector<vector<bool>>& matrix);
    void initialize_grid(const string & input_file);
    void read_blocks(const string & input_file);
    void gravityChange();
};



#endif // BLOCKFALL_H
