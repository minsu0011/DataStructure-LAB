#include<stdio.h>
#include<stdlib.h>

FILE* fin;
FILE* fout;

typedef struct Array Array;

struct Array{
    int size;
    int* values;
};

Array* CreateArray(int size);
void QuickSort(Array* array, int left, int right);
int Partition(Array* array, int left, int right);
void PrintArray(Array* array, int left, int right);
void DeleteArray(Array* array);
void swap(int* a, int* b);

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

    int size, i;
    Array* array;

    fscanf(fin, "%d", &size);
    array = CreateArray(size);
    for(i = 0; i < size; i++){
        fscanf(fin, "%d", &array->values[i]);
    }

    QuickSort(array, 0, size - 1);

    fprintf(fout, "\nsorting result :\n");
    PrintArray(array, 0, size - 1);

    DeleteArray(array);


	return 0;
}

Array* CreateArray(int size){
    Array* array = (Array*)malloc(sizeof(Array)); // array 할당
    array->size = size; // size 초기화
    array->values = (int*)malloc(sizeof(int) * size); // values 배열 할당
    return array;
}

void QuickSort(Array* array, int left, int right){
    if(left < right){ // left >= right 이면 quicksort 불필요
        int p = Partition(array, left, right); // 피봇 설정

        QuickSort(array, left, p - 1); // 피봇 왼쪽 정렬
        QuickSort(array, p + 1, right); // 피봇 오른쪽 정렬
    }
}

int Partition(Array* array, int left, int right){
    int pivot = array->values[right]; // 피봇 맨 오른쪽 값으로 설정
    int low = left; // i
    int high = right - 1; // j
    fprintf(fout, "left : %d, right : %d, pivot : %d\n", left, right, pivot);
    PrintArray(array, left, right);
    while(low <= high){
        while(array->values[low] < pivot) low ++; // pivot 보다 큰 값 찾기 왼쪽부터
        while(array->values[high] > pivot) high --; // pivot 보다 작은 값 찾기 오른쪽부터

        if(low <= high){ // 넘치지 않을 정도에서 low, high 결정되면
            swap(&array->values[low], &array->values[high]); // swap
            PrintArray(array, left, right);
        }
        
    }
    if(right != low){ // pivot과 위 동작 종료 지점이 같지 않다면
        swap(&array->values[right], &array->values[low]); // swap
        PrintArray(array, left, right);
    }
    return low;
}

void PrintArray(Array* array, int left, int right){
    int i;
    for(i = 0; i < array->size; i++){
        if(i < left || right < i){
            fprintf(fout, "%-3s", "-");
        }
        else{
            fprintf(fout, "%-3d", array->values[i]);
        }
    }
    fprintf(fout, "\n");
    return;
}

void DeleteArray(Array* array){
    free(array->values);
    free(array);
}

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}
