#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>


typedef struct Node {
    int key;
    struct Node* left;
    struct Node* right;
    int height; /
} Node;

// 노드 생성 
Node* createNode(int key) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->left = newNode->right = NULL;
    newNode->height = 1; // AVL 트리 초기 높이 설정
    return newNode;
}

// 트리 메모리 해제
void freeTree(Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// 이진 탐색 트리 삽입 함수
Node* insertBST(Node* root, int key) {
    if (root == NULL) return createNode(key);
    if (key < root->key)
        root->left = insertBST(root->left, key);
    else if (key > root->key)
        root->right = insertBST(root->right, key);
    return root;
}

// AVL 트리 삽입 함수
int height(Node* node) {
    return (node == NULL) ? 0 : node->height;
}
int getBalance(Node* node) {
    if (node == NULL) return 0;
    return height(node->left) - height(node->right);
}
Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));
    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));
    return x;
}
Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));
    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));
    return y;
}
Node* insertAVL(Node* root, int key) {
    if (root == NULL) return createNode(key);
    if (key < root->key)
        root->left = insertAVL(root->left, key);
    else if (key > root->key)
        root->right = insertAVL(root->right, key);
    else
        return root;

    root->height = 1 + (height(root->left) > height(root->right) ? height(root->left) : height(root->right));
    int balance = getBalance(root);
    if (balance > 1 && key < root->left->key)
        return rotateRight(root);
    if (balance < -1 && key > root->right->key)
        return rotateLeft(root);
    if (balance > 1 && key > root->left->key) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (balance < -1 && key < root->right->key) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }
    return root;
}

// 이진 탐색 트리 탐색
int searchBST(Node* root, int key, int* compareCount) {
    while (root != NULL) {
        (*compareCount)++;
        if (key == root->key) return 1;
        else if (key < root->key) root = root->left;
        else root = root->right;
    }
    return 0;
}

// AVL 트리 탐색
int searchAVL(Node* root, int key, int* compareCount) {
    return searchBST(root, key, compareCount);
}

// AVL 트리 실험
void doAVBatch(Node* root, int* compareCount, int* searchCount) {
    for (int i = 0; i < 2000; i++) {
        int A = rand() % 3; // 0~2 랜덤
        int B = rand() % 1000; // 0~999 랜덤
        if (A == 0) {
            root = insertAVL(root, B);
        }
        else if (A == 1) {
            // AVL 삭제 생략
        }
        else {
            searchAVL(root, B, compareCount);
            (*searchCount)++;
        }
    }
    freeTree(root);
}

// 이진 탐색 트리 실험
void doBinaryBatch(Node* root, int* compareCount, int* searchCount) {
    for (int i = 0; i < 2000; i++) {
        int A = rand() % 3; // 0~2 랜덤
        int B = rand() % 1000; // 0~999 랜덤
        if (A == 0) {
            root = insertBST(root, B);
        }
        else if (A == 1) {
            // BST 삭제 생략
        }
        else {
            searchBST(root, B, compareCount);
            (*searchCount)++;
        }
    }
    freeTree(root);
}

int main() {
    Node* root = NULL;
    srand(time(NULL)); // 랜덤 시드 초기화

    //3번 반복
    for (int i = 1; i <= 3; i++) {
       

        // AVL 트리 실험
        int compareCount = 0, searchCount = 0;
        float allCount = 0.0;

        doAVBatch(root, &compareCount, &searchCount);
        allCount = (float)compareCount / searchCount;
        printf("average AVL compare count: %.2f\n", allCount);

        // AVL 트리 초기화
        root = NULL;
        compareCount = 0;
        searchCount = 0;
        allCount = 0.0;

        // 이진 탐색 트리 실험
        doBinaryBatch(root, &compareCount, &searchCount);
        allCount = (float)compareCount / searchCount;
        printf("average Bin compare count: %.2f\n\n", allCount);

        Sleep(1000); // 1초 간격으로 다음 실험 진행
    }

    return 0;
}