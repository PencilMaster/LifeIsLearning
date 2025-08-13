// Here we check sizes of different types to understand sockaddr structs
//
// We see that sockaddr and sockaddr_in both use 16 bytes of storage.

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
    
    printf("char: %d \n unsigned short: %d \n short int: %d \n int: %lu \n unsigned int: %lu", sizeof(char), sizeof(unsigned short), sizeof(short int), sizeof(int), sizeof(unsigned int));
    
    return 0;
}
