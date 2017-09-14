#ifndef SET_HPP
#define SET_HPP
/* Niema Moshiri 2017
 *
 * Set class to add, remove, find, and pick random elements in O(1) time
 */
#include <iostream>
#include <random>
#include <unordered_map>
#include <vector>
using namespace std;

// global variables
random_device RDEV;
default_random_engine GEN(RDEV());

// set class with O(1) insert, remove, find, and randomly select
class set {
private:
    vector<int> arr;
    unordered_map<int,int> map;
public:
    // get size of set
    int size() {
        return arr.size();
    }

    // add integer x to set
    void add( const int & x ) {
        if(map.find(x) != map.end()) {
            return;
        }
        int index = arr.size();
        arr.push_back(x);
        map.insert(pair<int,int>(x,index));
    }

    // see if integer x exists in set
    bool find( const int & x ) {
        return map.find(x) != map.end();
    }

    // remove integer x from set
    void remove( const int & x ) {
        if(map.find(x) == map.end()) {
            return;
        }
        int index = map.at(x);
        map.erase(x);
        int last_index = arr.size()-1;
        int last = arr[last_index]; arr[last_index] = x; arr[index] = last;
        arr.pop_back();
        if(map.find(last) != map.end()) {
            map.at(last) = index;
        }
    }

    // return a random integer from set
    int random() {
        if(arr.size() == 0) {
            cerr << "ERROR: Calling random() on an empty set" << endl; exit(1);
        }
        uniform_int_distribution<int> dist(0,arr.size()-1);
        int index = dist(GEN);
        return arr[index];
    }
};
#endif // SET_HPP
