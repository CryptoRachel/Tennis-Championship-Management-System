#include "tournament.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Create an empty CSV file and write the header if it doesn't exist.
void csv_file(const string &tennis, const string &header)
{
    ifstream empty_csv_file(tennis);
    if (!empty_csv_file.good())
    {
        ofstream outfile(tennis);
        if (!outfile.is_open())
        {
            cerr << "Failed to create file " << tennis << endl;
            return;
        }
        outfile << header << endl;
        outfile.close();
        cout << "CSV file " << tennis << " created successfully and header written." << endl;
    }
}

// Helper function: Delete a match record from match history.
// This calls the delete_match() function defined in the history class.
bool delete_match_history(history &hist, const string &id)
{
    return hist.delete_match(id);
}

// Display the interactive menu and handle user input.
void display_menu(tournament &tour, history &hist, const string &tennis)
{
    int mainChoice, category;
    string input;
    while (true)
    {
        cout << string(62, '=') << endl;
        cout << "ASIA PACIFIC UNIVERSITY TENNIS CHAMPIONSHIP MANAGEMENT SYSTEM" << endl;
        cout << string(62, '=') << endl;
        cout << "1. Review" << endl;
        cout << "2. Modify" << endl;
        cout << "3. Add" << endl;
        cout << "4. Delete" << endl;
        cout << "5. Exit" << endl;
        cout << string(62, '=') << endl;
        cout << "Enter your choice: ";
        cin >> mainChoice;
        cin.ignore(); // Clear newline

        switch (mainChoice)
        {
            // == 1.Review Mode
            case 1: 
            {
                cout << "\n" << string(62, '=') << endl;
                cout << "\t\tYOU ARE IN THE REVIEW MODE" << endl;
                cout << string(62, '=') << endl;
                cout << "1. Review Match Schedule" << endl;
                cout << "2. Review Match History" << endl;
                cout << "3. Exit" << endl;
                cout << string(62, '=') << endl;
                cout << "Enter your choice: ";
                cin >> category;
                cin.ignore();
                if (category == 1)
                {
                    tour.updateSubstitutions();
                    tour.display_schedule();
                }
                else if (category == 2)
                    hist.display_history();
                else
                    cout << "Invalid category option!\n";
                break;
            }

            // == 2. Modify Mode
            case 2: 
            {
                cout << "\n" << string(62, '=') << endl;
                cout << "YOU ARE IN THE MODIFY MODE" << endl;
                cout << string(62, '=') << endl;
                cout << "1. Modify Match Schedule" << endl;
                cout << "2. Modify Match History" << endl;
                cout << string(62, '=') << endl;
                cout << "Enter your choice: ";
                cin >> category;
                cin.ignore();
                if (category == 1)
                {
                    cout << "Please enter the match ID to modify: ";
                    getline(cin, input);
                    // 查找匹配记录
                    Match<string>* matchPtr = tour.find_match(input);
                    if (matchPtr == nullptr)
                    {
                        cout << "Match ID " << input << " not found." << endl;
                    }
                    else
                    {
                        cout << "Current match details:\n";
                        cout << "ID: " << matchPtr->id << "\nDate: " << matchPtr->date
                            << "\nTime: " << matchPtr->time << "\nCourt: " << matchPtr->court
                            << "\nPlayers: " << matchPtr->player << "\nScore: " << matchPtr->score
                            << "\nWinner: " << matchPtr->winner << "\nLoser: " << matchPtr->loser << "\n";
                        cout << "Please enter new details (press Enter to skip if no changes):\n";
                        
                        cout << "New Date: ";
                        string newDate;
                        getline(cin, newDate);
                        if (!newDate.empty())
                            matchPtr->date = newDate;
                        
                        cout << "New Time: ";
                        string newTime;
                        getline(cin, newTime);
                        if (!newTime.empty())
                            matchPtr->time = newTime;
                        
                        cout << "New Court: ";
                        string newCourt;
                        getline(cin, newCourt);
                        if (!newCourt.empty())
                            matchPtr->court = newCourt;
                        
                        cout << "New Players (format: PLAYER1 VS PLAYER2): ";
                        string newPlayer;
                        getline(cin, newPlayer);
                        if (!newPlayer.empty())
                            matchPtr->player = newPlayer;
                        
                        cout << "New Score (format: score1|score2|score3): ";
                        string newScore;
                        getline(cin, newScore);
                        if (!newScore.empty())
                        {
                            matchPtr->score = newScore;
                            // 如果更新了分数且不为 "UPCOMING"，则更新胜者和败者信息。
                            if(newScore != "UPCOMING")
                                tournament::match_score(matchPtr->player, matchPtr->score, matchPtr->winner, matchPtr->loser);
                        }
                        // 保存更新后的数据到 CSV 文件
                        tour.save_to_csv(tennis);
                        tour.update_history(hist);
                        cout << "Tournament schedule updated.\n";
                    }
                }    
            
                else if (category == 2)
                {
                    cout << "No permission to modify match history! Please modify the tournament schedule instead.\n";
                }
                else
                {
                    cout << "Invalid category option!\n";
                }
                break;
            }

            // == 3.Add Mode
            case 3: 
            {
                cout << "\n" << string(62, '=') << endl;
                cout << "YOU ARE IN THE ADDING MODE" << endl;
                cout << string(62, '=') << endl;
                cout << "1. Add Match Schedule" << endl;
                cout << "2. Add Match History" << endl;
                cout << string(62, '=') << endl;
                cout << "Enter your choice: ";
                cin >> category;
                cin.ignore();
                if (category == 1)
                {
                    string date, time, player1, player2, score;
                    cout << "Please enter Date: ";
                    getline(cin, date);
                    cout << "Please enter Time: ";
                    getline(cin, time);
                    cout << "Enter player 1: ";
                    getline(cin, player1);
                    cout << "Enter player 2: ";
                    getline(cin, player2);
                    cout << "Enter match score (format: score1|score2|score3): ";
                    getline(cin, score);
                    string court = tournament::match_court();
                    string winner = "", loser = "";
                    if (!score.empty() && score != "UPCOMING")
                    {
                        tournament::match_score(player1 + " VS " + player2, score, winner, loser);
                    }
                    tour.add_match(date, time, court, player1 + " VS " + player2, score, winner, loser);
                    tour.save_to_csv(tennis);
                    cout << "Match schedule added.\n";
                }
                else if (category == 2)
                {
                    string date, player1, player2, score;
                    cout << "Please enter Date: ";
                    getline(cin, date);
                    cout << "Enter player 1: ";
                    getline(cin, player1);
                    cout << "Enter player 2: ";
                    getline(cin, player2);
                    cout << "Enter match score (format: score1|score2|score3): ";
                    getline(cin, score);
                    char idBuffer[10];
                    static int historyCounter = 1;
                    sprintf(idBuffer, "H%03d", historyCounter++);
                    Match<string> newMatch;
                    newMatch.id = idBuffer;
                    newMatch.date = date;
                    newMatch.player = player1 + " VS " + player2;
                    newMatch.score = score;
                    string winner = "", loser = "";
                    if (!score.empty() && score != "UPCOMING")
                    {
                        tournament::match_score(newMatch.player, newMatch.score, winner, loser);
                        newMatch.winner = winner;
                        newMatch.loser = loser;
                    }
                    newMatch.next = nullptr;
                    hist.store_match(newMatch);
                    cout << "Match history added with ID " << newMatch.id << endl;
                }
                else
                {
                    cout << "Invalid category option!\n";
                }
                break;
            }

            // == 4.Delete Mode
            case 4: 
            {
                cout << "\n" << string(62, '=') << endl;
                cout << "YOU ARE IN THE DELETE MODE" << endl;
                cout << string(62, '=') << endl;
                cout << "1. Delete Match Schedule" << endl;
                cout << "2. Delete Match History" << endl;
                cout << string(62, '=') << endl;
                cout << "Enter your choice: ";
                cin >> category;
                cin.ignore();
                if (category == 1)
                {
                    cout << "Please enter the match ID to delete: ";
                    getline(cin, input);
                    Match<string>* matchPtr = tour.find_match(input);
                    if (matchPtr == nullptr)
                    {
                        cout << "Match ID " << input << " not found." << endl;
                    }
                    else
                    {
                        cout << "Match found:\n";
                        cout << "ID: " << matchPtr->id << "  Date: " << matchPtr->date
                            << "  Time: " << matchPtr->time << "  Court: " << matchPtr->court
                            << "  Players: " << matchPtr->player << endl;
                        cout << "Confirm deletion? (Y/N): ";
                        char confirm;
                        cin >> confirm;
                        cin.ignore();
                        if (confirm == 'Y' || confirm == 'y')
                        {
                            if (tour.delete_match(input))
                            {
                                cout << "Tournament schedule deleted.\n";
                                tour.save_to_csv(tennis);
                            }
                            else
                            {
                                cout << "Deletion failed.\n";
                            }
                        }
                        else
                        {
                            cout << "Deletion cancelled.\n";
                        }
                    }
                }
                else if (category == 2)
                {
                    cout << "Please enter the match ID to delete from match history: ";
                    getline(cin, input);
                    if (delete_match_history(hist, input))
                        cout << "Match history deleted.\n";
                    else
                        cout << "Match ID " << input << " not found in match history.\n";
                }
                else
                {
                    cout << "Invalid category option!\n";
                }
                break;
            }

            case 5: // Exit
            {
                cout << "Exiting system!\n";
                return;
            }

            default:
            cout << "Invalid option, please try again.\n";
            break;
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// TASK 2: TICKET SALES AND SPECTATOR MANAGEMENT
// Define the Ticket structure
struct Ticket {
    int ticketID;
    string name;
    string category;
    string purchaseTime;
    bool entryStatus;
    string seatNumber;
    double price;
    string paymentStatus;
    bool exitStatus;
};

// Return the priority based on ticket category: VIP highest, then Early-Bird, then Public
int getCategoryPriority(const Ticket &t) {
    if (t.category == "VIP")
        return 3;
    else if (t.category == "Early-Bird")
        return 2;
    else if (t.category == "Public")
        return 1;
    return 0;
}

// Custom container based on an array (without using built-in containers)
class TicketArray {
private:
    Ticket* data;
    int size;
    int capacity;
    void resize(int newCapacity) {
        Ticket* newData = new Ticket[newCapacity];
        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }
public:
    TicketArray(int cap = 10) : size(0), capacity(cap) {
        data = new Ticket[capacity];
    }
    ~TicketArray() {
        delete[] data;
    }
    void push_back(const Ticket& t) {
        if (size == capacity) {
            resize(capacity * 2);
        }
        data[size++] = t;
    }
    void removeAt(int index) {
        if (index < 0 || index >= size) {
            cerr << "Index out of bounds in removeAt" << endl;
            exit(1);
        }
        for (int i = index; i < size - 1; i++) {
            data[i] = data[i + 1];
        }
        size--;
    }
    Ticket& operator[](int index) {
        if (index < 0 || index >= size) {
            cerr << "Index out of bounds" << endl;
            exit(1);
        }
        return data[index];
    }
    const Ticket& operator[](int index) const {
        if (index < 0 || index >= size) {
            cerr << "Index out of bounds" << endl;
            exit(1);
        }
        return data[index];
    }
    int getSize() const {
        return size;
    }
};

// Convert a string to a boolean value
bool stringToBool(const string& str) {
    return (str == "True" || str == "true");
}

// Helper function: Check if a given seat number already exists
bool isSeatNumberExists(const TicketArray* tickets, const string &seat) {
    for (int i = 0; i < tickets->getSize(); i++) {
        if ((*tickets)[i].seatNumber == seat) {
            return true;
        }
    }
    return false;
}

// Read a CSV file and store the data into a TicketArray
TicketArray* readTickets(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file " << filename << endl;
        return nullptr;
    }
    string line;
    getline(file, line); // Read the header line
    TicketArray* tickets = new TicketArray();
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string token;
        Ticket t;
        getline(ss, token, ',');
        t.ticketID = stoi(token);
        getline(ss, token, ',');
        t.name = token;
        getline(ss, token, ',');
        t.category = token;
        getline(ss, token, ',');
        t.purchaseTime = token;
        getline(ss, token, ',');
        t.entryStatus = stringToBool(token);
        getline(ss, token, ',');
        t.seatNumber = token;
        getline(ss, token, ',');
        t.price = stod(token);
        getline(ss, token, ',');
        t.paymentStatus = token;
        getline(ss, token, ',');
        t.exitStatus = stringToBool(token);
        tickets->push_back(t);
    }
    file.close();
    return tickets;
}

// Write the data from a TicketArray back to a CSV file
void updateCSVFile(const TicketArray* tickets, const string &filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file " << filename << " for writing." << endl;
        return;
    }
    file << "TicketID,Name,Category,PurchaseTime,EntryStatus,SeatNumber,Price,PaymentStatus,ExitStatus" << endl;
    for (int i = 0; i < tickets->getSize(); i++) {
        const Ticket &t = (*tickets)[i];
        file << t.ticketID << ","
             << t.name << ","
             << t.category << ","
             << t.purchaseTime << ","
             << (t.entryStatus ? "True" : "False") << ","
             << t.seatNumber << ","
             << fixed << setprecision(2) << t.price << ","
             << t.paymentStatus << ","
             << (t.exitStatus ? "True" : "False") << endl;
    }
    file.close();
    cout << "\nCSV file updated successfully." << endl;
}

// Nicely display all ticket records
void displayTickets(const TicketArray* tickets) {
    if (!tickets) return;
    cout << setfill(' ');

    cout << "\nTotal tickets read: " << tickets->getSize() << "\n" << endl;
    cout << left << setw(10) << "TicketID" 
         << left << setw(20) << "Name" 
         << left << setw(14) << "Category" 
         << left << setw(22) << "PurchaseTime" 
         << left << setw(18) << "EntryStatus" 
         << left << setw(10) << "Seat" 
         << left << setw(10) << "Price" 
         << left << setw(16) << "PaymentStatus" 
         << left << setw(18) << "ExitStatus" 
         << endl;
    cout << string(128, '-') << endl;
    for (int i = 0; i < tickets->getSize(); i++) {
        const Ticket &t = (*tickets)[i];
        string entryStr = t.entryStatus ? "Already entered" : "Not yet";
        string exitStr  = t.exitStatus  ? "Leaving" : "Not leaving yet";
        cout << left << setw(10) << t.ticketID
             << left << setw(20) << t.name
             << left << setw(14) << t.category
             << left << setw(22) << t.purchaseTime
             << left << setw(18) << entryStr
             << left << setw(10) << t.seatNumber
             << left << setw(10) << fixed << setprecision(2) << t.price
             << left << setw(16) << t.paymentStatus
             << left << setw(18) << exitStr
             << endl;
    }
}

// The following Priority Queue implementation remains unchanged
class TicketPriorityQueue {
private:
    Ticket* data;
    int capacity;
    int size;
    void resize(int newCapacity) {
        Ticket* newData = new Ticket[newCapacity];
        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }
    bool compare(const Ticket &a, const Ticket &b) {
        int pa = getCategoryPriority(a);
        int pb = getCategoryPriority(b);
        if (pa != pb)
            return pa > pb;
        return a.purchaseTime < b.purchaseTime;
    }
public:
    TicketPriorityQueue(int cap = 10): capacity(cap), size(0) {
        data = new Ticket[capacity];
    }
    ~TicketPriorityQueue() {
        delete[] data;
    }
    void push(const Ticket &t) {
        if (size == capacity) {
            resize(capacity * 2);
        }
        data[size] = t;
        int i = size;
        size++;
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (compare(data[i], data[parent])) {
                Ticket temp = data[i];
                data[i] = data[parent];
                data[parent] = temp;
                i = parent;
            } else {
                break;
            }
        }
    }
    Ticket top() {
        if (size == 0) {
            cerr << "Priority queue is empty" << endl;
            exit(1);
        }
        return data[0];
    }
    void pop() {
        if (size == 0) {
            cerr << "Priority queue is empty" << endl;
            exit(1);
        }
        data[0] = data[size - 1];
        size--;
        int i = 0;
        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int largest = i;
            if (left < size && compare(data[left], data[largest])) {
                largest = left;
            }
            if (right < size && compare(data[right], data[largest])) {
                largest = right;
            }
            if (largest != i) {
                Ticket temp = data[i];
                data[i] = data[largest];
                data[largest] = temp;
                i = largest;
            } else {
                break;
            }
        }
    }
    bool empty() const {
        return size == 0;
    }
};

// Process ticket sales based on priority
void processTicketSales(const TicketArray* tickets) {
    if (!tickets) return;
    TicketPriorityQueue pq;
    for (int i = 0; i < tickets->getSize(); i++) {
         pq.push((*tickets)[i]);
    }
    cout << "\nProcessing ticket sales in priority order:" << endl;
    cout << left << setw(10) << "TicketID" 
         << left << setw(20) << "Name" 
         << left << setw(14) << "Category" 
         << left << setw(22) << "PurchaseTime" 
         << endl;
    cout << string(70, '-') << endl;
    while (!pq.empty()) {
         Ticket t = pq.top();
         cout << left << setw(10) << t.ticketID 
              << left << setw(20) << t.name 
              << left << setw(14) << t.category 
              << left << setw(22) << t.purchaseTime 
              << endl;
         pq.pop();
    }
}

// Modified function for managing spectator entry and exit:
// Only displays the current status without automatically updating all tickets.
void manageSpectatorFlow(TicketArray* tickets) {
    if (!tickets) return;
    cout << "\nSpectator Entry Status:" << endl;
    cout << left << setw(10) << "TicketID" 
         << left << setw(20) << "Name" 
         << left << setw(30) << "EntryStatus" 
         << left << setw(10) << "Seat" 
         << endl;
    cout << string(70, '-') << endl;
    for (int i = 0; i < tickets->getSize(); i++) {
         Ticket &t = (*tickets)[i];
         string entryStr = t.entryStatus ? "Already entered" : "Not yet";
         cout << left << setw(10) << t.ticketID 
              << left << setw(20) << t.name 
              << left << setw(30) << entryStr 
              << left << setw(10) << t.seatNumber 
              << endl;
    }
    cout << "\nSpectator Exit Status:" << endl;
    cout << left << setw(10) << "TicketID" 
         << left << setw(20) << "Name" 
         << left << setw(30) << "ExitStatus" 
         << endl;
    cout << string(60, '-') << endl;
    for (int i = 0; i < tickets->getSize(); i++) {
         Ticket &t = (*tickets)[i];
         string exitStr = t.exitStatus ? "Leaving" : "Not leaving yet";
         cout << left << setw(10) << t.ticketID 
              << left << setw(20) << t.name 
              << left << setw(30) << exitStr 
              << endl;
    }
}

// Organize ticket sales
void organizeTicketSales(const TicketArray* tickets) {
    if (!tickets) return;
    TicketArray vipTickets;
    TicketArray earlyBirdTickets;
    TicketArray publicTickets;
    for (int i = 0; i < tickets->getSize(); i++) {
         const Ticket &t = (*tickets)[i];
         if (t.category == "VIP")
              vipTickets.push_back(t);
         else if (t.category == "Early-Bird")
              earlyBirdTickets.push_back(t);
         else if (t.category == "Public")
              publicTickets.push_back(t);
    }
    cout << "\nTicket Organization Summary:" << endl;
    cout << left << setw(15) << "Category" 
         << left << setw(10) << "Count" << endl;
    cout << string(25, '-') << endl;
    cout << left << setw(15) << "VIP" 
         << left << setw(10) << vipTickets.getSize() << endl;
    cout << left << setw(15) << "Early-Bird" 
         << left << setw(10) << earlyBirdTickets.getSize() << endl;
    cout << left << setw(15) << "Public" 
         << left << setw(10) << publicTickets.getSize() << endl;
    const int MAX_COUNTER_CAPACITY = 10;
    cout << "\nTicket Counter Status:" << endl;
    if (vipTickets.getSize() > MAX_COUNTER_CAPACITY)
         cout << "Warning: VIP ticket counter is overcrowded. Consider additional counters." << endl;
    if (earlyBirdTickets.getSize() > MAX_COUNTER_CAPACITY)
         cout << "Warning: Early-Bird ticket counter is overcrowded. Consider additional counters." << endl;
    if (publicTickets.getSize() > MAX_COUNTER_CAPACITY)
         cout << "Warning: Public ticket counter is overcrowded. Consider additional counters." << endl;
    if (vipTickets.getSize() <= MAX_COUNTER_CAPACITY && 
        earlyBirdTickets.getSize() <= MAX_COUNTER_CAPACITY && 
        publicTickets.getSize() <= MAX_COUNTER_CAPACITY)
         cout << "Ticket counters are well-organized." << endl;
}

// Function for administrators to modify ticket data, with an "0: Exit modifications" option
void modifyTicketData(TicketArray* tickets) {
    if (!tickets) return;
    int id;
    cout << "\nEnter TicketID to modify: ";
    cin >> id;
    int index = -1;
    for (int i = 0; i < tickets->getSize(); i++) {
        if ((*tickets)[i].ticketID == id) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        cout << "Ticket with ID " << id << " not found." << endl;
        return;
    }
    Ticket &t = (*tickets)[index];
    string entryStr = t.entryStatus ? "Already entered" : "Not yet";
    string exitStr  = t.exitStatus  ? "Leaving" : "Not leaving yet";
    cout << "\nCurrent Ticket Data:" << endl;
    cout << left << setw(10) << "TicketID" 
         << left << setw(20) << "Name" 
         << left << setw(14) << "Category" 
         << left << setw(22) << "PurchaseTime" 
         << left << setw(18) << "EntryStatus" 
         << left << setw(10) << "Seat" 
         << left << setw(10) << "Price" 
         << left << setw(16) << "PaymentStatus" 
         << left << setw(18) << "ExitStatus" 
         << endl;
    cout << string(128, '-') << endl;
    cout << left << setw(10) << t.ticketID
         << left << setw(20) << t.name
         << left << setw(14) << t.category
         << left << setw(22) << t.purchaseTime
         << left << setw(18) << entryStr
         << left << setw(10) << t.seatNumber
         << left << setw(10) << fixed << setprecision(2) << t.price
         << left << setw(16) << t.paymentStatus
         << left << setw(18) << exitStr
         << endl;
    
    int choice;
    do {
        cout << "\nSelect field to modify (or 0 to exit modifications):" << endl;
        cout << "0. Exit modifications" << endl;
        cout << "1. Name" << endl;
        cout << "2. Category" << endl;
        cout << "3. PurchaseTime" << endl;
        cout << "4. Entry Status (0: Not yet, 1: Already entered)" << endl;
        cout << "5. Seat" << endl;
        cout << "6. Price" << endl;
        cout << "7. PaymentStatus" << endl;
        cout << "8. Exit Status (0: Not leaving yet, 1: Leaving)" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        if (choice == 0) {
            cout << "Exiting modifications for TicketID: " << t.ticketID << endl;
            break;
        }
        switch(choice) {
            case 1: {
                cout << "Enter new Name: ";
                string newName;
                cin.ignore();
                getline(cin, newName);
                t.name = newName;
                cout << "Name updated." << endl;
                break;
            }
            case 2: {
                cout << "Enter new Category (VIP, Early-Bird, Public): ";
                string newCategory;
                cin >> newCategory;
                t.category = newCategory;
                cout << "Category updated." << endl;
                break;
            }
            case 3: {
                cout << "Enter new PurchaseTime (e.g., YYYY-MM-DD HH:MM:SS): ";
                string newPurchaseTime;
                cin.ignore();
                getline(cin, newPurchaseTime);
                t.purchaseTime = newPurchaseTime;
                cout << "PurchaseTime updated." << endl;
                break;
            }
            case 4: {
                cout << "Enter new Entry Status (0: Not yet, 1: Already entered): ";
                int newEntry;
                cin >> newEntry;
                t.entryStatus = (newEntry == 1);
                cout << "Entry Status updated." << endl;
                break;
            }
            case 5: {
                cout << "Enter new Seat: ";
                string newSeat;
                cin >> newSeat;
                t.seatNumber = newSeat;
                cout << "Seat updated." << endl;
                break;
            }
            case 6: {
                cout << "Enter new Price: ";
                double newPrice;
                cin >> newPrice;
                t.price = newPrice;
                cout << "Price updated." << endl;
                break;
            }
            case 7: {
                cout << "Enter new PaymentStatus (Paid/Unpaid): ";
                string newPayment;
                cin >> newPayment;
                t.paymentStatus = newPayment;
                cout << "PaymentStatus updated." << endl;
                break;
            }
            case 8: {
                cout << "Enter new Exit Status (0: Not leaving yet, 1: Leaving): ";
                int newExit;
                cin >> newExit;
                t.exitStatus = (newExit == 1);
                cout << "Exit Status updated." << endl;
                break;
            }
            default:
                cout << "Invalid choice, try again." << endl;
                break;
        }
    } while(choice != 0);
    updateCSVFile(tickets, "ticket_sales.csv");
}

// New functionality: Administrator updates the entry/exit status for a single ticket
void updateEntryExitStatus(TicketArray* tickets) {
    if (!tickets) return;
    int id;
    cout << "\nEnter TicketID to update entry/exit status: ";
    cin >> id;
    int index = -1;
    for (int i = 0; i < tickets->getSize(); i++) {
        if ((*tickets)[i].ticketID == id) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        cout << "Ticket with ID " << id << " not found." << endl;
        return;
    }
    Ticket &t = (*tickets)[index];
    int entry, exitStatus;
    cout << "Select Entry Status: (0: Not yet, 1: Already entered): ";
    cin >> entry;
    t.entryStatus = (entry == 1);
    cout << "Select Exit Status: (0: Not leaving yet, 1: Leaving): ";
    cin >> exitStatus;
    t.exitStatus = (exitStatus == 1);
    cout << "Entry and Exit status updated for TicketID: " << t.ticketID << endl;
    updateCSVFile(tickets, "ticket_sales.csv");
}

// Administrator deletion function
void deleteTicketData(TicketArray* tickets) {
    if (!tickets) return;
    int id;
    cout << "\nEnter TicketID to delete: ";
    cin >> id;
    int index = -1;
    for (int i = 0; i < tickets->getSize(); i++) {
        if ((*tickets)[i].ticketID == id) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        cout << "Ticket with ID " << id << " not found." << endl;
        return;
    }
    cout << "Deleting TicketID: " << id << ", Name: " << (*tickets)[index].name << endl;
    tickets->removeAt(index);
    cout << "Ticket deleted successfully." << endl;
    updateCSVFile(tickets, "ticket_sales.csv");
}

// Administrator function to add a new ticket (updated):
// After selecting Name and Category, the administrator only inputs the numeric part of the seat number.
// The system automatically adds a prefix and checks for duplicates.
void addTicketData(TicketArray* tickets) {
    if (!tickets) return;
    Ticket newTicket;
    int maxID = 0;
    for (int i = 0; i < tickets->getSize(); i++) {
        if ((*tickets)[i].ticketID > maxID)
            maxID = (*tickets)[i].ticketID;
    }
    newTicket.ticketID = (maxID == 0) ? 1001 : maxID + 1;
    
    cin.ignore(); // Clear input buffer
    cout << "\n--- Add New Ticket ---" << endl;
    cout << "Enter Name: ";
    getline(cin, newTicket.name);
    
    // Choose Category
    int catChoice;
    cout << "Select Category:" << endl;
    cout << "1. VIP" << endl;
    cout << "2. Early-Bird" << endl;
    cout << "3. Public" << endl;
    cout << "Enter your choice: ";
    cin >> catChoice;
    switch(catChoice) {
        case 1:
            newTicket.category = "VIP";
            break;
        case 2:
            newTicket.category = "Early-Bird";
            break;
        case 3:
            newTicket.category = "Public";
            break;
        default:
            cout << "Invalid choice, defaulting to Public." << endl;
            newTicket.category = "Public";
            break;
    }
    
    // Determine seat prefix based on category
    string prefix;
    if(newTicket.category == "VIP")
        prefix = "A";
    else if(newTicket.category == "Early-Bird")
        prefix = "B";
    else if(newTicket.category == "Public")
        prefix = "C";
    
    // Administrator inputs only the numeric part of the seat number.
    // The system automatically adds the prefix and checks for duplicates.
    bool duplicate = true;
    string seatInput;
    cin.ignore(); // Clear newline character
    while (duplicate) {
        cout << "Enter Seat Number (numeric part only): ";
        getline(cin, seatInput);
        newTicket.seatNumber = prefix + seatInput;
        if (isSeatNumberExists(tickets, newTicket.seatNumber)) {
            cout << "Seat " << newTicket.seatNumber << " already exists. Please enter a different seat number." << endl;
        } else {
            duplicate = false;
        }
    }
    
    // Automatically get current system time
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char timeStr[20];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", ltm);
    newTicket.purchaseTime = timeStr;
    
    // Choose Entry Status
    int entryChoice;
    cout << "Select Entry Status: (0: Not yet, 1: Already entered): ";
    cin >> entryChoice;
    newTicket.entryStatus = (entryChoice == 1);
    
    // Set Price automatically based on category
    if(newTicket.category == "VIP")
        newTicket.price = 150.00;
    else if(newTicket.category == "Early-Bird")
        newTicket.price = 100.00;
    else if(newTicket.category == "Public")
        newTicket.price = 80.00;
    
    // Choose Payment Status
    int payChoice;
    cout << "Select Payment Status:" << endl;
    cout << "1. Paid" << endl;
    cout << "2. Unpaid" << endl;
    cout << "Enter your choice: ";
    cin >> payChoice;
    newTicket.paymentStatus = (payChoice == 1) ? "Paid" : "Unpaid";
    
    // Choose Exit Status
    int exitChoice;
    cout << "Select Exit Status: (0: Not leaving yet, 1: Leaving): ";
    cin >> exitChoice;
    newTicket.exitStatus = (exitChoice == 1);
    
    tickets->push_back(newTicket);
    cout << "\nNew ticket added successfully with TicketID: " << newTicket.ticketID << endl;
    updateCSVFile(tickets, "ticket_sales.csv");
}

// Menu function for ticket management (with exit options for each submenu)
void menu(TicketArray* tickets) {
    int choice;
    do {
        cout << "\nMenu:" << endl;
        cout << "1. Display Tickets" << endl;
        cout << "2. Process Ticket Sales" << endl;
        cout << "3. Manage Spectator Flow" << endl;
        cout << "4. Organize Ticket Sales" << endl;
        cout << "5. Modify Ticket Data" << endl;
        cout << "6. Add New Ticket" << endl;
        cout << "7. Delete Ticket" << endl;
        cout << "8. Update Entry/Exit Status" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        switch(choice) {
            case 1:
                displayTickets(tickets);
                break;
            case 2:
                processTicketSales(tickets);
                break;
            case 3:
                manageSpectatorFlow(tickets);
                break;
            case 4:
                organizeTicketSales(tickets);
                break;
            case 5:
                displayTickets(tickets);
                modifyTicketData(tickets);
                break;
            case 6:
                addTicketData(tickets);
                break;
            case 7:
                deleteTicketData(tickets);
                break;
            case 8:
                displayTickets(tickets);
                updateEntryExitStatus(tickets);
                break;
            case 9:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice, please try again." << endl;
                break;
        }
    } while(choice != 9);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// TASK 3: HANDLING PLAYER WITHDRAWALS
// Define maximum number of players
const int MAX_PLAYERS = 50;

// Player data structure
struct Player {
    int number;
    string name;
    int age;
    string substitute;
    int subAge;
    bool hasWithdrawn;
    string withdrawalReason;
    bool needsSubstitute;
};

void readCSV(Player players[], int &playerCount) {
    ifstream file("player_and_Substitute.csv");
    if (!file.is_open()) {
        cout << "Error: Could not open file!" << endl;
        return;
    }

    string line;
    getline(file, line); // Read and skip header line
    
    while (getline(file, line) && playerCount < MAX_PLAYERS) {
        stringstream ss(line);
        string temp;

        getline(ss, temp, ','); 
        players[playerCount].number = stoi(temp);
        getline(ss, players[playerCount].name, ',');
        getline(ss, temp, ','); 
        players[playerCount].age = stoi(temp);
        getline(ss, players[playerCount].substitute, ',');
        getline(ss, temp, ','); 
        players[playerCount].subAge = stoi(temp);
        getline(ss, temp, ','); 
        players[playerCount].hasWithdrawn = (temp == "true");
        getline(ss, players[playerCount].withdrawalReason, ',');
        getline(ss, temp, ','); 
        players[playerCount].needsSubstitute = (temp == "true");

        playerCount++;
    }
    file.close();
}

void printPlayers(const Player players[], int playerCount) {
    cout << "=================================\n";
    cout << "=  HANDLING PLAYER WITHDRAWALS  =\n";
    cout << "=================================\n";
    cout << left << setw(5) << "No" 
         << setw(15) << "PLAYER" 
         << setw(10) << "AGE" 
         << setw(15) << "SUBSTITUTE"
         << setw(10) << "SUB AGE" 
         << setw(15) << "WITHDRAWN" 
         << setw(20) << "WITHDRAWAL REASON" 
         << setw(10) << "SUB NEED" << endl;
    cout << string(90, '-') << endl;
    
    for (int i = 0; i < playerCount; i++) {
        cout << left << setw(5) << players[i].number 
             << setw(15) << players[i].name 
             << setw(10) << players[i].age
             << setw(15) << players[i].substitute 
             << setw(10) << players[i].subAge
             << setw(15) << (players[i].hasWithdrawn ? "Yes" : "No")
             << setw(20) << players[i].withdrawalReason
             << setw(10) << (players[i].needsSubstitute ? "Yes" : "No") << endl;
    }
}

void writeCSV(const Player players[], int playerCount) {
    ofstream file("player_and_Substitute.csv");
    if (!file.is_open()) {
        cout << "Error: Could not open file for writing!" << endl;
        return;
    }

    // Write CSV header
    file << "number,name,age,substitute,subAge,hasWithdrawn,withdrawalReason,needsSubstitute\n";

    // Write all player data
    for (int i = 0; i < playerCount; i++) {
        file << players[i].number << ","
             << players[i].name << ","
             << players[i].age << ","
             << players[i].substitute << ","
             << players[i].subAge << ","
             << (players[i].hasWithdrawn ? "true" : "false") << ","
             << players[i].withdrawalReason << ","
             << (players[i].needsSubstitute ? "true" : "false") << "\n";
    }

    file.close();
    cout << "Player data successfully updated and saved!" << endl;
}

void updatePlayerData(Player players[], int playerCount) {
    printPlayers(players, playerCount);
    int playerNumber;
    cout << "Enter player number to update: ";
    cin >> playerNumber;
    
    for (int i = 0; i < playerCount; i++) {
        if (players[i].number == playerNumber) {
            cout << "Updating player: " << players[i].name << endl;
            cout << "Current Information:" << endl;
            printPlayers(&players[i], 1);
            
            int choice;
            do {
                cout << "Select field to modify (or 0 to exit modifications):" << endl;
                cout << "1. PLAYER" << endl;
                cout << "2. PLAYER AGE" << endl;
                cout << "3. SUBSTITUTE" << endl;
                cout << "4. SUBSTITUTE AGE" << endl;
                cout << "5. Has Withdrawn" << endl;
                cout << "Enter your choice: ";
                cin >> choice;
                cin.ignore();

                switch (choice) {
                    case 1:
                        cout << "Enter new name: ";
                        getline(cin, players[i].name);
                        break;
                    case 2:
                        cout << "Enter new age: ";
                        cin >> players[i].age;
                        break;
                    case 3:
                        cout << "Enter new substitute name: ";
                        cin.ignore();
                        getline(cin, players[i].substitute);
                        break;
                    case 4:
                        cout << "Enter new substitute age: ";
                        cin >> players[i].subAge;
                        break;
                    case 5:
                        cout << "Has Withdrawn (1 for yes, 0 for no): ";
                        cin >> players[i].hasWithdrawn;
                        if (players[i].hasWithdrawn) {
                            cout << "Reason for Withdrawal: ";
                            cin.ignore();
                            getline(cin, players[i].withdrawalReason);
                            cout << "Needs Substitute Participation (1 for yes, 0 for no): ";
                            cin >> players[i].needsSubstitute;
                        } else {
                            players[i].withdrawalReason = "";
                            players[i].needsSubstitute = false;
                        }
                        break;
                    case 0:
                        cout << "Exiting modifications." << endl;
                        break;
                    default:
                        cout << "Invalid choice!" << endl;
                }
                cout << "Updated Player Information:" << endl;
                printPlayers(&players[i], 1);
            } while (choice != 0);

            // After data update, write back to the CSV file
            writeCSV(players, playerCount);
            return;
        }
    }
    cout << "Player not found!" << endl;
}

void menu(Player players[], int &playerCount) {
    int choice;
    do {
        cout << "=================================" << endl;
        cout << "=  HANDLING PLAYER WITHDRAWALS  =" << endl;
        cout << "=================================" << endl;
        cout << "1. Display player information" << endl;
        cout << "2. Update player data (Withdrawn)" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                printPlayers(players, playerCount);
                break;
            case 2:
                updatePlayerData(players, playerCount);
                break;
            case 3:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice! Try again." << endl;
        }
    } while (choice != 3);
}

// Final Menu Function
void final_menu() {
    int choice;
    do {
        cout << string(62, '=') << endl;
        cout << "ASIA PACIFIC UNIVERSITY TENNIS CHAMPIONSHIP MANAGEMENT SYSTEM" << endl;
        cout << string(62, '=') << endl;
        cout << "1. TASK 1: TOURNAMENT SCHEDULING AND PLAYER PROGRESSION" << endl;
        cout << "   TASK 4: MATCH HISTORY TRACKING" << endl;
        cout << "\n2. TASK 2: TICKET SALES AND SPECTATOR MANAGEMENT" << endl;
        cout << "3. TASK 3: HANDLING PLAYER WITHDRAWALS" << endl;
        cout << "4. EXIT" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();
    
        switch(choice) {
            case 1:
            {
                // TASK 1 & TASK 4: Tournament schedule, player progression, and history tracking
                const string filename = "tennis.csv";
                const string header = "ID, DATE, TIME, COURT, PLAYER, SCORE, WINNER, LOSER";
                csv_file(filename, header);
                tournament tour;
                history matchHistory;
                // Check if the CSV file already contains data (beyond the header)
                ifstream infile(filename);
                int lineCount = 0;
                string temp;
                while(getline(infile, temp))
                    lineCount++;
                infile.close();

                if(lineCount == 1) {
                    tour.simulateTournament();
                    tour.updateSubstitutions();
                }
                else {
                    tour.load_from_file(filename);
                    cout << "Loaded existing match schedule from CSV file." << endl;
                }
                tour.update_history(matchHistory);
                tour.save_to_csv(filename);
                display_menu(tour, matchHistory, filename);
                break;
            }
            case 2:
            {
                // TASK 2: Ticket sales and spectator management
                TicketArray* tickets = readTickets("ticket_sales.csv");
                if (tickets != nullptr) {
                    menu(tickets);
                    delete tickets;
                } else {
                    cout << "Failed to load ticket data from ticket_sales.csv" << endl;
                }
                break;
            }
            case 3:
            {
                // TASK 3: Handling player withdrawals
                Player players[MAX_PLAYERS];
                int playerCount = 0;
                readCSV(players, playerCount);
                menu(players, playerCount);
                break;
            }
            case 4:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice, please try again." << endl;
                break;
        }
    } while(choice != 4);
}

int main()
{
    srand(time(0)); // Initialize the random seed

    const string filename = "tennis.csv";
    const string header = "ID, DATE, TIME, COURT, PLAYER, SCORE, WINNER, LOSER";

    // Create CSV file if it doesn't exist.
    csv_file(filename, header);

    // Create a tournament and history object.
    tournament tour;
    history matchHistory;

    // Check if the CSV file already contains data (beyond the header)
    ifstream infile(filename);
    int lineCount = 0;
    string temp;
    while(getline(infile, temp))
        lineCount++;
    infile.close();

    if(lineCount == 1)
    {
        tour.simulateTournament();
        tour.updateSubstitutions();
    }
    else
    {
        tour.load_from_file(filename);
        cout << "Loaded existing match schedule from CSV file." << endl;
        tour.updateSubstitutions();
    }

    tour.update_history(matchHistory);
    tour.save_to_csv(filename);
    // Finally, call the interactive menu.

    //task 2
    TicketArray* tickets = readTickets("ticket_sales.csv");
    delete tickets;

    // task 3 
    Player players[MAX_PLAYERS];
    int playerCount = 0;

    readCSV(players, playerCount);

    final_menu();

    return 0;
}
