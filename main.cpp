//
//
//  tp4graphes
//  Created by Matthieu de La Roche Saint-Andre and Victor Haffreingue on 01/13/2014.
//

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iterator>

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

bool circuit(const AdjMatrix &adjMatrix); // checks circuits presence in adjacency matrix

/*
 * Parses a file, and stores the result in adjMatrix.
 */
void parseFile(string path, AdjMatrix &adjMatrix);

bool hasPredecessor(int node, const AdjMatrix &adjMatrix); // checks whether node has predecessor(s)
bool hasSuccessor(int node, const AdjMatrix &adjMatrix); // checks whether node has successor(s)

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

// split the input string from console with whitespaces
vector<string> split(string const &input);

// parse string to int and return it
// careful : 0 is either "0" of an invalid string parsed
int parseInt(string s);

int main(int argc, const char * argv[]) {
    string path;
//    if (argc == 2) {
//        path = argv[1];
//    } else {
//        cout << "Which file do you want to open ? ";
//        cin >> path;
//        cout << endl;
//    }
//    path = "/Users/matthieudelaro/Documents/classes/L3/S5/theorie_des_graphes/TP/tp33/Haffreingue-de_La_Roche_Saint_Andre-EFREI-L3-TG-TP3-5.txt";
    path = "/Users/matthieudelaro/Documents/classes/L3/S5/theorie_des_graphes/TP/tp4/grammar.g";
//    path = "grammar.g";
    
    AdjMatrix adjMatrix;
    cout << "Parsing file " << path << " :" << endl;
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

bool circuit(const AdjMatrix &adjMatrix) {
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
            // if the node is an entry point in the graph,
            // we delete this node.
            if (!hasPredecessor(current, adjMatrix)) {
                deleted[current] = true;
                deletedQuantity++;
                cout << current << " has at least one predecessor. We delete it." << endl;
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
    
    cout << endl;
    cout << endl;
    if (deletedQuantity == size) {
        cout << "Each node has been deleted." << endl;
        return false;
    } else {
        cout << "Some node(s) has not been deleted." << endl;
        return true;
    }

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
           // TODO : get inspiration from parseInput
           // other options
           }*/
    }
    return true;
}

void computeDates(const AdjMatrix &adjMatrix, vector<int> &earliestDates, vector<int> &latestDates) {
    long size = adjMatrix.size();
    
    ///////////////////////
    ///////// Compute ranks
    ///////////////////////
    
    // Create an array the size of the matrix.
    // For each node i, ranks[i]
    // stores the rank of node i, or -1 if the rank
    // has not been computed yet.
    vector<int> ranks(size, -1);
    
    int computedRankQuantity = 0;
    // initialisation : find entry nodes
    for (int node = 0; node < size; ++node) {
        if (!hasPredecessor(node, adjMatrix)) {
            ranks[node] = 0;
            computedRankQuantity++;
        }
    }
    
    int currentRank = 1;
    // While some nodes has not been found,
    // iterate over more recently found nodes,
    // and look for their successors.
    while (computedRankQuantity != size) {
        for (int node = 0; node < size && computedRankQuantity != size; ++node) {
            // if it is a node we just found
            if (ranks[node] == currentRank - 1) {
                // we look for its successors
                for (int newNode = 0; newNode < size && computedRankQuantity != size; ++newNode) {
                    if (adjMatrix[node][newNode].valid) {
                        ranks[newNode] = currentRank;
                        computedRankQuantity++;
                    }
                }
            }
        }
        currentRank++;
    }
    
    ///////////////////////
    ///////// Compute earliest dates
    ///////////////////////
    
    // initialisation
    earliestDates.resize(size);
    for (int i = 0; i < size; ++i) {
        latestDates[i] = -1;
    }
    
    // algorithm seen in class
    for (int k = 1; k < currentRank; ++k) {
        for (int j = 0; j < size; ++j) {
            if (ranks[j] == k) {
                int max = -1;
                for (int predecessor = 0; predecessor < size; ++predecessor) {
                    if (adjMatrix[predecessor][j].valid) {
                        int value = ranks[predecessor] + adjMatrix[predecessor][j].value;
                        if (value > max) {
                            max = value;
                        }
                    }
                }
                earliestDates[j] = max;
            }
        }
    }
    
    ///////////////////////
    ///////// Compute latest dates
    ///////////////////////
    
    // initialisation
    latestDates.resize(size);
    for (int i = 0; i < size; ++i) {
        latestDates[i] = earliestDates[i];
    }
    
    // algorithm seen in class
    for (int k = currentRank - 1; k >= 0; --k) {
        for (int j = 0; j < size; ++j) {
            if (ranks[j] == k) {
                int min = -1;
                for (int successor = 0; successor < size; ++successor) {
                    if (adjMatrix[j][successor].valid) {
                        int value = latestDates[successor] - adjMatrix[j][successor].value;
                        if (value < min || min == -1) {
                            min = value;
                        }
                    }
                }
                latestDates[j] = min;
            }
        }
    }
}

void printCalendar(vector<int> &earliestDates, vector<int> &latestDates) {
    long size = earliestDates.size();
    cout << "\t\t\t" << "Nodes";
    for (int node = 0; node < size; ++node) {
        cout << "\t\t" << node;
    }
    cout << endl;
    
    cout << "Earliest dates";
    for (int node = 0; node < size; ++node) {
        cout << "\t\t" << earliestDates[node];
    }
    cout << endl;
    
    cout << "latestDates dates";
    for (int node = 0; node < size; ++node) {
        cout << "\t\t" << latestDates[node];
    }
    cout << endl;
}

bool hasPredecessor(int node, const AdjMatrix &adjMatrix) {
    int i = 0;
    while (i < adjMatrix.size()) {
        if (adjMatrix[i][node].valid) {
            return true;
        }
        i++;
    }
    return false;
}

bool hasSuccessor(int node, const AdjMatrix &adjMatrix) {
    int i = 0;
    while (i < adjMatrix.size()) {
        if (adjMatrix[node][i].valid) {
            return true;
        }
        i++;
    }
    return false;
}

void parseFile(string path, AdjMatrix &adjMatrix) {
    ifstream file(path.c_str()); // open the file
    
    string line = ""; // buffer that will contains every line once at the time
    
    int nbTasks = -1;       // number of tasks found on first line
    int nbTasksParsed = 0;  // number of tasks parsed to make a verification
    
    vector<string> splittedLine;   // default value to prevent cross OS errors ? ###############################
    
    // if the file successfully opened
    if(file.is_open()){
        // while we get a new line we put it in the line string
        while(getline(file, line)){
            cout << endl << "lenght(" <<  (split(line)).size() << ") : " << line << endl;
            //  first line : we get the number of tasks and put it in a variables
            //  if negative or null, return because of error
            if(nbTasks == -1){
                // we cast the line to int
                nbTasks = parseInt(line);
                if(nbTasks < 1){
                    cout << "error : bad number of tasks !" << endl;
                    return;
                }
                cout << "number of tasks to be parsed : " << nbTasks << endl;
                
                // resize the adgency matrix and set all value to not valid
                adjMatrix.resize(nbTasks + 2); // we considere the initial and final task
                for (int i = 0; i < adjMatrix.size(); ++i){
                    adjMatrix[i].resize(nbTasks + 2);
                    for (int j = 0; j < adjMatrix[j].size(); ++j){
                        adjMatrix[i][j].valid = false;
                    }
                }
                
            } else { // not the first turn
                // we check for the eof
                if(line == "-1"){
                    cout << "end of file detected" << endl;
                    // process the end of file :
                    // (1) check the number of parsed tasks
                    if(nbTasksParsed != nbTasks){
                        cout << "error : not right number of tasks : " << nbTasksParsed << " instead of " << nbTasks << " !" << endl;
                        return;
                    }
                    
                    // (2) what to do next ? ##############################
                    
                    cout << "right number of tasks parsed !" << endl;
                }
                else{ // if not end of file
                    cout << "new line to be splitted : " << line << endl;
                    // we split the line into a vector to get each
                    splittedLine = split(line);
                    cout << splittedLine.size() << " symbols splitten" << endl;
                    
                    // check if the line is not empty
                    if(splittedLine.size() == 0){
                        cout << "error : line empty !" << endl;
                        return;
                    }
                    
                    // check if the line contains at least 3 elements : task number length and -1
                    if(splittedLine.size() < 3){
                        cout << "error : line too short !" << endl;
                        return;
                    }
                    
                    // verify that the last one is "-1"
                    if(splittedLine[splittedLine.size() -1] != "-1"){
                        cout << "error : missing -1 at the end of the line" << endl;
                        return;
                    }
                    
                    // someting else to check directly ? ############################
                    
                    vector<int> constraints;
                    for (int i = 2; i < splittedLine.size() - 1 ; ++i) {
                        constraints.push_back(parseInt(splittedLine[i]));
                    }
                    cout <<  constraints.size() << " constraints parsed !"<< endl;
                    
                    // set the line to the length
                    for (int i = 0; i < adjMatrix.size(); ++i) {
                        adjMatrix[parseInt(splittedLine[0])][i].value = parseInt(splittedLine[1]);
                    }
                    
                    // if there is no constraint we set the edge to be the successor of the initial edge
                    if(constraints.size() == 0){
                        cout << "task n°" << parseInt(splittedLine[0]) << " of lenght " << parseInt(splittedLine[1]) << " without constraint "<< endl;
                        adjMatrix[0][parseInt(splittedLine[0])].valid = true;
                    }else{
                        // add the constraint to the adjency matrix
                        for (int i = 0; i < constraints.size(); ++i){
                            cout << "task n°" << parseInt(splittedLine[0]) << " of lenght " << parseInt(splittedLine[1]) << " with constraint " << constraints[i] << " added !" << endl;
                            adjMatrix[constraints[i]][parseInt(splittedLine[0])].valid = true;
                        }
                    }
                    nbTasksParsed ++;
                }
            }
        }
        // after we parsed the adjency matrix
        
        // we add all edge without successor to be predecessor of final edge
        for (int i = 1; i < adjMatrix.size() - 1; ++i) {
            bool filled = false;
            for (int j = 1; j < adjMatrix.size() - 1; ++j){
                filled = adjMatrix[i][j].valid || filled;
            }
            
            //if not filled then the edge has no successor
            if(!filled){
                cout << "node " << i << " has no sucessor : we add it to the predecessors of the final node" << endl;
                adjMatrix[i][adjMatrix.size() - 1].valid = true;
            }
        }
        
        // we add all edge without predecessors to be successor of initial edge
        for (int i = 1; i < adjMatrix.size() - 1; ++i) {
            bool filled = false;
            for (int j = 1; j < adjMatrix.size() - 1; ++j){
                filled = adjMatrix[j][i].valid || filled;
            }
            
            //if not filled then the edge has no successor
            if(!filled){
                cout << "node " << i << " has no predecessor : we add it to the successors of the initial node" << endl;
                adjMatrix[0][i].valid = true;
            }
        }
        
        // print the matrix created
        cout << "Matrix created : " << endl;
        print(adjMatrix);
        
        // we close the file
        file.close();
    }
    
}



vector<string> split(string const &input) {
    istringstream buffer(input);
    vector<string> ret((istream_iterator<string>(buffer)),istream_iterator<string>());
    return ret;
}

int parseInt(string s){
    istringstream buffer(s);
    int r = 0;
    buffer >> r;
    return r;
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
