#ifndef HELPER_HPP
#define HELPER_HPP
/* Niema Moshiri 2017
 *
 * Helper functions
 */
#include <iostream>
#include <vector>
using namespace std;
using DECIMAL = long double;

// output Newick tree
void newick( const int & x, const vector<int> & left, const vector<int> & right, const vector<int> & parent, const vector<DECIMAL> & time ) {
    // if leaf
    if(left[x] == -1 && right[x] == -1) {
        cout << 'L' << x;
    }

    // if internal node
    else {
        int x_left = left[x]; int x_right = right[x]; DECIMAL x_time = time[x];
        DECIMAL left_length = time[x_left] - x_time;
        DECIMAL right_length = time[x_right] - x_time;
        cout << '(';
        newick(x_left,left,right,parent,time);
        cout << ':' << left_length << ',';
        newick(x_right,left,right,parent,time);
        cout << ':' << right_length << ')';
        cout << 'I' << x;
    }

    // add semicolon if root
    if(parent[x] == -1) {
        cout << ';' << endl;
    }
}
#endif // HELPER_HPP
