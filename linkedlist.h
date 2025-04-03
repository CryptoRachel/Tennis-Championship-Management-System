#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <fstream>   
#include <sstream> 
#include <iomanip>

using namespace std;

//=== Match structure and linkedlist node for stored a match record.
template <typename T>
struct Match
{
    T id;
    T date;
    T time;
    T court;
    T player;
    T score;
    T winner;
    T loser;
    T stage;
    Match* next;

    Match() : next(nullptr) {}

    Match(const Match& m)
    : id(m.id), date(m.date), time(m.time), court(m.court),player(m.player), score(m.score), winner(m.winner), loser(m.loser), stage(m.stage), next(nullptr) {}
};

//=== Task 1: Circular queue algorithm for match schedule.
template <typename T>
class circular_queue
{
    private:
        Match<T>* front;
        Match<T>* rear;

    public:
        circular_queue() : front(nullptr), rear(nullptr){}

        ~circular_queue()
        {
            while (!isEmpty())
            {
                dequeue();
            }
        }

        // == Define is the list was empty
        bool isEmpty() const
        {
            return front == nullptr;
        }

        // == Add new match m to the circular lists
        void enqueue(const Match<T>& m)
        {
            Match<T>* newMatch = new Match<T>(m);
            if(isEmpty())
            {
                front = rear = newMatch;
                newMatch -> next = newMatch;
            }
            else
            {
                newMatch -> next = front;
                rear -> next = newMatch;
                rear = newMatch;
            }
        }

        // == Remove the head match and return copy
        Match<T> dequeue()
        {
            if (isEmpty()) 
            {
                cout << "Queue is empty!" << endl;
                exit(1);
            }

            Match<T>* temp = front;

            //Copy node data
            Match<T> m = *temp; 

            //when the lists only have one node
            if (front == rear) 
            {  
                front = rear = nullptr;
            } 
            else 
            {
                front = front->next;
                rear->next = front;
            }
            delete temp;
            return m;
        }

        // == Display match schedule
        void display() const
        {
            if (isEmpty()) 
            {
                cout << "No scheduled matches." << endl;
                return;
            }

            // Traverse starting from FRONT until it loops back to FRONT
            Match<T>* current = front; 
            do 
            {
                cout << left
                    << setw(6)  << current->id
                    << setw(12) << current->date
                    << setw(6)  << current->time
                    << setw(10) << current->court
                    << setw(30) << current->player
                    << setw(15) << current->score
                    << setw(10) << current->winner
                    << setw(10) << current->loser << endl;
                current = current->next;
            } 
            while (current != front);
        }
        
        // == Remove match records recording to the id.
        bool remove(const T &id) 
        {
            if (isEmpty()) return false;
            Match<T>* current = front;
            Match<T>* previous = rear; // In a circular list, the previous of front is rear.
            do 
            {
                if (current->id == id) 
                {
                    // If there's only one node.
                    if (front == rear && current == front) 
                    {
                        delete current;
                        front = rear = nullptr;
                    } 
                    else 
                    {
                        previous->next = current->next;
                        // If deleting the front node, update the front pointer.
                        if (current == front) 
                        {
                            front = current->next;
                            rear->next = front;
                        }
                        // If deleting the rear node, update the rear pointer.
                        if (current == rear) 
                        {
                            rear = previous;
                            rear->next = front;
                        }
                        delete current;
                    }
                    return true;
                }
                previous = current;
                current = current->next;
            } while (current != front);
            return false; // No matching node found.
        }

        //Searching function for modify match record by using match ID
        Match<T>* search(const T& id)
        {
            if (isEmpty()) return nullptr;
            Match<T>* current = front;
            do 
            {
                if(current->id == id)
                    return current;
                current = current->next;
            } while(current != front);
            return nullptr;
        }

        // Return the head pointer
        Match<T>* getFront() const 
        { 
            return front; 
        }
};


// === Task 4: Stack algorithm for match history.
template <typename T>
class stack
{
    private:
        struct Node 
        {
            T data;
            Node* next;
            Node(const T &data) : data(data), next(nullptr) {}
        };

        Node* topNode;

    public:
        stack() : topNode(nullptr) {}

        ~stack() 
        {
            while (!empty())
                pop();
        }

        // == Check if the stack was empty.
        bool empty() const 
        {
            return topNode == nullptr;
        }

        // == Insert the new match onto the stack.
        void push(const T& item) 
        {
            Node* newNode = new Node(item);
            newNode->next = topNode;
            topNode = newNode;
        }

        // == Remove the match on the stack and return stak top match. 
        T pop() 
        {
            if (empty()) 
            {
                cout << "Stack is empty!" << endl;
                exit(1);
            }

            Node* temp = topNode;
            T item = topNode->data;
            topNode = topNode->next;

            delete temp;
            return item;
        }

        // == Return a reference to the top match of the stack without removing it.
        T& top() 
        {
            if (empty()) 
            {
                cout << "Stack is empty!" << endl;
                exit(1);
            }
            return topNode->data;
        }
};

#endif