#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include "linkedlist.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iomanip>

using namespace std;

string trim(const string &s)
{
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    if (start == string::npos || end == string::npos)
        return "";
    return s.substr(start, end - start + 1);
}   

class history;

// === Task 1 : Tournament schedule
class tournament
{
    private:
        circular_queue<string> queue;
        string csv_header;
        string file_header;
        int id_counter;

    public:
        tournament():csv_header("ID, DATE, TIME, COURT, PLAYER, SCORE, WINNER, LOSER"), id_counter(1){}

        // == Insert data from CSV file
        void load_from_file(const string &tennis)
        {
            ifstream load_tennis_data(tennis);

            if(!load_tennis_data.is_open())
            {
                cerr << "Failed to open file to load data." <<endl;
                return;
            }

            // == Read the header to verify if the file header and csv header is matching.
            if (getline(load_tennis_data,file_header))
            {
                csv_header = file_header;
            }

            string line;
            while (getline(load_tennis_data,line))
            {
                if (line.empty()) 
                    continue;

                stringstream ss(line);
                string token;
                Match<string> newMatch;

                getline(ss, token, ','); newMatch.id = token;
                getline(ss, token, ','); newMatch.date = token;
                getline(ss, token, ','); newMatch.time = token;
                getline(ss, token, ','); newMatch.court = token;
                getline(ss, token, ','); newMatch.player = token;
                getline(ss, token, ','); newMatch.score = token;
                getline(ss, token, ','); newMatch.winner = token;
                getline(ss, token, ','); newMatch.loser = token;
                newMatch.next = nullptr;
                queue.enqueue(newMatch);
            }
            load_tennis_data.close();
        }

        // == Saved tournament data to csv file
        void save_to_csv(const string &tennis)
        {
            ofstream save_tennis_data(tennis);

            if (!save_tennis_data.is_open()) 
            {
                cerr << "Failed to open " << tennis << " file for writing." << endl;
                return;
            }

            // == Firstly, save header
            save_tennis_data << csv_header << endl;

            // == Check if the queue lists was empty, close csv file.
            if(queue.isEmpty()){
                save_tennis_data.close();
                return;
            }

            Match<string>* current = queue.getFront();

            do 
            {
                save_tennis_data << current->id << "," 
                                << current->date << "," 
                                << current->time << "," 
                                << current->court << "," 
                                << current->player << "," 
                                << current->score << "," 
                                << current->winner << "," 
                                << current->loser << endl;
                current = current->next;
            } while(current != queue.getFront());

            save_tennis_data.close();
        }
    
        // == Display tournament schedule.
        void display_schedule() const 
        {
            //Set the header only false when first operate.
            static bool header_printed = false;
            if (!header_printed)
            {
                cout << left
                    << setw(6) << "ID" 
                    << setw(12) << "DATE" 
                    << setw(10)  << "TIME" 
                    << setw(10) << "COURT" 
                    << setw(30) << "PLAYER" 
                    << setw(15) << "SCORE" 
                    << setw(10) << "WINNER" 
                    << setw(10) << "LOSER" << endl;
                cout << "\n" << string(80,'=') << endl;
                header_printed = true;
            }
            queue.display();
        }

        // == Function for add new tournament record to list.
        void add_match(const string &date, const string &time, const string &court,
            const string &player, const string &score, const string &winner,
            const string &loser, const string &stage ="Regular") 
            {
            Match<string> newMatch;
            newMatch.id = match_id();
            newMatch.date = date;
            newMatch.time = time;
            newMatch.court = court;
            newMatch.player = player;
            newMatch.score = score.empty() ? "UPCOMING" : score;
            newMatch.winner = winner.empty() ? "UPCOMING" : winner;
            newMatch.loser = loser.empty() ? "UPCOMING" :loser;
            newMatch.stage = stage;
            newMatch.next = nullptr;

            queue.enqueue(newMatch);
            }

        // == Functions for search the specific match ID.
        Match<string>* find_match(const string &id) 
        {
            return queue.search(id);
        }

        // == Functions for remove a match records.
        bool delete_match(const string &id) 
        {
            return queue.remove(id);
        }

        // == Helper Function for Random Data 
        // == Functions for auto generate unique tournament ID, format: T000.
        string match_id()
        {
            char buf[10];
            sprintf(buf, "T%03d", id_counter);
            id_counter++;
            return string(buf);
        }

        // == Functions for generate a ramdom date, format: 31 Mar 2025
        static inline string match_date(const string &month) 
        {
            int day = rand() % 28 + 1; 
            // == Fixed the tournament year.
            int year = 2025;  

            stringstream ss;
            ss << setw(2) << setfill('0') << day << " " << month << " " << year ;
            return ss.str();
        }

        // == Functions for generate time ramdomly, format : 2pm, 3am, 11pm,etc, only 1-12 number.
        static inline string match_time() 
        {
            // == Generate the ramdom numberic among 1 to 12, 1 <= NUM <= 12.
            int hour = (rand() % 12) + 1;           

            // == Generate am/pm ramdomly.
            string ampm = (rand() % 2 == 0) ? "am" : "pm";  
            
            stringstream ss;
            ss << hour << ampm;
            return ss.str();
        }

        // == Function for generate court number ramdomly like Court 1.
        static inline string match_court() 
        {
            // == Generate 1 to 6 number.
            int courtNumber = rand() % 6 + 1; 
            return "Court " + to_string(courtNumber);
        }

        // == Function for generate matchup string, format: Player1 VS Player2.
        static inline string match_players() 
        {
            const string names[] = {"RACHEL", "ALICE", "BOB", "CHARLIE", "DAVID", "EMILY"};
            const int numNames = 6;
            string player1 = names[rand() % numNames];
            string player2 = names[rand() % numNames];
            while(player2 == player1) 
            {
                player2 = names[rand() % numNames];
            }
            return player1 + " VS " + player2;
        }

        // == Function for generate score for 3 round
        // == Count which player won more round to determine the winner and loser automatically.
        static inline void match_score(const string &players, string &score, string &winner, string &loser) 
        {
            // == Refer to the player column
            size_t pos = players.find(" VS ");
            string player1 = players.substr(0, pos);
            string player2 = players.substr(pos + 4);

            int winCount1 = 0;
            int winCount2 = 0;
            string roundScores[3];

            for (int i = 0; i < 3; i++) 
            {
                int roundWinner = rand() % 2;
                int fixedScore = (rand() % 2 == 0) ? 6 : 7;
                int opponentScore = rand() % fixedScore;
                stringstream rs;
                if (roundWinner == 0) 
                {
                    rs << fixedScore << "-" << opponentScore;
                    winCount1++;
                } 
                else 
                {
                  rs << opponentScore << "-" << fixedScore;
                    winCount2++;
                }
                roundScores[i] = rs.str();
            }

            stringstream ss;

            ss << roundScores[0] << "|" << roundScores[1] << "|" << roundScores[2];
            score = ss.str();

            if (winCount1 > winCount2) 
            {
                winner = player1;
                loser = player2;
            } 
            else 
            {
                winner = player2;
                loser = player1;
            }
        }

        // == Generate a match record (schedule record) based on the given match ID and return a pointer  
        // == Except for the date, court, and players, the score and winner are initially empty due to the match status.
        static inline Match<string>* match_record(int matchNumber) 
        {
            Match<string>* m = new Match<string>;

            stringstream ss;
            ss << "T" << setw(3) << setfill('0') << matchNumber;

            m->id = ss.str();
            m->date = match_date("");
            m->court = match_court();
            m->player = match_players();
            m->score = "";
            m->winner = "";
            m->loser = "";
            m->next = nullptr;

            return m;
        }

        // In the tournament class, add the following member function to update substitutions
        void updateSubstitutions() 
        {
            // If the match schedule is empty, no update is needed
            if (queue.isEmpty()) 
            {
                cout << "No match records available for substitution update." << endl;
                return;
            }
            
            // Open the player_and_Substitute.csv file
            ifstream infile("player_and_Substitute.csv");
            if (!infile.is_open()) 
            {
                cerr << "Error: Cannot open player_and_Substitute.csv" << endl;
                return;
            }
            
            string line;
            // Read and skip the header line, assuming the header is:
            // "number,name,age,substitute,subAge,hasWithdrawn,withdrawalReason,needsSubstitute"
            getline(infile, line);
            
            // Read CSV data line by line
            while (getline(infile, line)) 
            {
                if (line.empty())
                    continue;
                stringstream ss(line);
                string number, playerName, age, substitute, subAge, hasWithdrawn, withdrawalReason, needsSubstitute;
                
                // Read each field
                getline(ss, number, ',');         // Player number (ignored)
                getline(ss, playerName, ',');       // Original player's name
                getline(ss, age, ',');              // Age (ignored)
                getline(ss, substitute, ',');       // Substitute player's name
                getline(ss, subAge, ',');           // Substitute age (ignored)
                getline(ss, hasWithdrawn, ',');     // Withdrawal status (ignored)
                getline(ss, withdrawalReason, ','); // Withdrawal reason (ignored)
                getline(ss, needsSubstitute, ',');  // Needs substitution
                
                // If needsSubstitute field is "yes" (case insensitive), perform substitution
                if (needsSubstitute == "yes" || needsSubstitute == "YES") 
                {
                    // Traverse all match records in the schedule
                    Match<string>* current = queue.getFront();
                    if (current == nullptr)
                        break;
                    do 
                    {
                        // The player field is in the format "PLAYER1 VS PLAYER2"
                        size_t pos = current->player.find(playerName);
                        if (pos != string::npos) 
                        {
                            // Replace the original player's name with the substitute's name
                            current->player.replace(pos, playerName.length(), substitute);
                            // If the winner or loser field exactly matches the original name, replace it too
                            if (current->winner == playerName)
                                current->winner = substitute;
                            if (current->loser == playerName)
                                current->loser = substitute;
                        }
                        current = current->next;
                    } while (current != queue.getFront());
                }
            }
            infile.close();
            cout << "Substitution update completed." << endl;
        }

        // Display the final results of the championship based on the actual data of the final stage matches
        void display_final_results() 
        {
            if (queue.isEmpty()) 
            {
                cout << "No match records available." << endl;
                return;
            }
            // Assume that the final stage matches have a stage value of "Final" and "ThirdPlace"
            Match<string>* finalMatch = nullptr;   // Final: Championship match
            Match<string>* thirdMatch = nullptr;     // Third place match

            Match<string>* current = queue.getFront();
            do 
            {
                // Check the stage field for "Final" or "ThirdPlace"
                if (current->stage == "Final") 
                {
                    finalMatch = current;
                }
                else if (current->stage == "ThirdPlace")
                {
                    thirdMatch = current;
                } 
                current = current->next;
            } while (current != queue.getFront());
                
            if (finalMatch && thirdMatch) 
            {
                cout << "\nRESULT:" << endl;
                cout << "CHAMPION: " << finalMatch->winner << endl;
                cout << "SECOND  : " << finalMatch->loser << endl;
                cout << "THIRD   : " << thirdMatch->winner << endl;
                cout << "FOURTH  : " << thirdMatch->loser << endl;
            } 
            else 
            {
                cout << "Final stage matches are not fully recorded." << endl;
            }
        }


        // New function: Simulate the entire tournament and output the results for each stage (Preliminary, Group, Knockout, and Finals)
        // New function: Simulate the entire tournament process and output results for each stage
        void simulateTournament()
        {
            cout << "\n" << string(62, '=') << endl;
            cout << "\t\tPreliminary Round" << endl;
            cout << string(62, '=') << endl;
            
            // --- Read player names from "player_and_Substitute.csv" ---
            ifstream infile("player_and_Substitute.csv");
            if (!infile.is_open()){
                cerr << "Error: Cannot open player_and_Substitute.csv" << endl;
                return;
            }
            string line;
            // Read and discard the header (assumed to be "number,name,age,substitute,subAge,hasWithdrawn,withdrawalReason,needsSubstitute")
            getline(infile, line);
            
            // First pass: count the number of players
            int totalPlayers = 0;
            while(getline(infile, line)){
                if(!line.empty())
                    totalPlayers++;
            }
            if(totalPlayers < 32){
                cerr << "Not enough players in player_and_Substitute.csv. At least 32 players are needed." << endl;
                infile.close();
                return;
            }
            // Reset file pointer to beginning
            infile.clear();
            infile.seekg(0, ios::beg);
            getline(infile, line); // skip header again
            
            // Dynamically allocate an array to hold player names
            string* playerNames = new string[totalPlayers];
            int index = 0;
            while(getline(infile, line)){
                if(line.empty())
                    continue;
                stringstream ss(line);
                string token;
                // Read first field (number) and ignore it
                getline(ss, token, ',');
                // Read second field (player name)
                getline(ss, token, ',');
                playerNames[index++] = token;
            }
            infile.close();
            
            // --- Shuffle the player names using the Fisher-Yates algorithm ---
            for (int i = totalPlayers - 1; i > 0; i--)
            {
                int j = rand() % (i + 1);
                string temp = playerNames[i];
                playerNames[i] = playerNames[j];
                playerNames[j] = temp;
            }
            
            // --- Preliminary Round: 32 players form 16 matches ---
            string prelimWinners[16];
            for (int i = 0; i < 16; i++) 
            {
                string p1 = playerNames[2 * i];
                string p2 = playerNames[2 * i + 1];
                string matchPlayers = p1 + " VS " + p2;
                string score, win, lose;
                tournament::match_score(matchPlayers, score, win, lose);
                string date = tournament::match_date("JAN");
                string time = tournament::match_time();
                string court = tournament::match_court();
                add_match(date, time, court, matchPlayers, score, win, lose);
                
                cout << setw(4) << left << match_id() << "  "
                    << setw(11) << left << date << "  "
                    << setw(4) << left << time << "  "
                    << setw(8) << left << court << "  "
                    << setw(20) << left << matchPlayers << "  "
                    << setw(15) << left << score << "  "
                    << setw(8) << left << win << "  "
                    << setw(8) << left << lose << endl;
                
                prelimWinners[i] = win;
            }
            
            // Free the dynamic array memory
            delete[] playerNames;
            
            // --- Round Robin Stage ---
            cout << "\n" << string(62, '=') << endl;
            cout << "\t\tRound Robin Stage" << endl;
            cout << string(62, '=') << endl;
            
            string groups[4][4];
            for (int g = 0; g < 4; g++)
            {
                for (int j = 0; j < 4; j++)
                {
                    groups[g][j] = prelimWinners[g * 4 + j];
                }
            }

            int groupWins[4][4] = {0};
            
            for (int g = 0; g < 4; g++)
            {
                cout << "\nGroup " << (g + 1) << " Participants:" << endl;
                for (int j = 0; j < 4; j++)
                {
                    cout << groups[g][j] << (j < 3 ? ", " : "\n");
                }
                cout << "\nRound Robin Matches for Group " << (g + 1) << ":" << endl;
                // Each pair in the group plays a match
                for (int i = 0; i < 4; i++) 
                {
                    for (int j = i + 1; j < 4; j++) 
                    {
                        string matchPlayers = groups[g][i] + " VS " + groups[g][j];
                        string score, win, lose;
                        tournament::match_score(matchPlayers, score, win, lose);
                        string date = tournament::match_date("FEB");
                        string time = tournament::match_time();
                        string court = tournament::match_court();
                        add_match(date, time, court, matchPlayers, score, win, lose);
                        cout << setw(4) << left << match_id() << "  "
                            << setw(11) << left << date << "  "
                            << setw(4) << left << time << "  "
                            << setw(8) << left << court << "  "
                            << setw(20) << left << matchPlayers << "  "
                            << setw(15) << left << score << "  "
                            << setw(8) << left << win << "  "
                            << setw(8) << left << lose << endl;
                        // Update wins count
                        if (win == groups[g][i])
                            groupWins[g][i]++;
                        else if (win == groups[g][j])
                            groupWins[g][j]++;
                    }
                }
                cout << "\nGroup " << (g + 1) << " Standings:" << endl;
                for (int i = 0; i < 4; i++) 
                {
                    cout << groups[g][i] << ": " << groupWins[g][i] << " wins" << endl;
                }
            }

            string qualified[8];
            int qIndex = 0;
            for (int g = 0; g < 4; g++)
            {
                // Select first and second place from each group
                int firstIdx = 0, secondIdx = 1;
                if (groupWins[g][secondIdx] > groupWins[g][firstIdx])
                    swap(firstIdx, secondIdx);
                for (int i = 2; i < 4; i++) {
                    if (groupWins[g][i] > groupWins[g][firstIdx]) {
                        secondIdx = firstIdx;
                        firstIdx = i;
                    }
                    else if (groupWins[g][i] > groupWins[g][secondIdx]) {
                        secondIdx = i;
                    }
                }
                cout << "\nQualified from Group " << (g + 1) << ": " 
                    << groups[g][firstIdx] << " and " << groups[g][secondIdx] << endl;
                qualified[qIndex++] = groups[g][firstIdx];
                qualified[qIndex++] = groups[g][secondIdx];
            }
            
            // --- Knockout Stage: Quarterfinals ---
            cout << "\n" << string(62, '=') << endl;
            cout << "\t\tKnockout Stage (Quarterfinals)" << endl;
            cout << string(62, '=') << endl;
            
            string quarterFinalWinners[4];
            for (int i = 0; i < 4; i++) {
                string matchPlayers = qualified[2 * i] + " VS " + qualified[2 * i + 1];
                string score, win, lose;
                tournament::match_score(matchPlayers, score, win, lose);
                string date = tournament::match_date("MAR");
                string time = tournament::match_time();
                string court = tournament::match_court();
                add_match(date, time, court, matchPlayers, score, win, lose);
                cout << setw(4) << left << match_id() << "  "
                    << setw(11) << left << date << "  "
                    << setw(4) << left << time << "  "
                    << setw(8) << left << court << "  "
                    << setw(20) << left << matchPlayers << "  "
                    << setw(15) << left << score << "  "
                    << setw(8) << left << win << "  "
                    << setw(8) << left << lose << endl;
                quarterFinalWinners[i] = win;
            }
            
            // --- Semifinals Stage ---
            cout << "\n" << string(62, '=') << endl;
            cout << "\t\tSemifinals" << endl;
            cout << string(62, '=') << endl;
            
            string semifinalWinners[2];
            string semifinalLosers[2];
            for (int i = 0; i < 2; i++) {
                string matchPlayers = quarterFinalWinners[2 * i] + " VS " + quarterFinalWinners[2 * i + 1];
                string score, win, lose;
                tournament::match_score(matchPlayers, score, win, lose);
                string date = tournament::match_date("APR");
                string time = tournament::match_time();
                string court = tournament::match_court();
                add_match(date, time, court, matchPlayers, score, win, lose);
                cout << setw(4) << left << match_id() << "  "
                    << setw(11) << left << date << "  "
                    << setw(4) << left << time << "  "
                    << setw(8) << left << court << "  "
                    << setw(20) << left << matchPlayers << "  "
                    << setw(15) << left << score << "  "
                    << setw(8) << left << win << "  "
                    << setw(8) << left << lose << endl;
                semifinalWinners[i] = win;
                semifinalLosers[i] = (win == quarterFinalWinners[2 * i]) ? quarterFinalWinners[2 * i + 1] : quarterFinalWinners[2 * i];
            }
            
            // --- Final Stage (Upcoming) ---
            cout << "\n" << string(62, '=') << endl;
            cout << "\t\tFinal Stage (Upcoming)" << endl;
            cout << string(62, '=') << endl;
            
            // Final match (Championship)
            string finalMatchPlayers = semifinalWinners[0] + " VS " + semifinalWinners[1];
            string finalDate = tournament::match_date("APR");
            string finalTime = tournament::match_time();
            string finalCourt = tournament::match_court();
            
            // Third place match
            string thirdPlaceMatchPlayers = semifinalLosers[0] + " VS " + semifinalLosers[1];
            string thirdDate = tournament::match_date("APR");
            string thirdTime = tournament::match_time();
            string thirdCourt = tournament::match_court();
            
            string upcomingScore = "UPCOMING";
            string upcomingWinner = "UPCOMING";
            string upcomingLoser = "UPCOMING";
            
            // Add the final stage matches
            add_match(finalDate, finalTime, finalCourt, finalMatchPlayers, upcomingScore, upcomingWinner, upcomingLoser, "Final");
            add_match(thirdDate, thirdTime, thirdCourt, thirdPlaceMatchPlayers, upcomingScore, upcomingWinner, upcomingLoser, "ThirdPlace");
            
            // Display final stage match records
            cout << "\nFinal Stage Match Records:" << endl;
            string finalID = match_id();
            cout << finalID << "\t" 
                << finalDate << "\t" 
                << finalTime << "\t" 
                << finalCourt << "\t" 
                << setw(24) << left << finalMatchPlayers << "\t" 
                << setw(8) << left << upcomingScore << "\t" 
                << setw(8) << left << upcomingWinner << "\t" 
                << setw(8) << left << upcomingLoser << endl;
            string thirdID = match_id();
            cout << thirdID << "\t" 
                << thirdDate << "\t" 
                << thirdTime << "\t" 
                << thirdCourt << "\t" 
                << setw(24) << left << thirdPlaceMatchPlayers << "\t" 
                << setw(8) << left << upcomingScore << "\t" 
                << setw(8) << left << upcomingWinner << "\t" 
                << setw(8) << left << upcomingLoser << endl;
            
            cout << "\nUpdated Final Results:" << endl;
            display_final_results();
            save_to_csv("tennis.csv");
        }
        void update_history(history &hist) ;
};

// === Task 4 : Tournament history
class history
{
    private:
        stack<Match<string>> history_stack;

    public:
        // == Functions for store a match record to history.
        // == Only when the score is not "UPCOMING" will be store to history.
        // == Due to upcoming event means the match is scheduled and has not yet commenced.
        void store_match(const Match<string>& m)
        {
            if (m.score != "UPCOMING" && !m.score.empty() && !m.winner.empty() && !m.loser.empty())
            history_stack.push(m);
        }

        // == Alias for add_match.
        void add_match(const Match<string>& m) 
        {
            store_match(m);
        }

        // == Helper function for compare date stings.
        // == RULES: Store the match records refer to the match's date.
        // == First, convert letter to numberic, easy for compare date.
        // == I dun wan to chnage the match date cuz letter is pretty XIXI. 
        static int month_to_int(const string &month) 
        {
            if (month == "JAN") return 1;
            else if (month == "FEB") return 2;
            else if (month == "MAR") return 3;
            else if (month == "APR") return 4;
            else if (month == "MAY") return 5;
            else if (month == "JUN") return 6;
            else if (month == "JUL") return 7;
            else if (month == "AUG") return 8;
            else if (month == "SEP") return 9;
            else if (month == "OCT") return 10;
            else if (month == "NOV") return 11;
            else if (month == "DEC") return 12;
            return 0; // In case the month is unrecognized
        }

        // == Second, compare date so that the latest match will be on the top of the stack.
        static int compare_dates(const string &d1, const string &d2) 
        {
            int day1, day2, year1, year2;
            string month1, month2;
            {
                stringstream ss(d1);
                ss >> day1 >> month1 >> year1;
            }
            {
                stringstream ss(d2);
                ss >> day2 >> month2 >> year2;
            }
            int m1 = month_to_int(month1);
            int m2 = month_to_int(month2);

            // == Compare years. But i have set the YEAR=2025... thats ok.
            if (year1 != year2) return year1 - year2;

            // == Compare months
            if (m1 != m2) return m1 - m2;

            // == Compare days
            return day1 - day2;
        }

        // == Final, display match history, sorted so that the latest matches are shown first.
        void display_history() 
        {
            if (history_stack.empty()) 
            {
                cout << "No match history available." << endl;
                return;
            }
            
            // Use two auxiliary stacks:
            // - 'aux' temporarily holds data popped from hisStack,
            // - 'sorted' will hold the records in sorted order (with the latest match on top).
            stack<Match<string>> aux;
            stack<Match<string>> sorted;
            
            // Step 1: Transfer all records from stack to aux.
            while (!history_stack.empty()) 
            {
                aux.push(history_stack.pop());
            }
            
            // Step 2: Insert each record from aux into 'sorted' using an insertion sort approach.
            // The sorting rule: If the top of 'sorted' has a date later than the record to insert,
            // pop from 'sorted' back into aux, then push the current record into sorted.
            while (!aux.empty()) 
            {
                Match<string> m = aux.pop();
                // compare_dates returns a positive value if the first date is later.
                while (!sorted.empty() && compare_dates(sorted.top().date, m.date) > 0) {
                    aux.push(sorted.pop());
                }
                sorted.push(m);
            }
            
            // Output the header.
            cout << "\nMatch History:" << endl;
            cout << left 
              << setw(6)  << "ID" 
              << setw(20) << "DATE" 
              << setw(30) << "PLAYER" 
              << setw(15) << "SCORE" 
              << setw(10) << "WINNER" 
              << setw(10) << "LOSER" 
              << endl;
            cout << string(80, '=') << endl;

            // Use a third auxiliary stack to restore the records after output.
            stack<Match<string>> restore;
            while (!sorted.empty()) 
            {
                Match<string> m = sorted.pop();
                cout << left 
                    << setw(6)  << m.id
                    << setw(20) << m.date
                    << setw(30) << m.player
                    << setw(15) << m.score
                    << setw(10) << m.winner
                    << setw(10) << m.loser
                    << endl;
                restore.push(m);
            }
            
            // Restore the history back into stack (the order may change, but all records are preserved).
            while (!restore.empty()) 
            {
                history_stack.push(restore.pop());
            }
        }

        // == Functions for delete a match record fromhistory by ID
        bool delete_match(const string &id) 
        {
            stack<Match<string>> temp;
            bool found = false;
            while (!history_stack.empty()) 
            {
                Match<string> m = history_stack.pop();
                if (!found && m.id == id) 
                {
                    found = true; // Skip this record.
                    continue;
                }
                temp.push(m);
            }
            // Restore records back to hisStack.
            while (!temp.empty()) 
            {
                history_stack.push(temp.pop());
            }
            return found;
        }
};

inline void tournament::update_history(history &hist) {
    if (queue.isEmpty())
        return;
    Match<string>* current = queue.getFront();
    do {
        if (current->score != "UPCOMING" && !current->score.empty() &&
            !current->winner.empty() && !current->loser.empty())
        {
            hist.add_match(*current);
        }
        current = current->next;
    } while (current != queue.getFront());
}

#endif