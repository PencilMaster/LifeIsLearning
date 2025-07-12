// Here I tried to be fancy first and shot myself in the foot with all the arrays I create instead of just writing out
// the vector logic of meetings and gaps without storing every gap specificly.
//
// Will change the approach
//


#include <cassert>
#include <algorithm>
#include <vector>

class Solution {
public:
    int maxFreeTime(int eventTime, vector<int>& startTime, vector<int>& endTime) {
        // measure all distances, store them in a vector
        // return sum of k+1 biggest entries
        
        size_t startSize = startTime.size(), endSize = endTime.size();
        assert(startSize == endSize && "startTime and endTime are not the same length");
        const int k = 1;

        if (startSize == 1) return eventTime - endTime[0] + startTime[0];

        // efficient when using insertion sort during comparison, but we'll use cpp functions for now.
        std::vector<int> gaps;
        gaps.reserve(startSize+1);
        std::vector<int> fullVector;
        fullVector.reserve(2*startSize + 1);

        fullVector.push_back(startTime[0]);
        gaps.push_back(startTime[0]);
        for (size_t front = 0, back = 0; front < startSize-1; ++front, ++back) {
            #pragma omp simd
            fullVector.push_back(endTime[front] - startTime[back]);
            fullVector.push_back(startTime[front+1] - endTime[back]);
            gaps.push_back(startTime[front+1] - endTime[back]);
        }
        fullVector.push_back(endTime[endSize-1] - startTime[startSize-1]);
        fullVector.push_back(eventTime - endTime[endSize-1]);
        gaps.push_back(eventTime - endTime[endSize-1]);

        std::vector<int> maxGaps = gaps;

        sort(maxGaps.begin(), maxGaps.end(), std::greater<int>()); // sort since we only need the 3 biggest ones
        maxGaps.resize(3);

        int windowSum = fullVector[0] + fullVector[1] + fullVector[2];
        std::vector<int> windowVector(startSize, 0);
        windowVector[0] = windowSum;
        for (size_t it = 2; it + 2 < 2*startSize + 1; it += 2) {
            windowSum -= (fullVector[it-2] + fullVector[it-1]);
            windowSum += fullVector[it+1] + fullVector[it+2];
            windowVector[it/2] = windowSum;
        }

        //sort(windowVector.begin(), windowVector.end(), std::greater<int>());

        for (size_t it = 0; it < startSize; ++it) {
            if (fullVector[2*it + 1] >= maxGaps[2]) return windowVector[it];
            else if (fullVector[2*it + 1] >= maxGaps[1] || maxGaps[1] != gaps[it] || maxGaps[1] != gaps[it+1]) return windowVector[it];
            else if (fullVector[2*it + 1] >= maxGaps[0] || maxGaps[0] != gaps[it] || maxGaps[0] != gaps[it+1]) return windowVector[it];
        }
        if (fullVector[2*startSize] >= maxGaps[2]) return windowVector.back();
        else if (fullVector[2*startSize] >= maxGaps[1] || maxGaps[1] != gaps[startSize] || maxGaps[1] != gaps[startSize-1]) return windowVector.back();
        else if (fullVector[2*startSize] >= maxGaps[0] || maxGaps[0] != gaps[startSize] || maxGaps[0] != gaps[startSize-1]) return windowVector.back();


        // Now sliding window gaps[i] + ... + gaps[i+k]
        // Each conditional statement in a loop adds a lot of time, since cpu lookahead makes mistakes. Minimise those.

        
        int resultSum = gaps[0] + gaps[1];
        int maxWindow = gaps[0] + gaps[1];
        for (size_t kIt = 2; kIt <= startSize; ++kIt) {
            resultSum += (gaps[kIt] - gaps[kIt-2]);
            maxWindow = ((maxWindow > resultSum) ? maxWindow : resultSum);
        }
        
        return maxWindow;
    }
};
