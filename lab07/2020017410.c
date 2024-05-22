#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct AVLTreeNode AVLNode;
typedef int Element;

struct AVLTreeNode{
	Element element;
	AVLNode* left;
	AVLNode* right;
	int height;
};

int maxx(int a, int b){ // max함수
    if(a > b) return a;
    else return b;
}

int getHeight(AVLNode* node){ // node->height 리턴(노드가 없을 시 height = -1)
    if(node == NULL) return -1; 
    else return node->height;
}

void updateHeight(AVLNode* node){ //height가 왼쪽 자식노드와 오른쪽 자식노드의 max보다 +1 이 되도록 업데이트
    if(node == NULL) return;
    else node->height = maxx(getHeight(node->left), getHeight(node->right)) + 1;
}

int calculateBF(AVLNode* node){ // BF 계산
    int bf = 0;
    if(getHeight(node->left) > getHeight(node->right))
        bf = getHeight(node->left) - getHeight(node->right);
    else bf = getHeight(node->right) - getHeight(node->left);
    return bf; 
}

AVLNode* RotateRight(AVLNode* node){ // Right Rotation
    AVLNode* L;
    L = node->left;
    node->left = L->right;
    L->right = node;
    updateHeight(L); // L의 Rotation 이후 Height갱신
    updateHeight(node); // node Rotation 후 Height 갱신
    return L;
}

AVLNode* RotateLeft(AVLNode* node){
    AVLNode* R;
    R = node->right;
    node->right = R->left;
    R->left = node;
    updateHeight(R);
    updateHeight(node); // Rotation 이후 height 갱신
    return R;
}

AVLNode* rebalance(AVLNode* T){ //rebalance
    AVLNode* node = NULL;
    if(getHeight(T->left) > getHeight(T->right) + 1){ //L Case
        node = T->left;
        if(getHeight(node->left) < getHeight(node->right)) T->left = RotateLeft(node); //LR Case
        T = RotateRight(T); //LL Case or LR Case
    }
    if(getHeight(T->left) + 1 < getHeight(T->right)){ // R Case
        node = T->right;
        if(getHeight(node->left) > getHeight(node->right)) T->right = RotateRight(node); //RL Case
        T = RotateLeft(T); // RR Case of RL Case
    }
    updateHeight(T);
    updateHeight(node); // height 갱신
    return T;
}


AVLNode* Insert(Element X, AVLNode* T){
    if(T == NULL){ //NULL이면 AVLNode 생성 후 반환
        T = (AVLNode*)malloc(sizeof(AVLNode));
        T->element = X;
        T->left = NULL;
        T->right = NULL;
        T->height = 0;
        return T;
    }
    if(T->element != X){
        if(T->element > X){
            T->left = Insert(X, T->left); // 왼쪽 노드로 X의 위치 찾아가기
            if(calculateBF(T) >= 2) T = rebalance(T); // rebalance
        }
        if(T->element < X){
            T->right = Insert(X, T->right); // 오른쪽 노드로 X의 위치 찾아가기
            if(calculateBF(T) >= 2) T = rebalance(T); // rebalance
        }
    }
    else if(T->element == X)
        fprintf(fout, "insertion error : %d is already in the tree\n", X);
    updateHeight(T); // height 갱신
    return T;
}

AVLNode* Delete(Element X, AVLNode* T){
    AVLNode* t = NULL;
    if(T == NULL){
        fprintf(fout, "deletion error : %d is not in the tree\n", X);
        return T;
    }  //return NULL
    else if(X < T->element){
		T->left = Delete(X, T->left); //delete할 노드 찾아가기 left recursive
        if(calculateBF(T) >= 2) T = rebalance(T);
    }
    else if(X > T->element){
		T->right = Delete(X, T->right); //delete할 노드 찾아가기 right recursive
        if(calculateBF(T) >= 2) T = rebalance(T);
    }
    else{ // delete할 노드를 발견
		if(T->left != NULL && T->right != NULL){ //자식이 둘일경우
			t = T->right; // 오른쪽 노드의 가장 왼쪽 자식노드 찾기
			while(t->left != NULL) t = t->left;
			T->element = t->element; //현재노드에 찾은 노드의 value를 대입
			T->right = Delete(T->element, T->right); //오른쪽 노드를 올리기
		}
		else if(T->left == NULL && T->right != NULL){
			t = T->right;
			free(T); //노드삭제
			return t; //자식 노드 갱신
		}
		else if(T->left != NULL && T->right == NULL){
			t = T->left;
			free(T); 
			return t;
		}
		else if(T->left == NULL && T->right == NULL){
			free(T); //노드 삭제
			return NULL; // return NULL
		}
	}
    updateHeight(T); // height 갱신
    return T;
}

void PrintInorder(AVLNode* T){
    //inorder로 출력
    if(T == NULL) return;
	PrintInorder(T->left); // left 방문
	fprintf(fout, "%d(%d) ", T->element, T->height + 1); //현재 노드 출력
	PrintInorder(T->right); // right 방문
}

void DeleteTree(AVLNode* T){
    if(T == NULL) return;
    DeleteTree(T->left);
    DeleteTree(T->right);
    free(T); //postorder로 free
}

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	AVLNode* Tree = NULL;
	char cv;
	int key;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d\n", &key);
				Tree = Insert(key, Tree);
				break;
			case 'd':
				fscanf(fin, "%d\n", &key);
				Tree = Delete(key, Tree);
		}
		PrintInorder(Tree);
		fprintf(fout, "\n");
	}

	DeleteTree(Tree);
	fclose(fin);
	fclose(fout);

	return 0;
}
