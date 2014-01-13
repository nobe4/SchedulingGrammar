//
//
//  tp4graphes
//  Created by Matthieu de La Roche Saint-Andre and Victor Haffreingue on 01/13/2014.
//

#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

// Cell of the adjacency matrix
// This structure is used to store values in adjacency matrix
// The field "valid" is null when the value is not valid,
// and has any other value otherwise.
// The field "value" is the weight of the link.
typedef struct _adjCell {
    int value, valid;
} AdjCell;

// Link in the graph.
// This structure is used to store a link.
// The field "value" is the weight of the link.
// The field "edge1" is the first extremity of the link.
// The field "value" is the second extremity of the link.
typedef struct _link {
    int value, edge1, edge2;
} Link;

typedef vector<vector<AdjCell> > AdjMatrix;

void print(const AdjMatrix &adjMatrix); // prints adjacency matrix

bool cycle(const AdjMatrix &adjMatrix); // checks cycle presence in adjacency matrix
bool circuit(const AdjMatrix &adjMatrix); // checks circuits presence in adjacency matrix
void generateAdjMatrix(vector<Link> linkArray, AdjMatrix &adjMatrix, int size); // generate adjacency matrix based on an array of links

/*
 * Parses a file, and stores the result in adjMatrix.
 */
void parseFile(string path, AdjMatrix &adjMatrix);

/*
 * Reads a line from user and modify adjMatrix based on that order.
 * You can either create (c), delete (d), verify constraint existence (v), stop (s) 
 * (If ever you do not have any modification to perform.)
 *
 * Returns true if an operation has been performed, return false otherwise.
 */
bool executeCommand(AdjMatrix adjMatrix);

/*
 * Computes earliest and latest dates based on adjMatrix.
 */
void computeDates(const AdjMatrix &adjMatrix, vector<int> &earliestDates, vector<int> &latestDates);

/*
 * Prints earliest and latest dates.
 */
void printCalendar(vector<int> &earliestDates, vector<int> &latestDates);

bool circuit(const AdjMatrix &adjMatrix) {
    // TODO
    return true;
}

bool executeCommand(AdjMatrix adjMatrix) {
    string s;
    bool goOn = true;
    while(goOn) {
        getline(cin, s);
        if (s == "stop" || s == "s") {
            goOn = false;
            return false;
        } /*else if {
            // TODO
            // other options
        }*/
    }
    return true;
}

void computeDates(const AdjMatrix &adjMatrix, vector<int> &earliestDates, vector<int> &latestDates) {
    // TODO
}

void printCalendar(vector<int> &earliestDates, vector<int> &latestDates) {
    // TODO
}

void parseFile(string path, AdjMatrix &adjMatrix) {
    // TODO
}

int main(int argc, const char * argv[]) {
    string path;
    if (argc == 2) {
        path = argv[1];
    } else {
        cout << "Which file do you want to open ? ";
        cin >> path;
        cout << endl;
    }
//    path = "/Users/matthieudelaro/Documents/classes/L3/S5/theorie_des_graphes/TP/tp33/Haffreingue-de_La_Roche_Saint_Andre-EFREI-L3-TG-TP3-5.txt";
    
    AdjMatrix adjMatrix;
    parseFile(path, adjMatrix);
    
    bool goOn = true;
    while (goOn) {
        if (circuit(adjMatrix)) {
            cout << "There is one or several circuits. Please delete them." << endl;
            executeCommand(adjMatrix);
        } else {
            cout << "There is not any circuit." << endl;
            vector<int> latestDates, earliestDates;
            computeDates(adjMatrix, earliestDates, latestDates);
            
            cout << "Here is the calendar :" << endl;
            printCalendar(earliestDates, latestDates);
            
            cout << "Would you like to create, delete, or verify the existence of a constraint ?" << endl;
            cin >> goOn;
            if (goOn) {
                executeCommand(adjMatrix);
            }
        }
    }
}

void print(const vector<vector<AdjCell> > &adjMatrix) {
    long size = adjMatrix.size();
    for (int j = 0; j < size; ++j) {
        cout << "\t" << j;
    }
    for (int i = 0; i < size; ++i) {
        cout << endl << i << "\t";
        for (int j = 0; j < size; ++j) {
            if (adjMatrix[i][j].valid) {
                cout << adjMatrix[i][j].value << "\t";
            } else {
                cout << ".\t";
            }
        }
    }
    cout << endl;
}

// checks cycle presence in adjacency matrix
bool cycle(const vector<vector<AdjCell> > &adjMatrix) {
    long size = adjMatrix.size();
    // Create an array the size of the matrix.
    // For each node i, deleted[i]
    // stores whether i has been deleted.
    vector<bool> deleted(size, false);
    
    // Each time we find a new node,
    // we incremente the value of deletedQuantity.
    // This way, we know when we reach the end
    // of the algorithm (deletedQuantity == size).
    int deletedQuantity = 0;
    
    // Used to store previous
    // value of foundQuantity.
    int previousDeletedQuantity = 0;
    
    int current = 0;
    bool goOn = true;
    while (deletedQuantity < size && goOn) {
        if (deleted[current] == false) {
            
            int destinationQuantity = 0;
            for (int destination = 0; destination < size && destinationQuantity < 2; ++destination) {
                if (adjMatrix[current][destination].valid && !deleted[destination]) {
                    destinationQuantity++;
                }
            }
            
            // if the node is an entry point in the graph,
            // we delete this node
            if (destinationQuantity < 2) {
                deleted[current] = true;
                deletedQuantity++;
//                cout << current << " has " << destinationQuantity << " neighbour. We delete it." << endl;
            }
        }
        
        if (current == size - 1) {
            if (deletedQuantity == previousDeletedQuantity) {
                goOn = false;
            } else {
                previousDeletedQuantity = deletedQuantity;
                current = 0;
            }
        } else {
            current++;
        }
    }
    
//    cout << endl;
//    cout << endl;
    if (deletedQuantity == size) {
//        cout << "Each node has been deleted." << endl;
        return false;
    } else {
//        cout << "Some node(s) has not been deleted." << endl;
        return true;
    }
}

// generate adjacency matrix based on an array of links
void generateAdjMatrix(vector<Link> linkArray, vector<vector<AdjCell> > &adjMatrix, int size) {
    // initialise adjMatrix to the right size, with invalid cells
    AdjCell init;
    init.valid = false;
    adjMatrix.resize(size);
    for (int i = 0; i < size; ++i) {
        adjMatrix[i].resize(size);
        for (int j = 0; j < size; ++j) {
            adjMatrix[i][j] = init;
        }
    }
    
    // add links one after another, twice : back and forth (for the graph is not oriented)
    for (int i = 0; i < linkArray.size(); ++i) {
        adjMatrix[linkArray[i].edge1][linkArray[i].edge2].valid = true;
        adjMatrix[linkArray[i].edge1][linkArray[i].edge2].value = linkArray[i].value;
        
        
        adjMatrix[linkArray[i].edge2][linkArray[i].edge1].valid = true;
        adjMatrix[linkArray[i].edge2][linkArray[i].edge1].value = linkArray[i].value;
    }
}
