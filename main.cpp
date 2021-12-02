#include <string>
#include <map>
#include <set>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <list>

using namespace std;

string convertToBinary(const string hex) {
    string binary = "";
    for (int i = 2; i < hex.size(); ++i) {
        // loops through the hex string and add to binary string with corresponding 4 bit binary
        switch (hex[i]) {
            case '0':
                binary += "0000";
                break;
            case '1':
                binary += "0001";
                break;
            case '2':
                binary += "0010";
                break;
            case '3':
                binary += "0011";
                break;
            case '4':
                binary += "0100";
                break;
            case '5':
                binary += "0101";
                break;
            case '6':
                binary += "0110";
                break;
            case '7':
                binary += "0111";
                break;
            case '8':
                binary += "1000";
                break;
            case '9':
                binary += "1001";
                break;
            case 'a':
            case 'A':
                binary += "1010";
                break;
            case 'b':
            case 'B':
                binary += "1011";
                break;
            case 'c':
            case 'C':
                binary += "1100";
                break;
            case 'd':
            case 'D':
                binary += "1101";
                break;
            case 'e':
            case 'E':
                binary += "1110";
                break;
            case 'f':
            case 'F':
                binary += "1111";
                break;
            default:
                throw runtime_error("Invalid Hex Character!");
        }
    }
    // check that the binary is a multiple of 4, if not then add leading 0s until it is
    string leading0s = "";
    while (binary.size() % 4 != 0) {
        leading0s = "0" + leading0s;
        binary = leading0s + binary;
    }
    return binary;
}

// read function for direct-mapped
void readTraceFile(const int replaceType, const string fileName, map<string, string>& tagMap, int& hits, int& misses, int& bytesPerBlock, int& numSets) {
    // cache types: 1 (fully associative); 2 (direct-mapped); 3 (set-associative)
    ifstream inFile(fileName);
    string addressLine;
    string addressBinary;
    while(getline(inFile, addressLine)) {
        istringstream stream(addressLine);
        string addressHex;
        string ignore;
        getline(stream, ignore, ' ');
        // ignores the first string obtained by the first get line since we don't care about that value from the trace files
        getline(stream, addressHex, ' ');
        // converting the addressHex to Binary
        addressBinary = convertToBinary(addressHex);

    }
}
// read function for fully-associative
// TODO: Ask if my logic behind the LRU for fully-associative is correct
void readTraceFile(const int replaceType, const string& fileName, set<string> tags, int& hits, int& misses, const int& numBlocks, const int& bytesPerBlock) {
    // cache types: 1 (fully associative); 2 (direct-mapped); 3 (set-associative)
    ifstream inFile(fileName);
    string addressLine;
    string addressBinary;
    list<string> tagsList;
    while (getline(inFile, addressLine)) {
        istringstream stream(addressLine);
        string addressHex;
        string ignore;
        getline(stream, ignore, ' ');
        // ignores the first string obtained by the first get line since we don't care about that value from the trace files
        getline(stream, addressHex, ' ');
        // converting the addressHex to Binary
        addressBinary = convertToBinary(addressHex);
        // for fully-associative, tag = address - offset(log2(2^bytesPerBlock) = address - bytesPerBlock
        int tagIndex = addressBinary.size() - bytesPerBlock; string currentTag;
        for (int i = 0; i < tagIndex; ++i) {
            currentTag += addressBinary[i];
        }
        // Checking whether the tag already exists in the set, if so then it's a hit
        if (tags.find(currentTag) != tags.end()) {
            ++hits;
            if (replaceType == 2) {
                // go through the list and move the one that hits to the back of the list
                list<string>::iterator iter = tagsList.begin();
                while (iter != tagsList.end()) {
                    if (*iter == currentTag) {
                        string temp = *iter;
                        tagsList.erase(iter);
                        tagsList.push_back(temp);
                        break;
                    }
                    ++iter;
                }
            }
        }
        // else check whether the set is full
        else if (tags.size() >= numBlocks) {
            ++misses;
            tags.erase(tags.find(tagsList.front()));
            tagsList.pop_front();
            tags.emplace(currentTag);
            tagsList.push_back(currentTag);
        }
        // else tag is not known and set has space so add the new tag
        else {
            ++misses;
            tags.emplace(currentTag);
            tagsList.push_back(currentTag);
        }
    }
}
int main() {
    // Initiating the parameters for the cache simulation
    string fileName = "gcc.trace"; set<string> fullTags; map<string, string> tags;
    // cache types: 1 (fully associative); 2 (direct-mapped); 3 (set-associative)
    // TODO: Change these hardcoded values back to allowing the user to choose
    int hits = 0; int misses = 0; int cacheType = 1; int numBlock = 3; int bytesPerBlock = 4; int numSets = 1; int replaceType = 1;
    /*// Getting file name and cache type from the user
    cout << "Enter the file name: ";
    cin >> fileName;
    cout << "Enter the cache type: ";
    cin >> cacheType;
    */
    // cache types: 1 (fully associative); 2 (direct-mapped); 3 (set-associative)
    readTraceFile(2, "traces/" + fileName, fullTags, hits, misses, numBlock, bytesPerBlock);
    cout << "Hits: " << hits << endl;
    cout << "Misses: " << misses << endl;
    cout << "Hit/Miss Ratio: " << (double) hits / (double) misses << endl;

    return 0;
}
