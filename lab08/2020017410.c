#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

FILE *fin;
FILE *fout;

typedef struct _QueueNode QueueNode;

struct _QueueNode{
    QueueNode* next;
    int data;
};

typedef struct _Queue {
	QueueNode* front;
	QueueNode* rear;
	int size;
}Queue;

typedef struct _Graph {
	int size;
	int* node;
	bool** matrix;
}Graph;

Graph* CreateGraph(int* nodes, int n);
void InsertEdge(Graph* G, int a, int b);
void PrintGraph(Graph* G);
void DeleteGraph(Graph* G);
void TopologicalSort(Graph* G);
Queue* CreateQueue();
bool IsEmpty(Queue* Q);
int Dequeue(Queue* Q);
void Enqueue(Queue* Q, int X);
void DeleteQueue(Queue* Q);

void SetIndegree(Graph* G, int* idg);
void SortNode(int* arr, int n);


void countInput(int* n, char* str) { //0~9의 숫자 개수 세기
	int len = strlen(str), i;
	for (i = 0; i < len; i++)
		if (0 <= str[i] - '0' && str[i] - '0' < 10) (*n)++;
}

void parseInput(int* arr, char* str, int n) { // int로 바꾸어 저장
	int len = strlen(str), i;
	int cnt = 0;
	for (i = 0; i < len; i++)
	if (0 <= str[i] - '0' && str[i] - '0' < 10) arr[cnt++] = str[i] - '0';
}

int main(int argc, char* agrv[]) {
	fin = fopen(agrv[1], "r");
	fout = fopen(agrv[2], "w");
    
	char numbers[100], inputs[100];
	fgets(numbers, 100, fin);
	int n = 0;
	countInput(&n, numbers);
	int* nodes = (int*)malloc(sizeof(int)*n);
	parseInput(nodes, numbers, n);
	
    SortNode(nodes, n);
	Graph* G = CreateGraph(nodes, n);

	fgets(inputs, 100, fin);
	int len = strlen(inputs), i, j;
	for (i = 0; i < len; i += 4) {
		int a = inputs[i] - '0', b = inputs[i + 2] - '0';
		InsertEdge(G, a, b);
	}

	PrintGraph(G);
    fprintf(fout, "\n");
	TopologicalSort(G);
	
	DeleteGraph(G);

	fclose(fin);
	fclose(fout);

	return 0;
}


Graph* CreateGraph(int* nodes, int n){ // 그래프 생성
    Graph* graph = (Graph*)malloc(sizeof(Graph)); // 메모리 할당
    graph->size = n;
    graph->node = nodes; // 각 변수들 초기화
    bool** t = NULL; // 임시 변수
    t = (bool**)malloc(sizeof(bool*) * n); // 임시 변수에 할당
    for(int i = 0 ; i < n ; i++)
        *(t + i) = (bool*)malloc(sizeof(bool) * n); // 각 t[i]에 할당
    for(int i = 0 ; i < n ; i++)
        for(int j = 0 ; j < n ; j++)
            t[i][j] = false; // false로 초기화
    graph->matrix = t; // 임시 변수 matrix에 할당
    return graph;
}

void InsertEdge(Graph* G, int a, int b){
    int ap = 0, bp = 0;
    for(int i = 0 ; i < G->size ; i++){ // a,b의 해당 인데스 찾기
        if(G->node[i] == a) ap = i;
        if(G->node[i] == b) bp = i;
    }
    G->matrix[ap][bp] = true; // 해당 edge 적용
}

void PrintGraph(Graph* G){ // matrix 출력
    fprintf(fout, "0 ");
    for(int i = 0 ; i < G->size ; i++) // 첫줄 node정보 출력(행)
        fprintf(fout, "%d ", G->node[i]);
    fprintf(fout, "\n");
    for(int i = 0 ;i < G->size ; i++){
        fprintf(fout, "%d ", G->node[i]); // 첫줄 node정보 출력(열)
        for(int j = 0 ; j < G->size ; j++)
            fprintf(fout, "%d ",G->matrix[i][j]); // 출력
        fprintf(fout, "\n");
    }
}

void DeleteGraph(Graph* G){
    free(G->node); // node free
    for(int i = 0 ; i < G->size ; i++) // matirx[i] free
        free(G->matrix[i]);
    free(G->matrix); // matrix free
}

void TopologicalSort(Graph* G){
    Queue* Q = CreateQueue(); // Queue 생성
    int* indegree = (int*)malloc(sizeof(int) * G->size); // indegree 할당
    for(int i = 0 ; i < G->size ; i++) indegree[i] = 0; // indegree 초기화
    int* printorder = (int*)malloc(sizeof(int) * G->size); // printorder 할당
    for(int i = 0 ; i < G->size ; i++) printorder[i] = 0; // printorder 초기화
    int printorder_size = 0;
    SetIndegree(G, indegree); // Indegree 설정
    for(int i = 0 ; i < G->size ; i++){ // indegree가 0인 node의 index를 Enqueue
        if(indegree[i] == 0) Enqueue(Q, i);
    }
    while(!IsEmpty(Q)){
        int ele = Dequeue(Q); // Dequeue
        for(int i = 0 ; i < G->size ; i++){
            if(G->matrix[ele][i] == true){ // Dequeue한 index에 해당하는 node가 가리키는 node에 대해
                indegree[i] --; // indegree 1 감소
                if(indegree[i] == 0) Enqueue(Q, i); // 감소 후 indegree가 0이면 해당 노드의 index를 Enqueue
            }
        }
        printorder[printorder_size] = G->node[ele]; // Dequeue한 node printorder에 저장
        printorder_size ++; // 사이즈 + 1
    }
    if(G->size != printorder_size) // 모두 시행 후 node의 개수와 printorder_size가 다르면 사이클 존재
        fprintf(fout, "sorting error : cycle");
    else{
        for(int i = 0 ; i < printorder_size ; i++) // printorder 출력
            fprintf(fout, "%d ", printorder[i]);
    }
    free(printorder); // free printorder
    free(indegree); // free indegree
    DeleteQueue(Q); // free Q
}

Queue* CreateQueue(){ // Queue 생성 및 할당
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = NULL; // 각 변수 초기화
    q->rear = NULL;
    q->size = 0;
    return q;
}

bool IsEmpty(Queue* Q){ // Q가 비었으면 return true
    if(Q->size == 0) return true;
    return false;
}

int Dequeue(Queue* Q){
    QueueNode* freeQ; // Dequeue 후 free할 QueueNode(Q->front)
    if(IsEmpty(Q)) return -1;
    freeQ = Q->front;
    int element = Q->front->data; // element : return할 정수
    Q->front = Q->front->next; // 포인터 갱신
    free(freeQ); // free QueueNode(Dequeue할)
    Q->size --; // size -1
    if(IsEmpty(Q)) // size = 0 이 되면 Q->rear = NULL(Q->front는 이미NULL)
        Q->rear = NULL;
    return element;
}

void Enqueue(Queue* Q, int X){
    if(IsEmpty(Q)){ // Queue가 비어있으면 새롭게 할당 후 front, rear모두 새로운 QueueNode 가리킴
        Q->front = (QueueNode*)malloc(sizeof(QueueNode));
        Q->front->data = X;
        Q->front->next = NULL;
        Q->rear = Q->front;
        Q->size++; // size +1
        return;
    }
    // Queue가 비어있지 않다면
    Q->rear->next = (QueueNode*)malloc(sizeof(QueueNode)); // 새로운 QueueNode 생성
    Q->rear->next->data = X; // 새로운 노드에 data 입력
    Q->rear->next->next = NULL; // 새로운 노드가 가리키는 다음 Node는 NULL
    Q->size ++; // size +1
    Q->rear = Q->rear->next; // Q->rear가 새로운 노드를 기리킴
}

void DeleteQueue(Queue* Q){
    while(!IsEmpty(Q)){
        int t = Dequeue(Q); // Queue안의 모든 QueueNode Dequeue(Dequeue(Q)에서 free 수행)
    }
    free(Q); // free Q
}

void SetIndegree(Graph* G, int* idg){
    for(int i = 0 ; i < G->size ; i++)
        for(int j = 0 ; j < G->size ; j++)
            if(G->matrix[i][j] == true) idg[j] ++; // Indegree Array를 초기화
}

void SortNode(int* arr, int n){ // Bubble Sort
    for(int i = 0 ; i < n ; i++){
        for(int j = i + 1 ; j < n ; j ++){
            if(arr[i] > arr[j]){
                int t = arr[i];
                arr[i] = arr[j];
                arr[j] = t;
            }
        }
    }
}

