// This is the finished task.
// Intuition

// Holy Powers of two.
// Imagine the operation string [1,0,1,1]:
// (no op) 2^0 = a
// (op[0]) 2^1 = ab
// (op[1]) 2^2 = abab
// (op[2]) 2^3 = ababbcbc
// (op[3]) 2^4 = ababbcbcbcbccdcd
// Realise, that the first 2^i-1 characters never change after their first change.
// Approach
// 
// We want to find out how many operations were used on the character at position [k-1]. Since it only changes, when op[x] == 1, we initialise a counter "itCount" that will count all applied operations on that character.
// 
//     Indexing is from 0, so we copy k-1 into the variable "rest".
//     We find the highest power of 2^x that is smaller equal than "rest".
//     We do itCount += op[x] since this was the last operation that changed the character at position "rest".
//     We subtract 2^x from "rest". This gives us its position in the previous part of the string before the last operation (op[x]).
//     We simply do this until we reach the first character at position 0 ("rest" == 0).
//

char kthCharacter(long long k, int* const operations, const int operationsSize) {
    // assert(operationsSize > 0 && "OperationsSize needs to be positive");

    // Greedy
    // Build vector modulo 26 for values (do if (== 'z') -> = 'a')
    
    long long rest = k-1;
    int itCount = 0;
    while (rest > 0) {
        long long kLog2Part = 1;
        int potCount = 0;
            while (kLog2Part * 2 <= rest) {
                kLog2Part <<= 1; // The amount if Iterations need to be done to first one as well!
                ++potCount;
            }
        itCount += operations[potCount];
        rest -= kLog2Part;
    }
    itCount %= 26;

    return 'a' + itCount;
}
