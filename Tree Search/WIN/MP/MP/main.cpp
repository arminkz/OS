#include <iostream>
#include <queue>
#include <time.h>
#include <windows.h>
#include <string>

#include "Tree.h"

#define BRANCH_FACTOR 5
#define DATA_MAX 1000

using namespace std;

Tree* T;

STARTUPINFO si;
PROCESS_INFORMATION pi[BRANCH_FACTOR];

TreeNode * pStartNode;

void fillTree();
void printTree();
void search(TreeNode*);

int id = -1;
int target = 0;

string ExePath() {
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	return string(buffer);
}

int main(int argc , char* argv[]) {
	
	T = new Tree;
	fillTree();

	if (argc != 3)
	{
		cout << "Multi Process Tree Search v0.1 - Windows Version" << endl;

		cout << "Creating Tree... " << endl;

		printf("Starting as [PARENT]  \nPlease Enter Target : ");
		cin >> target;
		cout << endl;

		clock_t tStart = clock();

		for (int i = 0; i < BRANCH_FACTOR; i++) {
			printf("Creating child Process %d ...\n", i);

			string cmdline = ExePath() + " " + to_string(i) + " " + to_string(target);

			// Start the child process. 
			if (!CreateProcess(NULL,   // No module name (use command line)
				const_cast<char *>(cmdline.c_str()),        // Command line 
				NULL,           // Process handle not inheritable
				NULL,           // Thread handle not inheritable
				FALSE,          // Set handle inheritance to FALSE
				0,              // No creation flags
				NULL,           // Use parent's environment block
				NULL,           // Use parent's starting directory 
				&si,            // Pointer to STARTUPINFO structure
				&pi[i])           // Pointer to PROCESS_INFORMATION structure
				)
			{
				printf("Error Creating child %d : \n", i);
				printf("CreateProcess failed (%d).\n", GetLastError());
				system("PAUSE");
				return -1;
			}

		}


		for (int i = 0; i < BRANCH_FACTOR; i++) {
			WaitForSingleObject(&pi[i], INFINITE);
		}

		printf("Time taken: %.3fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
		system("PAUSE");
	}
	else {

		id = stoi(argv[1]);
		target = stoi(argv[2]);
		
		pStartNode = T->root->children[id];
		search(pStartNode);
	}

	return 0;
}

void fillTree() {
	long nodecount = 200;
	//cout << "Creating Tree with " << nodecount << " nodes ..." << endl;
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

void search(TreeNode * start) {
	//printf("Search Started in Process %d !\n", id);
	queue<TreeNode*> nodesToPrint;
	nodesToPrint.push(start);
	while (!nodesToPrint.empty()) {
		TreeNode* node = nodesToPrint.front();
		nodesToPrint.pop();
		if (node->data == target) {
			printf("Process %d : Found the Target ! \n", id);
			//system("PAUSE");
			return;
		}
		//add children
		for (int i = 0;i<node->children.size();i++) {
			TreeNode * c = node->children.at(i);
			nodesToPrint.push(c);
		}
	}

	return;
}