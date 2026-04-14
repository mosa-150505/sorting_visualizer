#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 560
#define ARR_SIZE 130
#define RECT_SIZE 7

int arr[ARR_SIZE];
int Barr[ARR_SIZE];

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int complete = 0;

int init(void);
void closeProgram(void);
void visualize(int x, int y, int z);
void inplaceHeapSort(int* input, int n);
int partition_array(int a[], int si, int ei);
void quickSort(int a[], int si, int ei);
void merge2SortedArrays(int a[], int si, int ei);
void mergeSort(int a[], int si, int ei);
void bubbleSort(void);
void insertionSort(void);
void selectionSort(void);
void loadArr(void);
void randomizeAndSaveArray(void);
void printControls(void);

// SLD initializer
int init(void)
{
    int success = 1;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Couldn't initialize SDL. SDL_Error: %s\n", SDL_GetError());
        success = 0;
        return success;
    }
    else
    {
        if (!(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")))
        {
            printf("Warning: Linear Texture Filtering not enabled.\n");
        }

        window = SDL_CreateWindow("Sorting Visualizer (C)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            printf("Couldn't create window. SDL_Error: %s\n", SDL_GetError());
            success = 0;
            return success;
        }
        else
        {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL)
            {
                printf("Couldn't create renderer. SDL_Error: %s\n", SDL_GetError());
                success = 0;
            }
        }
    }

    return success;
}

// Cleaner
void closeProgram(void)
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();
}

// Visualizer
void visualize(int x, int y, int z)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int j = 0;
    for (int i = 0; i <= SCREEN_WIDTH - RECT_SIZE; i += RECT_SIZE)
    {
        SDL_Rect rect;
        rect.x = i;
        rect.y = 0;
        rect.w = RECT_SIZE;
        rect.h = arr[j];

        if (complete)
        {
            SDL_SetRenderDrawColor(renderer, 100, 180, 100, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
        else if (j == x || j == z)
        {
            SDL_SetRenderDrawColor(renderer, 100, 180, 100, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
        else if (j == y)
        {
            SDL_SetRenderDrawColor(renderer, 165, 105, 189, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 170, 183, 184, 255);
            SDL_RenderDrawRect(renderer, &rect);
        }
        j++;
    }
    SDL_RenderPresent(renderer);
}

// Some sorting Algo

void inplaceHeapSort(int* input, int n)
{
    for (int i = 1; i < n; i++)
    {
        int childIndex = i;
        int parentIndex = (childIndex - 1) / 2;

        while (childIndex > 0)
        {
            if (input[childIndex] > input[parentIndex])
            {
                int temp = input[parentIndex];
                input[parentIndex] = input[childIndex];
                input[childIndex] = temp;
            }
            else
            {
                break;
            }
            visualize(parentIndex, childIndex, -1);
            SDL_Delay(40);

            childIndex = parentIndex;
            parentIndex = (childIndex - 1) / 2;
        }
    }

    for (int heapLast = n - 1; heapLast >= 0; heapLast--)
    {
        int temp = input[0];
        input[0] = input[heapLast];
        input[heapLast] = temp;

        int parentIndex = 0;
        int leftChildIndex = 2 * parentIndex + 1;
        int rightChildIndex = 2 * parentIndex + 2;

        while (leftChildIndex < heapLast)
        {
            int maxIndex = parentIndex;

            if (input[leftChildIndex] > input[maxIndex])
                maxIndex = leftChildIndex;
            if (rightChildIndex < heapLast && input[rightChildIndex] > input[maxIndex])
                maxIndex = rightChildIndex;
            
            if (maxIndex == parentIndex)
                break;

            int temp_swap = input[parentIndex];
            input[parentIndex] = input[maxIndex];
            input[maxIndex] = temp_swap;

            visualize(maxIndex, parentIndex, heapLast);
            SDL_Delay(40);

            parentIndex = maxIndex;
            leftChildIndex = 2 * parentIndex + 1;
            rightChildIndex = 2 * parentIndex + 2;
        }
    }
}

int partition_array(int a[], int si, int ei)
{
    int count_small = 0;
    for (int i = si + 1; i <= ei; i++)
    {
        if (a[i] <= a[si])
            count_small++;
    }
    
    int c = si + count_small;
    int temp = a[c];
    a[c] = a[si];
    a[si] = temp;
    visualize(c, si, -1);
    SDL_Delay(50);

    int i = si, j = ei;
    while (i < c && j > c)
    {
        if (a[i] <= a[c])
            i++;
        else if (a[j] > a[c])
            j--;
        else
        {
            int temp_1 = a[j];
            a[j] = a[i];
            a[i] = temp_1;
            visualize(i, j, -1);
            SDL_Delay(70);
            i++;
            j--;
        }
    }
    return c;
}

void quickSort(int a[], int si, int ei)
{
    if (si >= ei) return;
    int c = partition_array(a, si, ei);
    quickSort(a, si, c - 1);
    quickSort(a, c + 1, ei);
}

void merge2SortedArrays(int a[], int si, int ei)
{
    int size_output = (ei - si) + 1;
    int* output = (int*)malloc(size_output * sizeof(int));
    if (!output) return;

    int mid = (si + ei) / 2;
    int i = si, j = mid + 1, k = 0;

    while (i <= mid && j <= ei)
    {
        if (a[i] <= a[j])
        {
            output[k] = a[i];
            visualize(i, j, -1);
            i++; k++;
        }
        else
        {
            output[k] = a[j];
            visualize(i, j, -1);
            j++; k++;
        }
    }
    while (i <= mid) { output[k] = a[i]; visualize(-1, i, -1); i++; k++; }
    while (j <= ei) { output[k] = a[j]; visualize(-1, j, -1); j++; k++; }

    for (int l = si, x = 0; l <= ei; l++, x++)
    {
        a[l] = output[x];
        visualize(l, -1, -1);
        SDL_Delay(15);
    }
    free(output);
}

void mergeSort(int a[], int si, int ei)
{
    if (si >= ei) return;
    int mid = (si + ei) / 2;
    mergeSort(a, si, mid);
    mergeSort(a, mid + 1, ei);
    merge2SortedArrays(a, si, ei);
}

void bubbleSort(void)
{
    for (int i = 0; i < ARR_SIZE - 1; i++)
    {
        for (int j = 0; j < ARR_SIZE - 1 - i; j++)
        {
            if (arr[j + 1] < arr[j])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                visualize(j + 1, j, ARR_SIZE - i);
            }
            SDL_Delay(1);
        }
    }
}

void insertionSort(void)
{
    for (int i = 1; i < ARR_SIZE; i++)
    {
        int j = i - 1;
        int temp = arr[i];
        while (j >= 0 && arr[j] > temp)
        {
            arr[j + 1] = arr[j];
            j--;
            visualize(i, j + 1, -1);
            SDL_Delay(5);
        }
        arr[j + 1] = temp;
        visualize(i, j + 1, -1);
    }
}

void selectionSort(void)
{
    int minIndex;
    for (int i = 0; i < ARR_SIZE - 1; i++)
    {
        minIndex = i;
        for (int j = i + 1; j < ARR_SIZE; j++)
        {
            if (arr[j] < arr[minIndex])
            {
                minIndex = j;
                visualize(i, minIndex, -1);
            }
            SDL_Delay(1);
        }
        int temp = arr[i];
        arr[i] = arr[minIndex];
        arr[minIndex] = temp;
    }
}

// Loaders
void loadArr(void)
{
    memcpy(arr, Barr, sizeof(int) * ARR_SIZE);
    complete = 0;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void randomizeAndSaveArray(void)
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < ARR_SIZE; i++)
        Barr[i] = (rand() % (SCREEN_HEIGHT - RECT_SIZE)) + RECT_SIZE;
}

void printControls(void)
{
    printf("\n==========================================================\n");
    printf("       SORTING VISUALIZER CONTROLS\n");
    printf("==========================================================\n");
    printf("  [0] : Randomise a new list\n");
    printf("  [1] : Selection Sort\n");
    printf("  [2] : Insertion Sort\n");
    printf("  [3] : Bubble Sort\n");
    printf("  [4] : Merge Sort\n");
    printf("  [5] : Quick Sort\n");
    printf("  [6] : Heap Sort\n");
    printf("  [Q] : Quit\n");
    printf("==========================================================\n");
}

// Main loop
void execute(void)
{
    if (!init())
    {
        printf("SDL Initialization Failed.\n");
        return;
    }

    randomizeAndSaveArray();
    loadArr();
    printControls();

    SDL_Event e;
    int quit = 0;
    int sortingInProgress = 0;

    while (!quit)
    {
        // Events
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = 1;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                // Only one sort 
                if (sortingInProgress && e.key.keysym.sym != SDLK_q) continue;

                switch (e.key.keysym.sym)
                {
                    case SDLK_q:
                        quit = 1;
                        printf("\nEXITING...\n");
                        break;
                    case SDLK_0:
                        randomizeAndSaveArray();
                        loadArr();
                        printf("\nNEW RANDOM LIST GENERATED.\n");
                        break;
                    case SDLK_1:
                        loadArr();
                        printf("\nSELECTION SORT STARTED.\n");
                        sortingInProgress = 1;
                        selectionSort();
                        complete = 1; sortingInProgress = 0;
                        printf("SELECTION SORT COMPLETE.\n");
                        break;
                    case SDLK_2:
                        loadArr();
                        printf("\nINSERTION SORT STARTED.\n");
                        sortingInProgress = 1;
                        insertionSort();
                        complete = 1; sortingInProgress = 0;
                        printf("INSERTION SORT COMPLETE.\n");
                        break;
                    case SDLK_3:
                        loadArr();
                        printf("\nBUBBLE SORT STARTED.\n");
                        sortingInProgress = 1;
                        bubbleSort();
                        complete = 1; sortingInProgress = 0;
                        printf("BUBBLE SORT COMPLETE.\n");
                        break;
                    case SDLK_4:
                        loadArr();
                        printf("\nMERGE SORT STARTED.\n");
                        sortingInProgress = 1;
                        mergeSort(arr, 0, ARR_SIZE - 1);
                        complete = 1; sortingInProgress = 0;
                        printf("MERGE SORT COMPLETE.\n");
                        break;
                    case SDLK_5:
                        loadArr();
                        printf("\nQUICK SORT STARTED.\n");
                        sortingInProgress = 1;
                        quickSort(arr, 0, ARR_SIZE - 1);
                        complete = 1; sortingInProgress = 0;
                        printf("QUICK SORT COMPLETE.\n");
                        break;
                    case SDLK_6:
                        loadArr();
                        printf("\nHEAP SORT STARTED.\n");
                        sortingInProgress = 1;
                        inplaceHeapSort(arr, ARR_SIZE);
                        complete = 1; sortingInProgress = 0;
                        printf("HEAP SORT COMPLETE.\n");
                        break;
                }
            }
        }

        // 2. Visual renderer in case none of those sorting are in progress
        if (!sortingInProgress) {
            visualize(-1, -1, -1);
        }

        SDL_Delay(10);
    }

    closeProgram();
}

int main(int argc, char* args[])
{
    (void)argc;
    (void)args;
    
    printf("==============================Sorting Visualizer (C)==============================\n");

    
    execute();
    return 0;
}
