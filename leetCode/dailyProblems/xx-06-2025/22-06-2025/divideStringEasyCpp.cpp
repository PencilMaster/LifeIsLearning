// Since I had malloc issues initialising a 2D vector in C, I created this little beauty in C++ in a couple of minutes.
//

#include <string>
#include <vector>

class Solution {

public:
    vector<string> divideString(string s, int k, char fill) {
        std::vector<string> resultVector;
        for (size_t i = 0; i < s.length(); i += k) {
            std::string tempString(s, i, k);
            resultVector.push_back(tempString);
        }

        while ((resultVector.end()-1)->length() < k) {
            (resultVector.end()-1)->push_back(fill);
        }
        return resultVector;
    }
};
