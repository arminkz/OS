#include <iostream>
#include <queue>
#include <pthread.h>
#include <time.h>

#include "Tree.h"

#define BRANCH_FACTOR 5
#define DATA_MAX 100000

using namespace std;

Tree* T;
pthread_t threads[BRANCH_FACTOR];

int target;
TreeNode * threadStartNode[BRANCH_FACTOR];

void fillTreeRandom();
void printTree();
void* search(void*);

int main() {
    cout << "\033[1;30;44m Multi Threaded Tree Search v0.1 - POSIX Version \033[0m\n";

    T = new Tree;

    fillTreeRandom();
    //printTree();

    cout << endl << "\033[33mPlease Enter Target : \033[0m\n";
    cin >> target ;
    cout << endl;

    clock_t tStart = clock();

    for(int i = 0; i < BRANCH_FACTOR; i++ ) {
        printf("Creating thread %d ...\n",i);

        threadStartNode[i] = T->root->children[i];

        int rc = pthread_create(&threads[i], NULL, search, (void *)i);

        if (rc) {
            printf("Error Creating thread %d ! \n",i);
            exit(-1);
        }
    }

    for(int i = 0; i < 5; i++ ) {
        pthread_join(threads[i],NULL);
    }

    printf("Time taken: %.3fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    return 0;
}

void fillTreeRandom() {
    srand(time(NULL));
    long nodecount = 200000;
    cout << "Creating Tree with " << nodecount << " nodes ..." << endl;
    queue<TreeNode*> nodesToFill;
    T->root = new TreeNode(rand() % DATA_MAX + 1);
    nodesToFill.push(T->root);
    int n = 1;
    bool isTerminated = false;
    while(!nodesToFill.empty() && !isTerminated){
        TreeNode* node = nodesToFill.front();
        nodesToFill.pop();
        //put children
        int childcount = BRANCH_FACTOR;
        for(int i=0;i<childcount;i++){
            TreeNode* child = new TreeNode(rand() % DATA_MAX + 1);
            node->children.push_back(child);
            nodesToFill.push(child);
            n++;
            if(n>=nodecount){
               isTerminated = true;
                break;
            }
        }
    }
};

void printTree(){
    queue<TreeNode*> nodesToPrint;
    nodesToPrint.push(T->root);
    while(!nodesToPrint.empty()){
        TreeNode* node = nodesToPrint.front();
        nodesToPrint.pop();
        cout << node->data << " ";
        //add children
        for(int i=0;i<node->children.size();i++){
            TreeNode * c = node->children.at(i);
            nodesToPrint.push(c);
        }
    }
    cout << endl;
}

void *search(void *i){
    int threadno = (long)i;
    printf("Search Started in Thread %d !\n",threadno);
    queue<TreeNode*> nodesToPrint;
    nodesToPrint.push(threadStartNode[threadno]);
    while(!nodesToPrint.empty()){
        TreeNode* node = nodesToPrint.front();
        nodesToPrint.pop();
        if(node->data==target){
            printf("\033[30;42mThread %d : Found the Target !\033[0m\n",threadno);
            pthread_exit(NULL);
        }
        //add children
        for(int i=0;i<node->children.size();i++){
            TreeNode * c = node->children.at(i);
            nodesToPrint.push(c);
        }
    }
    cout << endl;
    printf("\033[30;41mThread %d : No Match Found !\033[0m\n",threadno);
    pthread_exit(NULL);
}