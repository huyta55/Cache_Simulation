#include <string>
#include <map>
#include <set>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

void readTraceFile(const int cacheType, const string fileName, map<string, string>& address, int& hits, int& misses) {
    // cache types: 1 (fully associative); 2 (direct-mapped); 3 (set-associative)
    ifstream inFile(fileName);
    if (inFile.is_open()) {
        cout << "Opened Successfully";
    }
}
int main() {
    // Initiating the parameters for the cache simulation
    map<string, string> address;
    int hits = 0; int misses = 0;
    // cache types: 1 (fully associative); 2 (direct-mapped); 3 (set-associative)
    readTraceFile(1, "traces/gcc.trace", address, hits, misses);
    return 0;
}
