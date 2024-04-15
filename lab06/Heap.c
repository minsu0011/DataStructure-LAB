#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct HeapStruct{
	int Capacity;
	int Size;
	int *Elements;
}Heap;

Heap* CreateHeap(int heapSize);
void DeleteHeap(Heap* heap);
void Push(Heap *heap, int value);
bool Find(Heap *heap, int value);
int Pop(Heap* heap);
void PrintHeap(Heap* heap);

Heap* CreateHeap(int heapSize){
    Heap* heap = (Heap*)malloc(sizeof(Heap)); //할당
    heap->Capacity = heapSize; //전체사이즈 
    heap->Size = 0;
    heap->Elements = (int*)malloc(sizeof(int) * heapSize); //heapSize만큼 할당
    return heap;
}
void DeleteHeap(Heap* heap){
    free(heap); // heap free
}
void Push(Heap* heap, int value){
    if(heap->Size >= heap->Capacity){ //Capacity가 넘어가면 full
        fprintf(fout, "push error : heap is full\n");
        return;
    }
    if(Find(heap, value)){
        fprintf(fout, "push error : %d is already in the heap\n", value); //이미존재하면 error
        return;
    }
    heap->Elements[(++(heap->Size))-1] = value; //사이즈 증가 및 Elements마지막 위치에 value저장
    int now = heap->Size - 1; //현재 위치(마지막)
    while(now > 0){ // now == 0 이 될때 까지 루프
        if(heap->Elements[now] > heap->Elements[(now - 1) / 2]){ //부모보다 크면
            int t = heap->Elements[now];
            heap->Elements[now] = heap->Elements[(now - 1) / 2];
            heap->Elements[(now - 1) / 2] = t; //swap
            now = (now - 1) / 2; //now = 부모위치
        }
        else break; //부모보다 이미 작으면 루프 종료
    }
}
bool Find(Heap* heap, int value){
    for(int i = 0 ; i < heap->Size ; i++) //모든 Elements 탐색
        if(heap->Elements[i] == value)
            return true;
    return false;
}
int Pop(Heap* heap){
    if(heap->Size == 0) return -INF; //힙이 비어있을경우 -INF리턴
    int t = heap->Elements[0]; 
    int max_key = t; //max_key 저장(pop된 원소)
    heap->Elements[0] = heap->Elements[heap->Size - 1];
    heap->Elements[heap->Size - 1] = t; //맨마지막과 맨처음 swap
    heap->Size--; //size -1
    int now = 0; //현재위치(root)
    while(now <= heap->Size / 2 - 1){ //자식이 존재하는 노드까지 루프
        if((now * 2 + 1) == heap->Size - 1){ // 자식이 1개일 경우
            if(heap->Elements[now] < heap->Elements[now * 2 + 1]){ //Heapify
                int t = heap->Elements[now];
                heap->Elements[now] = heap->Elements[now * 2 + 1];
                heap->Elements[now * 2 + 1] = t;
                now = now * 2 + 1;
            }
            break; //else 고려할 필요 없음
        }
        else{ // 자식이 두개일경우
            if(heap->Elements[2 * now + 1] < heap->Elements[2 * now + 2] && heap->Elements[now] < heap->Elements[2 * now + 2]){
                int t = heap->Elements[now];
                heap->Elements[now] = heap->Elements[2 * now + 2];
                heap->Elements[2 * now + 2] = t;
                now = 2 * now + 2; //오른쪽 자식노드와 swap(Heapify)
            }
            else if(heap->Elements[2 * now + 1] > heap->Elements[2 * now + 2] && heap->Elements[now] < heap->Elements[2 * now + 1]){
                int t = heap->Elements[now];
                heap->Elements[now] = heap->Elements[2 * now + 1];
                heap->Elements[2 * now + 1] = t;
                now = 2 * now + 1; //왼쪽 자식노드와 swap
            }
            else break; // Heapify 종료
        }
    }
    return max_key; // pop된 원소 리턴
}
void PrintHeap(Heap* heap){ // 모든 Elements 탐색 및 출력
    if(heap->Size == 0) fprintf(fout, "print error : heap is empty");
    for(int i = 0 ; i < heap->Size ; i++) fprintf(fout, "%d ", heap->Elements[i]);
    fprintf(fout, "\n");
}

int main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	char cv;
	Heap* maxHeap;
	int heapSize, key, max_element;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'n':
				fscanf(fin, "%d", &heapSize);
				maxHeap = CreateHeap(heapSize);
				break;
			case 'i':
				fscanf(fin, "%d", &key);
				Push(maxHeap, key);
				break;
			case 'd':
				max_element = Pop(maxHeap);
				if(max_element != -INF){
					fprintf(fout, "max element : %d deleted\n", max_element);
				}
                else fprintf(fout, "pop error : heap is empty\n");
				break;
			case 'p':
				PrintHeap(maxHeap);
				break;
			case 'f':
				fscanf(fin, "%d", &key);
				if(Find(maxHeap, key)) fprintf(fout, "%d is in the heap\n", key);
				else fprintf(fout, "%d is not in the heap\n", key);
				break;
		}
	}
	DeleteHeap(maxHeap);

	return 0;
}