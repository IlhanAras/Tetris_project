#ifndef PA2_LEADERBOARD_H
#define PA2_LEADERBOARD_H

#include <ctime>
#include <string>
#include "LeaderboardEntry.h"

#define MAX_LEADERBOARD_SIZE 10

using namespace std;

class Leaderboard {
public:

    int playerCount=0;
    LeaderboardEntry* head_leaderboard_entry = nullptr;
    void read_from_file(const string &filename);
    void write_to_file(const string &filename);
    std::string formatTimestamp(time_t timestamp);
    void remove_LastEntry();
    void print_leaderboard();
    void insert_new_entry(LeaderboardEntry *new_entry);
    string leaderboardtoResultstr();
    virtual ~Leaderboard();
};


#endif //PA2_LEADERBOARD_H
