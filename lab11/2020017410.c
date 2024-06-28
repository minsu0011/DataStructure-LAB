#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

FILE *fin;
FILE *fout;

typedef struct _DisjointSet {
	int size_cell;
	int* ptr_cell;
	int size_wall;
	bool* ptr_wall;
}DisjointSets;

DisjointSets* init(int num);
void Union(DisjointSets *sets, int i, int j);
int find(DisjointSets *sets, int i);
void createMaze(DisjointSets *sets, int num);
void printMaze(DisjointSets *sets, int num);
void freeMaze(DisjointSets *sets);
void swap(int *x, int *y);

void swap(int *x, int *y) {
    int t = *x;
    *x = *y;
    *y = t;
}

int main(int argc, char* argv[]) {

	int num, i;
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	fscanf(fin, "%d", &num);

	DisjointSets* sets = init(num);
	createMaze(sets, num);
	printMaze(sets, num);

	freeMaze(sets);

	fclose(fin);
	fclose(fout);

	return 0;
}

DisjointSets* init(int num) {
    DisjointSets* sets = (DisjointSets*)malloc(sizeof(DisjointSets));
    sets->size_cell = num * num; // num제곱으로 초기화
    sets->ptr_cell = (int*)malloc(sizeof(int) * sets->size_cell); // num제곱만큼 방 생성
    for(int i = 0 ; i < sets->size_cell ; i++) sets->ptr_cell[i] = -1; // root 본인으로 초기화
    sets->size_wall = sets->size_cell * 2; // 방 개수 * 2개의 벽
    sets->ptr_wall = (bool*)malloc(sizeof(bool) * sets->size_wall); // 벽 개수 만큼 방 생성
    for(int i = 0 ; i < sets->size_wall ; i++) sets->ptr_wall[i] = true; // 벽 ture로 초기화
    sets->ptr_wall[sets->size_wall - 2] = false; // 출구 false으로 초기화
	return sets;
}

void Union(DisjointSets *sets, int i, int j) {
    int pi = find(sets, i); // find i parent
    int pj = find(sets, j); // find j parent
    if(pi == pj) return; // i==j -> return
    if(sets->ptr_cell[pi] < sets->ptr_cell[pj]) sets->ptr_cell[pj] = pi; // pj의 rank가 더 작은경우 -> pi에 pj를 합친다
    else if(sets->ptr_cell[pi] > sets->ptr_cell[pi]) sets->ptr_cell[pi] = pj; // 더 큰 경우 -> pj에 pi를 합친다
    else{ // 같은 경우 rank --
        sets->ptr_cell[pj] --;
        sets->ptr_cell[pi] = pj;
    }
}

int find(DisjointSets *sets, int i) {	
    while(sets->ptr_cell[i] >= 0) i = sets->ptr_cell[i]; // 부모 탐색
    return i; // 부모 return
}


void shuffle(int* array, int num){
    srand(time(NULL));
    for(int i = 0 ; i < num - 1 ; i++){
        int rn = rand() % (num - i) + i; // i ~ num-1 임의의 수 생성
        swap(&array[i], &array[rn]);
    }
}

void createMaze(DisjointSets *sets, int num) {
    int* arr = (int*)malloc(sizeof(int) * num * num); // arr 할당
    for(int i = 0 ; i < sets->size_cell ; i++) arr[i] = i; // arr 0~num2-1
    shuffle(arr, sets->size_cell); // random하게 shuffle

    for(int i = 0 ; i < sets->size_cell ; i++){ // shuffle한 arr에 대해 전부 탐색
        int x = arr[i];
        if((x + 1) % num != 0){ // 맨 오른쪽 줄 제외
            if(find(sets, x) != find(sets, x + 1)){ // 오른쪽과 다른 set이면
                Union(sets, x, x + 1); // Union
                sets->ptr_wall[2 * x] = false; // 벽 허물기
            }
        }
        if((x / num) < num - 1){ // 맨 아랫쪽 줄 제외
            if(find(sets, x) != find(sets, x + num)){ // 다른 set이면 
                Union(sets, x, x + num); // Union
                sets->ptr_wall[2 * x + 1] = false; // 벽허물기
            }
        }
    }
    free(arr);
}

void printMaze(DisjointSets *sets, int num) {
    for (int i = 0; i < num; i++) fprintf(fout, "+---");
    fprintf(fout, "+\n"); // 맨 윗줄
    for(int i = 0 ; i < 2 * num ; i++){
        if(i % 2 == 0){
            if(i == 0) fprintf(fout, " ");
            else fprintf(fout, "|");
            for(int j = 0 ; j < num ; j++){
                if(sets->ptr_wall[2 * j + (i / 2) * num * 2]) fprintf(fout, "   |"); // 벽이 있을경우
                else fprintf(fout, "    "); // 없을 경우
            }
        }
        else{ 
            fprintf(fout, "+");
            for(int j = 0 ; j < num ; j++){
                if(sets->ptr_wall[2 * j + (i / 2) * num * 2 + 1]) fprintf(fout, "---+"); // 있을 경우
                else fprintf(fout, "   +"); // 없을 경우
            }
        }
        fprintf(fout, "\n");
    }
}

void freeMaze(DisjointSets *sets) {
    free(sets->ptr_cell);
    free(sets->ptr_wall);
    free(sets);
}