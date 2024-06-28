#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define INF 1e7

FILE *fin;
FILE *fout;

typedef struct Node {
	int index; //index of this node
	int dist;	//distance for heap
	int prev;
}Node;

typedef struct Graph {
	int size;
	int** distMatrix;
	Node** nodes;
}Graph;

typedef struct Heap {
	int capacity;
	int size;
	Node** element;
}Heap;

//구현해야 하는 함수
Graph* CreateGraph(int size);
void DeleteGraph(Graph* g);
void PrintShortestPath(Graph* g);
Heap* CreateHeap(int heapSize);
void DeleteHeap(Heap* heap);
void Push(Heap* heap, Node* node);
bool Find(Heap* heap, Node* node);
Node* Pop(Heap* heap);

//구현하면 좋은 함수
bool isEmpty(Heap* heap){ // heap이 비었는지 check
    if(heap->size == 0) return true;
    else return false;
}

void main(int argc, char *argv[]) {
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	Graph* g;
	int size;
	fscanf(fin, "%d\n", &size);
	g = CreateGraph(size);

	char tmp = 0;
	while (tmp != '\n' && tmp != EOF) {
		int node1, node2, weight;
		fscanf(fin, "%d-%d-%d", &node1, &node2, &weight);
		g->distMatrix[node1 - 1][node2 - 1] = weight;
		tmp = fgetc(fin);
	}

	PrintShortestPath(g);

	DeleteGraph(g);
}

Graph* CreateGraph(int size) {
    Graph* graph = (Graph*)malloc(sizeof(Graph)); // 그래프 할당
    graph->size = size; // 그래프 크기 초기화
    int** t = (int**)malloc(sizeof(int*) * size); // 임시 변수에 할당
    for(int i = 0 ; i < size ; i++)
        t[i] = (int*)malloc(sizeof(int) * size); // distMatrix 각각 할당
    for(int i = 0 ; i < size ; i++)
        for(int j = 0 ; j < size ; j++)
            t[i][j] = INF; // 초기화
    graph->distMatrix = t; // distMatrix에 임시변수t 할당

    graph->nodes = (Node**)malloc(sizeof(Node*) * size); // nodes 할당
    for(int i = 0 ; i < size ; i++){
        graph->nodes[i] = (Node*)malloc(sizeof(Node)); // nodes[i] 할당
        graph->nodes[i]->index = i + 1; // 인덱스 번호 초기화
        graph->nodes[i]->dist = INF; // 초기화
        graph->nodes[i]->prev = INF; // 초기화
    }
    return graph;
}

void DeleteGraph(Graph* graph){ // Create 역순으로 free
    for(int i = 0 ; i < graph->size ; i++){
        free(graph->nodes[i]);
        free(graph->distMatrix[i]);
    }
    free(graph->nodes);
    free(graph->distMatrix);
    free(graph);
}

Heap* CreateHeap(int heapSize) {
    Heap* heap = (Heap*)malloc(sizeof(Heap)); //할당
    heap->capacity = heapSize; //전체사이즈 
    heap->size = 0;

    heap->element = (Node**)malloc(sizeof(Node*) * heapSize); // element 할당
    for(int i = 0 ; i < heapSize ; i++){
        heap->element[i] = (Node*)malloc(sizeof(Node)); // element[i] 할당
        heap->element[i]->index = i + 1; // index 번호 붙이기
        heap->element[i]->dist = INF; // 초기화
        heap->element[i]->prev= INF; // 초기화
    }
    return heap;
}

void DeleteHeap(Heap* heap){ // Create 역순으로 free
    for(int i = 0 ; i < heap->capacity ; i++)
        free(heap->element[i]);
    free(heap->element);
    free(heap);
}

void PrintShortestPath(Graph* graph) {
    graph->nodes[0]->dist = 0; // 1번 노드 dist = 0 초기화
    Heap* heap = CreateHeap(graph->size); // 힙 생성
    Push(heap, graph->nodes[0]); // 1번 노드 push
    while(!isEmpty(heap)){ // heap이 비어있을 때가지 반복
        Node* a = Pop(heap); // min Node pop
        int distOA = a->dist; // 1번노드부터 A node 까지의 dist  
        for(int i = 0 ; i < graph->size ; i++){
            Node* b = graph->nodes[i]; // 모든 노트 탐색 b = nodes[i]
            if(a->index == b->index) continue; // 같은 node 탐색x
            int distOB = b->dist; // node b 까지의 거리
            if(distOA + graph->distMatrix[a->index - 1][b->index - 1] < distOB){ // node B까지의 거리가 node A까지의 거리 + AB dist 보다 크면
                b->dist = distOA + graph->distMatrix[a->index - 1][b->index - 1]; // 갱신
                b->prev = a->index; // b 까지의 최단 거리는 a를 거쳐서 간다
                Push(heap, b); // node b push
            }
        }
    }

    for(int i = 0 ; i < graph->size ; i++){ // print part
        if(i == 0) continue; // node 1 -> node 1 생략
        int cnt = 0;
        int* arr = (int*)malloc(sizeof(int) * graph->size); // 이동 경로 저장할 array 할당
        int p = graph->nodes[i]->prev; // 바로 전 node = p
        if(p == INF){ // p == INF 이면 한번도 갱신되지 않은 node이므로 can not reach
            fprintf(fout, "can not reach to node %d", i + 1);
            continue;
        }
        while(p != 1){ // 바로 전 node가 1이 될때까지
            arr[cnt] = p;
            p = graph->nodes[p - 1]->prev; // arr[]에 경로 저장
            cnt ++;
        }
        fprintf(fout, "1->");
        for(int j = cnt - 1 ; j >= 0 ; j --){
            fprintf(fout, "%d->", arr[j]); // arr에 저장된 경로 출력
        }
        fprintf(fout, "%d", i + 1); // 최종 도착 Node
        fprintf(fout, " (cost : %d)\n", graph->nodes[i]->dist); // print cost
        free(arr); // arr 배열 free
    }
    DeleteHeap(heap); // heap free
}

void Push(Heap* heap, Node* node) { // lab06 heap_push 수정해서 사용
    if(heap->size >= heap->capacity){ //Capacity가 넘어가면 full
        fprintf(fout, "push error : heap is full\n");
        return;
    }
    if(Find(heap, node)){ // 이미 존재하면(이미 방문한 node) return
        return;
    }
    heap->element[(heap->size)++] = node; //사이즈 증가 및 element마지막 위치에 node 저장
    int now = heap->size - 1; //현재 위치(마지막)
    while(now > 0){ // now == 0 이 될때 까지 루프
        if(heap->element[now]->dist < heap->element[(now - 1) / 2]->dist){ //부모보다 크면
            Node* t = heap->element[now]; // swap node
            heap->element[now] = heap->element[(now - 1) / 2];
            heap->element[(now - 1) / 2] = t; //swap node
            now = (now - 1) / 2; //now = 부모위치
        }
        else break; //부모보다 이미 작으면 루프 종료
    }
}


bool Find(Heap* heap, Node* node){
    for(int i = 0 ; i < heap->size ; i++) //모든 element 탐색
        if(heap->element[i]->index == node->index)
            return true;
    return false;
}

Node* Pop(Heap* heap){ // min_heap에 맞게 수정
    if(heap->size == 0) return NULL; //힙이 비어있을경우 NULL리턴
    Node* tnode = heap->element[0]; // pop 
    Node* max_node = tnode; //max_node 저장(pop된 node)
    heap->element[0] = heap->element[heap->size - 1];
    heap->element[heap->size - 1] = tnode; //맨마지막과 맨처음 swap
    heap->size--; //size -1
    int now = 0; //현재위치(root)
    while(now <= heap->size / 2 - 1){ //자식이 존재하는 노드까지 루프
        if((now * 2 + 1) == heap->size - 1){ // 자식이 1개일 경우
            if(heap->element[now]->dist > heap->element[now * 2 + 1]->dist){ //Heapify
                Node* t = heap->element[now];
                heap->element[now] = heap->element[now * 2 + 1];
                heap->element[now * 2 + 1] = t;
                now = now * 2 + 1;
            }
            break; //else 고려할 필요 없음
        }
        else{ // 자식이 두개일경우
            if(heap->element[2 * now + 1]->dist > heap->element[2 * now + 2]->dist && heap->element[now]->dist > heap->element[2 * now + 2]->dist){
                Node* t = heap->element[now];
                heap->element[now] = heap->element[2 * now + 2];
                heap->element[2 * now + 2] = t;
                now = 2 * now + 2; //오른쪽 자식노드와 swap(Heapify)
            }
            else if(heap->element[2 * now + 1]->dist < heap->element[2 * now + 2]->dist && heap->element[now]->dist > heap->element[2 * now + 1]->dist){
                Node* t = heap->element[now];
                heap->element[now] = heap->element[2 * now + 1];
                heap->element[2 * now + 1] = t;
                now = 2 * now + 1; //왼쪽 자식노드와 swap
            }
            else break; // Heapify 종료
        }
    }
    return max_node; // pop된 node return
}
