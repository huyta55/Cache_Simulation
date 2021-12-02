#include <string>
using namespace std;

class Tag {
private:
    string tag;
    int timesUsed;
    int age;
public:
    Tag() {
        tag = "";
        timesUsed = 0;
        age = 0;
    }
    Tag(string tagBinary) {
       tag = tagBinary;
       timesUsed = 0;
       age = 0;
    }
    void increaseUse() {
        ++timesUsed;
    }
    void resetAge() {
        age = 0;
    }
    void increaseAge() {
        ++age;
    }
    string getTag() {
        return tag;
    }

};
bool operator==(Tag& tag1, Tag& otherTag) {
    // if size is not the same, then it's not the same tag
    if (tag1.getTag().size() != otherTag.getTag().size()) {
        return false;
    }
    // check every character, if any are not the same then false
    for (int i = 0; i < tag1.getTag().size(); ++i) {
        if (tag1.getTag()[i] != otherTag.getTag()[i]) {
            return false;
        }
    }
    return true;
}

