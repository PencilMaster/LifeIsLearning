// I drew inspiration from a stackoverflow post.
//
// It worked super fast after a quick implementation with an added swap function,
// that the code is not as cluttered.
//


void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int medianOfThree(int* arr, const int l, const int m, const int h) {
    int low = arr[l];
    int mid = arr[m];
    int high = arr[h];

    if ((low < mid && mid < high) || (high < mid && mid < low))
        return m;
    else if ((mid < low && low < high) || (high < low && low < mid))
        return l;
    else return h;
}

int partition(int* arr, const int low, const int high) {
    int pivotIndex = medianOfThree(arr, low, low + (high - low) / 2, high);
    swap(arr + pivotIndex, arr + high);

    int left = low-1;
    int right = low;
    while (right < high) {
        if (arr[right++] < arr[high]) {
            ++left;
            swap(arr + left, arr + right-1);
        }
    }
    swap(arr + (++left), arr + high);
    return left;
}

void quickSort(int* arr, const int low, const int high) {
    if (high-low <= 0) return;
    int pivot = partition(arr, low, high);
    quickSort(arr, low, pivot - 1);
    quickSort(arr, pivot + 1, high);
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
    quickSort(arr, 0, arrSize-1);

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

