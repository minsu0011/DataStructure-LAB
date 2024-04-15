#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;


typedef struct BinarySearchTreeNode BSTNode;
struct BinarySearchTreeNode{
	int value;
	BSTNode* left;
	BSTNode* right;
};

BSTNode* insertNode(BSTNode* current, int key);
BSTNode* deleteNode(BSTNode* current, int key);
BSTNode* findNode(BSTNode* current, int key);
void printInorder(BSTNode* current);
void deleteTree(BSTNode* current);

BSTNode* insertNode(BSTNode* current, int key){
    if(current == NULL){//if current == NULL일 경우 비어있는 노드 -> 새로운 노드 할당
		current = (BSTNode*)malloc(sizeof(BSTNode));
		current->left = NULL;
		current->right = NULL;
		current->value = key;
		return current;
	}
	else if(key < current->value) //left recursive
		current->left = insertNode(current->left, key);
    else if(key > current->value) //right recursive
		current->right = insertNode(current->right, key);
    else //이미 있을 경우 에러메시지 출력
		fprintf(fout,"insertion error %d", key);
    return current;
}
BSTNode* deleteNode(BSTNode* current, int key){
	BSTNode* t = NULL;
    if(current == NULL) return current; //return NULL
    else if(key < current->value)
		current->left = deleteNode(current->left, key); //delete할 노드 찾아가기 left recursive
    else if(key > current->value)
		current->right = deleteNode(current->right, key); //delete할 노드 찾아가기 right recursive
    else{ // delete할 노드를 발견
		if(current->left != NULL && current->right != NULL){ //자식이 둘일경우
			t = current->right; // 오른쪽 노드의 가장 왼쪽 자식노드 찾기
			while(t->left != NULL) t = t->left;
			current->value = t->value; //현재노드에 찾은 노드의 value를 대입
			current->right = deleteNode(current->right, current->value); //오른쪽 노드를 올리기
		}
		else if(current->left == NULL && current->right != NULL){
			t = current->right;
			free(current); //노드삭제
			return t; //자식 노드 갱신
		}
		else if(current->left != NULL && current->right == NULL){
			t = current->left;
			free(current); 
			return t;
		}
		else if(current->left == NULL && current->right == NULL){
			free(current); //노드 삭제
			return NULL; // return NULL
		}
	}

    return current;
}
BSTNode* findNode(BSTNode* current, int key){
    if(current == NULL) return NULL; //트리에 없을경우 return NULL
    if(key < current->value) //reft recursive
		return findNode(current->left, key);
    else if(key > current->value) //right recursive
		return findNode(current->right, key);
    else return current; // 노드를 찾을경우 현재 노드 리턴
}
void printInorder(BSTNode* current){
	if(current == NULL) return;
	printInorder(current->left); // left 방문
	fprintf(fout, "%d ", current->value); //현재 노드 출력
	printInorder(current->right); // right 방문
}

void deleteTree(BSTNode* current){
	if(current == NULL) return; // NULL일경우 리턴
	deleteTree(current->left); //왼쪽트리삭제
	deleteTree(current->right); //오른쪽트리삭제
	free(current); //본인삭제
}

void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char cv;
	int key;

	BSTNode* root = NULL;
	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d", &key);
				root = insertNode(root, key);
				break;
			case 'f':
				fscanf(fin,"%d",&key);
				if(findNode(root, key)){ // findNode가 Null이아니면
					fprintf(fout, "%d is in the tree\n", key);
				}else{ //NULL이면
					fprintf(fout, "%d is not in the tree\n", key);
				}
				break;
			case 'd':
				fscanf(fin, "%d", &key);
				if(findNode(root, key)){
					root = deleteNode(root, key);
				}else{
					fprintf(fout, "deletion error %d\n", key);
				}
				break;
			case 'p':
				fscanf(fin, "%c", &cv);
				if(cv == 'i'){
					if(root == NULL){
						fprintf(fout, "tree is empty");
					}else{
						printInorder(root);
					}
				}
				fprintf(fout, "\n");
				break;
		}
	}
	deleteTree(root);
}