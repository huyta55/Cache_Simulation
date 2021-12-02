#include <string>
#include <map>
#include <set>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

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
string getTag(const int cacheType, string addressBinary) {
    switch (cacheType) {
        case 1:
            // fully-associative

            break;
        case 2:
            // direct-mapped

            break;
        case 3:
            // set-associative
            break;
        default:
            throw runtime_error("invalid cache type");
    }
}

void readTraceFile(const int cacheType, const string fileName, map<string, string>& address, int& hits, int& misses) {
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
        // Depending on the different cache type that the user selects, the way the tag is separated from the address is different

    }


}
int main() {
    // Initiating the parameters for the cache simulation
    map<string, string> address; string fileName = "read01.trace";
    // cache types: 1 (fully associative); 2 (direct-mapped); 3 (set-associative)
    int hits = 0; int misses = 0; int cacheType = 1; int numBlock; int bytesPerBlock;
    /*// Getting file name and cache type from the user
    cout << "Enter the file name: ";
    cin >> fileName;
    cout << "Enter the cache type: ";
    cin >> cacheType;
    */
    // cache types: 1 (fully associative); 2 (direct-mapped); 3 (set-associative)
    readTraceFile(cacheType, "traces/" + fileName, address, hits, misses);
    return 0;
}
