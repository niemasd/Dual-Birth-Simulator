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
#define USAGE "USAGE: ./dualbirth lambda_A lambda_B [-n end_taxa] [-t end_time] [-r num_replicates]"
#define BAD_RATE "ERROR: Rates must be positive"
#define BAD_N "ERROR: End number of taxa must be positive"
#define BAD_T "ERROR: End time must be positive"
#define BAD_R "ERROR: Number of replicates must be positive"
#define BAD_END "ERROR: Must specify either end_taxa or end_time (or both)"

// Dual-Birth simulator
void dualbirth( double LA, double LB, int N, DECIMAL T ) {
    // global variables
    DECIMAL t = 0;        // current global time
    vector<int> left;         // left[i] is the left child of node i
    vector<int> right;        // right[i] is the right child of node i
    vector<int> parent;       // parent[i] is the parent of node i
    vector<DECIMAL> time; // time[i] is the time of node i
    set active;               // set containing all active nodes
    set inactive;             // set containing all inactive nodes

    // prepare for simulations
    left.push_back(-1);   // root (i = 0) has no left child yet
    right.push_back(-1);  // root (i = 0) has no right child yet
    parent.push_back(-1); // root (i = 0) has no parent
    time.push_back(0);    // root (i = 0) starts with a time of 0
    active.add(0);

    // perform simulations
    while(active.size() + inactive.size() < N) {
        // sample delta time until next splitting event
        exponential_distribution<double> exp_rv(active.size()*LB + inactive.size()*LA);
        t += exp_rv(GEN); // sample next splitting time
        if(t >= T) {
            break;
        }

        // choose the next branch to split
        double p = (active.size()*LB) / (active.size()*LB + inactive.size()*LA); // probability splitting branch is active
        bernoulli_distribution coin(p);
        int next = -1;
        if(coin(GEN)) {
            next = active.random();
            active.remove(next);
        }
        else {
            next = inactive.random();
            inactive.remove(next);
        }
        time[next] = t;

        // perform splitting
        int new_left = time.size();
        left[next] = new_left;
        inactive.add(new_left);
        parent.push_back(next);
        left.push_back(-1);
        right.push_back(-1);
        time.push_back(t);
        int new_right = time.size();
        right[next] = new_right;
        active.add(new_right);
        parent.push_back(next);
        left.push_back(-1);
        right.push_back(-1);
        time.push_back(t);
    }

    // if reached n leaves, sample one more time to end at time JUST BEFORE n+1 leaves
    if(active.size() + inactive.size() == N) {
        exponential_distribution<double> exp_rv(active.size()*LB + inactive.size()*LA);
        t += exp_rv(GEN); // sample next splitting time
    }

    // update all leaves to be proper time
    while(active.size() != 0) {
        int leaf = active.random();
        active.remove(leaf);
        time[leaf] = t;
    }
    while(inactive.size() != 0) {
        int leaf = inactive.random();
        inactive.remove(leaf);
        time[leaf] = t;
    }

    // output tree in Newick format
    newick(0,left,right,parent,time);
}

// main function
int main( int argc, char* argv[] ) {
    if(argc != 5 && argc != 7 && argc != 9) {
        cout << USAGE << endl; exit(1);
    }
    bool END = false;
    int     N = numeric_limits<int>::max();     // end number of leaves
    DECIMAL T = numeric_limits<DECIMAL>::max(); // end time
    long    R = DEFAULT_R;                      // number of replicates

    // parse rates
    double LA = strtod(argv[1],NULL); // activation rate
    double LB = strtod(argv[2],NULL); // birth rate

    // parse first argument
    if(strcmp(argv[3],"-n") == 0) {
        N = strtol(argv[4],NULL,10); END = true;
    }
    else if(strcmp(argv[3],"-t") == 0) {
        T = strtold(argv[4],NULL); END = true;
    }
    else if(strcmp(argv[3],"-r") == 0) {
        R = strtol(argv[4],NULL,10);
    }

    // parse second argument
    if(argc > 5) {
        if(strcmp(argv[5],"-n") == 0) {
            N = strtol(argv[6],NULL,10); END = true;
        }
        else if(strcmp(argv[5],"-t") == 0) {
            T = strtold(argv[6],NULL); END = true;
        }
        else if(strcmp(argv[5],"-r") == 0) {
            R = strtol(argv[6],NULL,10);
        }
        else {
            cerr << USAGE << endl; exit(1);
        }
    }

    // parse third argument
    if(argc > 7) {
        if(strcmp(argv[7],"-n") == 0) {
            N = strtol(argv[8],NULL,10); END = true;
        }
        else if(strcmp(argv[7],"-t") == 0) {
            T = strtold(argv[8],NULL); END = true;
        }
        else if(strcmp(argv[7],"-r") == 0) {
            R = strtol(argv[8],NULL,10);
        }
        else {
            cerr << USAGE << endl; exit(1);
        }
    }

    // check arguments
    if(LA <= 0 || LB <= 0) {
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
        dualbirth(LA,LB,N,T);
    }
}
