
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include "Leaderboard.h"

void Leaderboard::read_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << " for reading\n";
        return;
    }

    unsigned long score;
    time_t last_played;
    std::string player_name;



    while (file >> score >> last_played >> player_name) {
        LeaderboardEntry* playerEntry = new LeaderboardEntry(score, last_played, player_name);
        insert_new_entry(playerEntry);
    }


    file.close();
}

void Leaderboard::write_to_file(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << " for writing\n";
        return;
    }

    LeaderboardEntry* temp = head_leaderboard_entry;
    while (temp) {
        file << temp->score << " " << temp->last_played << " " << temp->player_name << '\n';
        temp = temp->next_leaderboard_entry;
    }

    file.close();
}

std::string Leaderboard::formatTimestamp(time_t timestamp) {
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(timestamp);

    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M:%S/%d.%m.%Y");

    return oss.str();
}

void Leaderboard::remove_LastEntry() {

    LeaderboardEntry* temp = head_leaderboard_entry;
    while (temp->next_leaderboard_entry->next_leaderboard_entry) {
        temp = temp->next_leaderboard_entry;
    }

    delete temp->next_leaderboard_entry;
    temp->next_leaderboard_entry = nullptr;

    playerCount--;
}

string Leaderboard::leaderboardtoResultstr() {
    if (head_leaderboard_entry == nullptr) {
        return "Leaderboard is empty.\n";
    }
    string str;
    str+= "Leaderboard\n-----------\n";

    LeaderboardEntry* temp = head_leaderboard_entry;
    for(int i=0; i<playerCount; i++){
        str+= std::to_string(i+1) + ". " + temp->player_name + " " + std::to_string(temp->score) + " " + formatTimestamp(temp->last_played) + '\n';
        temp = temp->next_leaderboard_entry;

    }
    return str;

}


void Leaderboard::print_leaderboard() {
    if (head_leaderboard_entry == nullptr) {
        std::cout << "Leaderboard is empty.\n";
        return;
    }

    std::cout << "Leaderboard\n-----------\n";

    LeaderboardEntry* temp = head_leaderboard_entry;
    int counter=1;
    while(temp){
        std::cout << counter << ". " << temp->player_name << " " << temp->score << " " << formatTimestamp(temp->last_played) << '\n';
        temp = temp->next_leaderboard_entry;
        counter++;

    }

}

void Leaderboard::insert_new_entry(LeaderboardEntry* new_entry) {
    if (head_leaderboard_entry == nullptr) {
        head_leaderboard_entry = new_entry;
        playerCount++;
        return;
    }

    else if (new_entry->score > head_leaderboard_entry->score) {
        new_entry->next_leaderboard_entry = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
        playerCount++;
        while (playerCount > MAX_LEADERBOARD_SIZE) {
            remove_LastEntry();
        }
        return;
    }
    else{
        LeaderboardEntry* temp = head_leaderboard_entry;
        while (temp->next_leaderboard_entry && new_entry->score <= temp->next_leaderboard_entry->score) {
            temp = temp->next_leaderboard_entry;
        }

        new_entry->next_leaderboard_entry = temp->next_leaderboard_entry;
        temp->next_leaderboard_entry = new_entry;

        playerCount++;
        while (playerCount > MAX_LEADERBOARD_SIZE) {
            remove_LastEntry();
        }
    }

}



Leaderboard::~Leaderboard() {
    LeaderboardEntry* temp = head_leaderboard_entry;

    while (temp != nullptr) {
        LeaderboardEntry* deletor = temp;
        temp = temp->next_leaderboard_entry;
        delete deletor;
    }
    head_leaderboard_entry = nullptr;
}
