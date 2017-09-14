/* Niema Moshiri 2017
 *
 * Simulate trees under the Dual-Birth model in O(n)
 *
 * USAGE: ./dualbirth [-n end_taxa] [-t end_time]
 */

// include statements
#include "helper.hpp"
#include "set.hpp"
#include <iostream>
#include <random>
#include <unordered_map>
#include <vector>
using namespace std;

// definitions
#define DEFAULT_R 1
#define USAGE "USAGE: ./yule lambda [-n end_taxa] [-t end_time] [-r num_replicates]"
#define BAD_RATE "ERROR: Rate must be positive"
#define BAD_N "ERROR: End number of taxa must be positive"
#define BAD_T "ERROR: End time must be positive"
#define BAD_R "ERROR: Number of replicates must be positive"
#define BAD_END "ERROR: Must specify either end_taxa or end_time (or both)"

// Yule simulator
void yule( double L, int N, DECIMAL T ) {
    // global variables
    DECIMAL t = 0;        // current global time
    vector<int> left;     // left[i] is the left child of node i
    vector<int> right;    // right[i] is the right child of node i
    vector<int> parent;   // parent[i] is the parent of node i
    vector<DECIMAL> time; // time[i] is the time of node i
    set leaves;           // set containing all leaves

    // prepare for simulations
    left.push_back(-1);   // root (i = 0) has no left child yet
    right.push_back(-1);  // root (i = 0) has no right child yet
    parent.push_back(-1); // root (i = 0) has no parent
    time.push_back(0);    // root (i = 0) starts with a time of 0
    leaves.add(0);

    // perform simulations
    while(leaves.size() < N) {
        // sample delta time until next splitting event
        exponential_distribution<double> exp_rv(leaves.size()*L);
        t += exp_rv(GEN); // sample next splitting time
        if(t >= T) {
            break;
        }

        // choose the next branch to split
        int next = leaves.random();
        leaves.remove(next);
        time[next] = t;

        // perform splitting
        int new_left = time.size();
        left[next] = new_left;
        leaves.add(new_left);
        parent.push_back(next);
        left.push_back(-1);
        right.push_back(-1);
        time.push_back(t);
        int new_right = time.size();
        right[next] = new_right;
        leaves.add(new_right);
        parent.push_back(next);
        left.push_back(-1);
        right.push_back(-1);
        time.push_back(t);
    }

    // if reached n leaves, sample one more time to end at time JUST BEFORE n+1 leaves
    if(leaves.size() == N) {
        exponential_distribution<double> exp_rv(leaves.size()*L);
        t += exp_rv(GEN); // sample next splitting time
    }

    // update all leaves to be proper time
    while(leaves.size() != 0) {
        int leaf = leaves.random();
        leaves.remove(leaf);
        time[leaf] = t;
    }

    // output tree in Newick format
    newick(0,left,right,parent,time);
}

// main function
int main( int argc, char* argv[] ) {
    if(argc != 4 && argc != 6 && argc != 8) {
        cout << USAGE << endl; exit(1);
    }
    bool END = false;
    int     N = numeric_limits<int>::max();     // end number of leaves
    DECIMAL T = numeric_limits<DECIMAL>::max(); // end time
    long    R = DEFAULT_R;                      // number of replicates

    // parse rates
    double L = strtod(argv[1],NULL); // Yule rate

    // parse first argument
    if(strcmp(argv[2],"-n") == 0) {
        N = strtol(argv[3],NULL,10); END = true;
    }
    else if(strcmp(argv[2],"-t") == 0) {
        T = strtold(argv[3],NULL); END = true;
    }
    else if(strcmp(argv[2],"-r") == 0) {
        R = strtol(argv[3],NULL,10);
    }

    // parse second argument
    if(argc > 4) {
        if(strcmp(argv[4],"-n") == 0) {
            N = strtol(argv[5],NULL,10); END = true;
        }
        else if(strcmp(argv[4],"-t") == 0) {
            T = strtold(argv[5],NULL); END = true;
        }
        else if(strcmp(argv[4],"-r") == 0) {
            R = strtol(argv[5],NULL,10);
        }
        else {
            cerr << USAGE << endl; exit(1);
        }
    }

    // parse third argument
    if(argc > 6) {
        if(strcmp(argv[6],"-n") == 0) {
            N = strtol(argv[7],NULL,10); END = true;
        }
        else if(strcmp(argv[6],"-t") == 0) {
            T = strtold(argv[7],NULL); END = true;
        }
        else if(strcmp(argv[6],"-r") == 0) {
            R = strtol(argv[7],NULL,10);
        }
        else {
            cerr << USAGE << endl; exit(1);
        }
    }

    // check arguments
    if(L <= 0) {
        cerr << BAD_RATE << endl << USAGE << endl; exit(1);
    }
    if(N <= 0) {
        cerr << BAD_N << endl << USAGE << endl; exit(1);
    }
    if(T <= 0) {
        cerr << BAD_T << endl << USAGE << endl; exit(1);
    }
    if(R <= 0) {
        cerr << BAD_R << endl << USAGE << endl; exit(1);
    }
    if(!END) {
        cerr << BAD_END << endl << USAGE << endl; exit(1);
    }

    // simulate trees
    for(long i = 0; i < R; ++i) {
        yule(L,N,T);
    }
}
