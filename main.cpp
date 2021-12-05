#include <string>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <cmath>
#include <vector>

using namespace std;
// Declaring functions
string convertToBinary(const string& hex);
void writeResults(const string& resultName, const int& replaceType, const vector<double>& ratio, const vector<int>& numBlocks, const vector<int>& bytesPerBlock, const vector<int>& numSets);
void test(const int& replaceType, const string& fileName, const int& cacheType);
void readTraceFile(const string& fileName, int& hits, int& misses, int& bytesPerBlock, int& numSets);
void readTraceFile(const int& replaceType, const string& fileName, int& hits, int& misses, const int& numBlocks, const int& bytesPerBlock);
void readTraceFile(const int& replaceType, const string& fileName, int& hits, int& misses, const int& numBlocks, const int& bytesPerBlock, const int& numSets);

// Defining Functions
string convertToBinary(const string& hex) {
    string binary;
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
    string leading0s;
    while (binary.size() % 4 != 0) {
        leading0s = "0" + leading0s;
        binary = leading0s + binary;
    }
    return binary;
}

void writeResults(const string& resultName, const int& replaceType, const vector<double>& ratio, const vector<int>& numBlocks, const vector<int>& bytesPerBlock, const vector<int>& numSets) {
    ofstream outFile(resultName + ".csv");
    // Writing Data
    for (int i = 0; i < ratio.size(); ++i) {
        outFile << replaceType << "," << numSets[i] << "," << numBlocks[i] << "," << bytesPerBlock[i] << "," << ratio[i] << endl;
    }
}
void test(const int& replaceType, const string& fileName, const int& cacheType) {
    vector<int> numBlocksArray;
    vector<int> numSetsArray;
    vector<int> bytesPerBlockArray;
    vector<double> ratioArray;
    string resultName;
    if (replaceType == 1) {
        resultName = "FIFO_";
    }
    else {
        resultName = "LRU_";
    }
    int hits = 0; int misses = 0;
    if (cacheType == 1) {
        // fully-associative
        int numSets = 1;
        int counter = 0;
        for (int bytesPower = 2; bytesPower <= 4; bytesPower++) {
            int bytesPerBlock = pow(2, bytesPower);
            // loop for numBlocks
            for (int nBlocks = 1; nBlocks <= 4; nBlocks += 2) {
                bytesPerBlockArray.push_back(bytesPerBlock);
                numSetsArray.push_back(numSets);
                numBlocksArray.push_back(nBlocks);
                readTraceFile(replaceType, fileName, hits, misses, nBlocks, bytesPerBlock);
                double result = (double) hits / (double) misses;
                ratioArray.push_back(result);
                cout << counter++ << endl;
            }
        }
        writeResults(resultName + "resultFA", replaceType, ratioArray, numBlocksArray, bytesPerBlockArray, numSetsArray);
    }
    else if (cacheType == 2) {
        // direct-mapped
        int nBlocks = 1;
        int counter = 0;
        for (int bytesPower = 2; bytesPower <= 4; bytesPower++) {
            int bytesPerBlock = pow(2, bytesPower);
            // loop for numBlocks
            for (int numSets = 1; numSets <= 2; numSets++) {
                bytesPerBlockArray.push_back(bytesPerBlock);
                numSetsArray.push_back(numSets);
                numBlocksArray.push_back(nBlocks);
                readTraceFile(fileName, hits, misses, bytesPerBlock, numSets);
                double result = (double) hits / (double) misses;
                ratioArray.push_back(result);
                cout << counter++ << endl;
            }
        }
        writeResults("resultDM", replaceType, ratioArray, numBlocksArray, bytesPerBlockArray, numSetsArray);
    }
    else if (cacheType == 3) {
        // set-associative
        int counter = 0;
        for (int bytesPower = 2; bytesPower <= 4; bytesPower++) {
            int bytesPerBlock = pow(2, bytesPower);
            for (int numSets = 1; numSets <= 2; numSets++) {
                for (int nBlocks = 1; nBlocks <= 4; nBlocks += 2) {
                    bytesPerBlockArray.push_back(bytesPerBlock);
                    numSetsArray.push_back(numSets);
                    numBlocksArray.push_back(nBlocks);
                    readTraceFile(1, fileName, hits, misses, nBlocks, bytesPerBlock, numSets);
                    double result = (double) hits / (double) misses;
                    ratioArray.push_back(result);
                    cout << counter++ << endl;
                }
                writeResults(resultName + "resultSA", replaceType, ratioArray, numBlocksArray, bytesPerBlockArray, numSetsArray);
            }
        }
    }
    else {
        throw runtime_error("Invalid Cache Type");
    }
}

// read function for direct-mapped
void readTraceFile(const string& fileName, int& hits, int& misses, int& bytesPerBlock, int& numSets) {
    // cache types: 2 (direct-mapped)
    ifstream inFile(fileName);
    map<string, string> tagMap;
    string addressLine;
    string addressBinary;
    while(getline(inFile, addressLine)) {
        istringstream stream(addressLine);
        string addressHex;
        string ignoreThis;
        getline(stream, ignoreThis, ' ');
        // ignores the first string obtained by the first get line since we don't care about that value from the trace files
        getline(stream, addressHex, ' ');
        // converting the addressHex to Binary
        addressBinary = convertToBinary(addressHex);
        // for direct map, tag is address - offset (bytesPerBlock) - log2(numSets);
        int lineOffset = log2(numSets);
        int tagOffset = addressBinary.size() - log2(bytesPerBlock) - lineOffset;
        // grabbing the lineBits
        string lineBits;
        for (int i = tagOffset; i < addressBinary.size() - log2(bytesPerBlock); ++i) {
            lineBits += addressBinary[i];
        }
        string currentTag;
        for (int i = 0; i < tagOffset; ++i) {
            currentTag += addressBinary[i];
        }
        if (tagMap.find(lineBits) != tagMap.end()) {
            // if the tagMap does already have a value at this line-bit
            // check if the tag at this line bit matches with the currentTag, if so then it's a hit
            if (tagMap.find(lineBits)->second == currentTag) {
                ++hits;
            }
            // else it's a miss and replace the old tag with the new tag;
            else {
                ++misses;
                tagMap.find(lineBits)->second = currentTag;
            }
        }
        else {
            // else this line bit is empty, so it's a miss and add in a new tag at this line bit
            ++misses;
            tagMap.insert(make_pair(lineBits, currentTag));
        }
    }
}

// read function for fully-associative
void readTraceFile(const int& replaceType, const string& fileName, int& hits, int& misses, const int& numBlocks, const int& bytesPerBlock) {
    // cache types: 1 (fully associative)
    ifstream inFile(fileName);
    string addressLine;
    string addressBinary;
    list<string> tagsList;
    set<string> tags;
    while (getline(inFile, addressLine)) {
        istringstream stream(addressLine);
        string addressHex;
        string ignoreThis;
        getline(stream, ignoreThis, ' ');
        // ignores the first string obtained by the first get line since we don't care about that value from the trace files
        getline(stream, addressHex, ' ');
        // converting the addressHex to Binary
        addressBinary = convertToBinary(addressHex);
        // for fully-associative, tag = address - offset(log2(2^bytesPerBlock) = address - log2(bytesPerBlock)
        int tagIndex = addressBinary.size() - log2(bytesPerBlock); string currentTag;
        for (int i = 0; i < tagIndex; ++i) {
            currentTag += addressBinary[i];
        }
        // Checking whether the tag already exists in the set, if so then it's a hit
        if (tags.find(currentTag) != tags.end()) {
            ++hits;
            if (replaceType == 2) {
                // go through the list and move the one that hits to the back of the list
                auto iter = tagsList.begin();
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

// read function for set-associative
void readTraceFile(const int& replaceType, const string& fileName, int& hits, int& misses, const int& numBlocks, const int& bytesPerBlock, const int& numSets) {
    // cache types: 3 (set-associative)
    ifstream inFile(fileName);
    string addressLine;
    string addressBinary;
    map<string, vector<string>> tagMap;
    map<string, list<string>> tagsList;
    while (getline(inFile, addressLine)) {
        istringstream stream(addressLine);
        string addressHex;
        string ignoreThis;
        getline(stream, ignoreThis, ' ');
        // ignores the first string obtained by the first get line since we don't care about that value from the trace files
        getline(stream, addressHex, ' ');
        // converting the addressHex to Binary
        addressBinary = convertToBinary(addressHex);
        // for set-associative tag = address - offset(log2(bytesPerBlock)) - setField(number of Sets)
        // getting the setField
        int setIndex = log2(numSets);
        // getting the set
        string currentSet;
        for (int i = addressBinary.size() - log2(bytesPerBlock) - setIndex; i < addressBinary.size() - log2(bytesPerBlock); ++i) {
            currentSet += addressBinary[i];
        }
        // grabbing the tag
        string currentTag;
        for (int i = 0; i < addressBinary.size() - log2(bytesPerBlock) - setIndex; ++i) {
            currentTag += addressBinary[i];
        }
        // Checking whether the cache is full
        if (tagMap.find(currentSet) != tagMap.end()) {
            list<string>& tList = tagsList.find(currentSet)->second;
            vector<string>& tags = tagMap.find(currentSet)->second;
            bool hit = false;
            // iterate through the vector at the set to see if the tag matches any of the ones in the set to hit
            for (int i = 0; i < tags.size(); ++i) {
                if (tags[i] == currentTag) {
                    hit = true;
                }
            }
            if (hit) {
                ++hits;
                if (replaceType == 2) {
                    // go through the list and move the one that hits to the back of the list
                    auto iter = tList.begin();
                    while (iter != tList.end()) {
                        if (*iter == currentTag) {
                            string temp = *iter;
                            tList.erase(iter);
                            tList.push_back(temp);
                            break;
                        }
                        ++iter;
                    }
                }
            }
            // else if the set is in the map, check whether the vector of strings is full
            else if (tags.size() >= numBlocks) {
                // if full, then replace using desired FIFO or LRU
                ++misses;
                auto removalIndex = tags.begin();
                for (int i = 0; i < tags.size(); ++i) {
                    if (tags[i] == tList.front()) {
                        removalIndex += i;
                    }
                }
                tags.erase(removalIndex);
                tList.pop_front();
                tags.push_back(currentTag);
                tList.push_back(currentTag);
            }
            // else there is space in the vector, so add the tag
            else {
                ++misses;
                tList.push_back(currentTag);
                tags.push_back(currentTag);
            }
        }
        // else add the new set and tag
        else {
            ++misses;
            vector<string> temp;
            temp.push_back(currentTag);
            tagMap.insert(make_pair(currentSet, temp));
            list<string> tempList;
            tempList.push_back(currentTag);
            tagsList.insert((make_pair(currentSet, tempList)));
            cout << "";
        }
    }
}

int main() {
    // Initiating the parameters for the cache simulation
    string fileName = "traces/gcc.trace";
    // cache types: 1 (fully associative); 2 (direct-mapped); 3 (set-associative)
    int hits = 0; int misses = 0; int cacheType; int numBlock; int bytesPerBlock; int numSets; int replaceType = 1;
    // Getting file name and cache type from the user
    /*cout << "Enter the file name: ";
    cin >> fileName;
    fileName = "traces/" + fileName;
    cout << "Enter the cache type: ";
    cin >> cacheType;
    cout << "Enter desired bytes per block: ";
    cin >> bytesPerBlock;
    // cache types: 1 (fully associative); 2 (direct-mapped); 3 (set-associative)
    if (cacheType == 1) {
        numSets = 1;
        cout << "Enter desired number of blocks: ";
        cin >> numBlock;
        cout << "Replace Type? 1 (FIFO) ; 2 (LRU): ";
        cin >> replaceType;
        readTraceFile(replaceType, fileName, hits, misses, numBlock, bytesPerBlock);
    }
    else if (cacheType == 2) {
        numBlock = 1;
        cout << "Enter desired number of sets: ";
        cin >> numSets;
        readTraceFile(fileName, hits, misses, bytesPerBlock, numSets);
    }
    else if (cacheType == 3) {
        cout << "Enter desired number of sets: ";
        cin >> numSets;
        cout << "Enter desired number of blocks: ";
        cin >> numBlock;
        cout << "Replace Type? 1 (FIFO) ; 2 (LRU): ";
        cin >> replaceType;
        readTraceFile(replaceType, fileName, hits, misses, numBlock, bytesPerBlock, numSets);
    }
    else {
        throw runtime_error("Invalid Cache Type!");
    }
    */
    // Hard-coding for data gathering
    test(1, fileName, 2);
    test(1, fileName, 1);
    test(1, fileName, 3);
    test(2, fileName, 1);
    test(2, fileName, 3);




    return 0;
}
