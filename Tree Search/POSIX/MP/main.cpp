#include <iostream>
#include <queue>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "Tree.h"

#define BRANCH_FACTOR 5
#define DATA_MAX 1000

using namespace std;

Tree* T;

pid_t pid[BRANCH_FACTOR];

int target;
TreeNode * pStartNode;

void fillTreeRandom();
void printTree();
void search(TreeNode*);

int main() {
    cout << "\033[1;30;44m Multi Process Tree Search v0.1 - POSIX Version \033[0m\n";

    T = new Tree;

    fillTreeRandom();
    printTree();

    cout << endl << "\033[33mPlease Enter Target : \033[0m\n";
    cin >> target ;
    cout << endl;

    clock_t tStart = clock();

    bool parent = true;

    for(int i = 0; i < BRANCH_FACTOR; i++ ) {
        printf("Creating child Process %d ...\n",i);

        pStartNode = T->root->children[i];

        pid[i] = fork();

        if (pid[i]>0) {
            //Parent , do nothing
        }else if(pid[i]==0){
            parent = false;
            break;
        }else{
            printf("Error Creating child %d ! \n",i);
            exit(-1);
        }
    }

    if(parent){
        //wait for all childs
        for(int i = 0; i < BRANCH_FACTOR; i++ ) {
            waitpid(pid[i],NULL,0);
        }

        printf("Time taken: %.3fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    }else{
        search(pStartNode);
    }

    return 0;
}

void fillTreeRandom() {
    srand(time(NULL));
    long nodecount = 200;
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

void search(TreeNode * start){
    int mypid = getpid();
    printf("Search Started in Process %d !\n",mypid);
    queue<TreeNode*> nodesToPrint;
    nodesToPrint.push(start);
    while(!nodesToPrint.empty()){
        TreeNode* node = nodesToPrint.front();
        nodesToPrint.pop();
        if(node->data==target){
            printf("\033[30;42mProcess %d : Found the Target !\033[0m\n",mypid);
            pthread_exit(NULL);
        }
        //add children
        for(int i=0;i<node->children.size();i++){
            TreeNode * c = node->children.at(i);
            nodesToPrint.push(c);
        }
    }
    cout << endl;
    printf("\033[30;41mProcess %d : No Match Found !\033[0m\n",mypid);
    pthread_exit(NULL);
}