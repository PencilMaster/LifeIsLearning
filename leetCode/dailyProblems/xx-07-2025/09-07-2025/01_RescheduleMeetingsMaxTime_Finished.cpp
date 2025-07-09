// Implemented this answer in cpp as a refresher of the language.
// Sadly did not need to use lambdas or more complex features.
//

#include <cassert>
#include <algorithm>
#include <vector>

class Solution {
public:
    int maxFreeTime(int eventTime, int k, vector<int>& startTime, vector<int>& endTime) {
        // measure all distances, store them in a vector
        // return sum of k+1 biggest entries

        size_t startSize = startTime.size(), endSize = endTime.size();
        assert(startSize == endSize && "startTime and endTime are not the same length");

        // efficient when using insertion sort during comparison, but we'll use cpp functions for now.
        std::vector<int> result;
        result.reserve(startSize+1);

        result.push_back(startTime[0]);
        for (size_t it = 0; it < startSize-1; ++it) {
            result.push_back(startTime[it+1] - endTime[it]);
        }
        result.push_back(eventTime - endTime[endSize-1]);

        // Now sliding window result[i] + ... + result[i+k]
        // Each conditional statement in a loop adds a lot of time, since cpu lookahead makes mistakes. Minimise those.

        int resultSum = 0;
        for (size_t resIt = 0; resIt <= startSize && resIt <= k; ++resIt) {
            resultSum += result[resIt];
        }

        int maxWindow = resultSum;
        for (size_t kIt = k+1; kIt <= startSize; ++kIt) {
            resultSum += (result[kIt] - result[kIt-k-1]);
            maxWindow = ((maxWindow > resultSum) ? maxWindow : resultSum);
        }
        return maxWindow;
    }
};
