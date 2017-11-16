#include <iostream>
#include <queue>
#include <time.h>

#include "Monitor.h"
#include "Tree.h"

#define BRANCH_FACTOR 5
#define DATA_COUNT 1000000

using namespace std;

Tree* T;

int target;

void fillTree();
void printTree();
void search();

int main() {
    cout << "Single Threaded Tree Search v0.1 - WINDOWS Version \n";

	initCPUMonitor();
	
    T = new Tree;

    fillTree();
    //printTree();

    cout << endl << "Please Enter Target :";
    cin >> target ;
    cout << endl;

    clock_t tStart = clock();

    search();

    printf("Time taken: %.3fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
	printf("CPU Usage: %.3f \n",getCPU());
	printf("RAM Usage: %d KB \n",getRAM());
    return 0;
}

void fillTree() {
    long nodecount = DATA_COUNT;
    cout << "Creating Tree with " << nodecount << " nodes ..." << endl;
    queue<TreeNode*> nodesToFill;
    T->root = new TreeNode(0);
    nodesToFill.push(T->root);
    int n = 1;
    bool isTerminated = false;
    while(!nodesToFill.empty() && !isTerminated){
        TreeNode* node = nodesToFill.front();
        nodesToFill.pop();
        //put children
        int childcount = BRANCH_FACTOR;
        for(int i=0;i<childcount;i++){
            TreeNode* child = new TreeNode(n);
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

void search(){
    printf("Search Started in main() !\n");
    queue<TreeNode*> nodesToPrint;
    nodesToPrint.push(T->root);
    while(!nodesToPrint.empty()){
        TreeNode* node = nodesToPrint.front();
        nodesToPrint.pop();
        if(node->data==target){
            printf("main() : Found the Target !\n");
            return;
        }
        //add children
        for(int i=0;i<node->children.size();i++){
            TreeNode * c = node->children.at(i);
            nodesToPrint.push(c);
        }
    }
    cout << endl;
    printf("main() : No Match Found !\n");
    return;
}
