// Here Quicksort still has a problem to be fixed. I will change the style.
//
// 1. Choose Median of three as pivot.
// 2. Move pivot to end
// 3. Iterate from the left with two pointers
// 4. Repeat on left and right side
//
//

void quickSort(int* arr, int arrSize) {
    if (arrSize <= 1) return;

    size_t left = 0, right = arrSize-2, pivot = arrSize-1;
    while (left <= right) {
        if (left == right) {
            if (left <= pivot) return;
            else {
                int temp = arr[pivot];
                arr[pivot] = arr[left];
                arr[left] = temp;
                return;
            }
        }
        while (left < right && arr[left] < arr[pivot]) ++left;
        while (left < right && arr[right] > arr[pivot]) --right;
        if (left < right) {
            int temp = arr[right];
            arr[right] = arr[left];
            arr[left] = temp;
        }
    }
    int temp = arr[pivot];
    arr[pivot] = arr[left];
    arr[left] = temp;


    quickSort(it1, (int) pivot);
    quickSort(it2 + pivot + 1, (int) arrSize - pivot - 1);
    // 0, 1, 2, 3, 4

}

// while left <= right
// while left < right && arr[left] < arr[pivot] ++left
// while left < right && arr[right] > arr[pivot] --right

int findLucky(int* arr, int arrSize) {
    // 1. quicksort
    // 2. iterate from back
    // 3. Stop when lucky number exists or iterator reaches 0
    // 4. Return lucky number or -1;

    // 1. Trivial quicksort with right most element
    // left, ... , right, pivot
    // left/right, pivot

    // Wait.. In the examples they are already sorted, let's try it out
    quickSort(arr, arrSize);


    if (arrSize == 1 && arr[0] == 1) return 1;
    else if (arrSize == 1) return -1;

    int curCount = 1;
    for (size_t arrIterator = arrSize-1; arrIterator > 0;) {
        if (arr[arrIterator--] == arr[arrIterator]) ++curCount;
        else if (arr[arrIterator+1] != arr[arrIterator]) {
            if (arr[arrIterator+1] == curCount) return curCount;
            curCount = 1;
        }
    }
    if (arr[0] == curCount) return curCount;
    return -1;
}
