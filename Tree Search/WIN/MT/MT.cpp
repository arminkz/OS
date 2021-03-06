#include <iostream>
#include <queue>
#include <time.h>
#include <windows.h>

#include "Monitor.h"
#include "Tree.h"

#define BRANCH_FACTOR 5
#define DATA_COUNT 1000000

using namespace std;

HANDLE threads[BRANCH_FACTOR];

Tree* T;

int target;
TreeNode * threadStartNode[BRANCH_FACTOR];

void fillTree();
void printTree();
DWORD WINAPI search(void *);

int main() {
	cout << "Multi Threaded Tree Search v0.1 - WINDOWS Version \n";

	initCPUMonitor();
	
	T = new Tree;

	fillTree();
	//printTree();

	cout << endl << "Please Enter Target : \n";
	cin >> target;
	cout << endl;

	clock_t tStart = clock();

	//search();
	for (int i = 0; i < BRANCH_FACTOR; i++) {
		printf("Creating thread %d ...\n", i);

		threadStartNode[i] = T->root->children[i];

		threads[i] = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			search,                 // thread function name
			(void*)i,               // argument to thread function 
			0,                      // use default creation flags 
			NULL);   // returns the thread identifier 

		if (threads[i] == NULL)
		{
			printf("Error Creating thread %d ! \n", i);
			exit(-1);
		}
	}

	for (int i = 0; i < BRANCH_FACTOR; i++) {
		WaitForSingleObject(threads[i], INFINITE);
	}

	printf("Time taken: %.3fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	printf("CPU Usage: %.3f \n",getCPU());
	printf("RAM Usage: %d KB \n",getRAM());
	system("PAUSE");
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
	while (!nodesToFill.empty() && !isTerminated) {
		TreeNode* node = nodesToFill.front();
		nodesToFill.pop();
		//put children
		int childcount = BRANCH_FACTOR;
		for (int i = 0;i<childcount;i++) {
			TreeNode* child = new TreeNode(n);
			node->children.push_back(child);
			nodesToFill.push(child);
			n++;
			if (n >= nodecount) {
				isTerminated = true;
				break;
			}
		}
	}
};

void printTree() {
	queue<TreeNode*> nodesToPrint;
	nodesToPrint.push(T->root);
	while (!nodesToPrint.empty()) {
		TreeNode* node = nodesToPrint.front();
		nodesToPrint.pop();
		cout << node->data << " ";
		//add children
		for (int i = 0;i<node->children.size();i++) {
			TreeNode * c = node->children.at(i);
			nodesToPrint.push(c);
		}
	}
	cout << endl;
}

DWORD WINAPI search(void * param) {
	int threadno = (long)param;
	printf("Search Started in Thread %d !\n", threadno);
	queue<TreeNode*> nodesToPrint;
	nodesToPrint.push(threadStartNode[threadno]);
	while (!nodesToPrint.empty()) {
		TreeNode* node = nodesToPrint.front();
		nodesToPrint.pop();
		if (node->data == target) {
			printf("Thread %d : Found the Target !\n", threadno);
			return 1;
		}
		//add children
		for (int i = 0;i<node->children.size();i++) {
			TreeNode * c = node->children.at(i);
			nodesToPrint.push(c);
		}
	}
	cout << endl;
	printf("Thread %d : No Match Found !\n", threadno);
	return 0;
}