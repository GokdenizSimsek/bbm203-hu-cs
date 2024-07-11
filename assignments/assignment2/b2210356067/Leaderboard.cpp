#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Leaderboard.h"

void Leaderboard::insert_new_entry(LeaderboardEntry *new_entry) {
    if (!head_leaderboard_entry || new_entry->score > head_leaderboard_entry->score) {
        new_entry->next_leaderboard_entry = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    } else {
        LeaderboardEntry *current = head_leaderboard_entry;

        while (current->next_leaderboard_entry && new_entry->score <= current->next_leaderboard_entry->score) {
            current = current->next_leaderboard_entry;
        }

        new_entry->next_leaderboard_entry = current->next_leaderboard_entry;
        current->next_leaderboard_entry = new_entry;
    }

    // Keep only the top 10 entries
    LeaderboardEntry *temp = head_leaderboard_entry;
    int count = 1;
    while (count < 10 && temp && temp->next_leaderboard_entry) {
        temp = temp->next_leaderboard_entry;
        count++;
    }

    if (temp && !temp->next_leaderboard_entry) {
        temp->next_leaderboard_entry = nullptr;
        delete temp->next_leaderboard_entry;

    }
}
std::string format_timestamp(time_t timestamp) {
    std::tm *tm_info = std::localtime(&timestamp);

    std::stringstream ss;
    ss << std::put_time(tm_info, "%H:%M:%S/%d.%m.%Y");

    return ss.str();
}

void Leaderboard::write_to_file(const string &filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    LeaderboardEntry *current = head_leaderboard_entry;
    while (current) {
        file << current->score << " " << current->last_played << " " << current->player_name << "\n";
        current = current->next_leaderboard_entry;
    }

    file.close();
}

void Leaderboard::read_from_file(const string &filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    unsigned long score;
    time_t lastPlayed;
    string playerName;
    while (file>>score) {
        file.ignore();
        file >> lastPlayed >> playerName;
        LeaderboardEntry *new_entry = new LeaderboardEntry(score, lastPlayed, playerName);
        insert_new_entry(new_entry);
    }

    file.close();
}

void Leaderboard::print_leaderboard() {
    std::cout << "Leaderboard\n-----------\n";

    LeaderboardEntry *current = head_leaderboard_entry;
    int order = 1;

    while (current) {
        std::cout << order << ". " << current->player_name << " " << current->score << " ";
        std::cout << format_timestamp(current->last_played) << "\n";
        current = current->next_leaderboard_entry;
        order++;
    }
}

Leaderboard::~Leaderboard() {
    LeaderboardEntry *current = head_leaderboard_entry;

    while (current) {
        LeaderboardEntry *temp = current;
        current = current->next_leaderboard_entry;
        delete temp;
    }
}
