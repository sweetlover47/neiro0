
#include <math.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>
#include "DecisionTree.h"

DecisionTree* createTree(std::vector<std::string> attrs, std::vector<std::vector<std::string>> examples) {
	DecisionTree* dt = new DecisionTree();
	dt->createRootNode(dt->chooseNewAttr(attrs, examples));
	return dt;
}

void constructTree(int i, DecisionTree* dt, TreeNode* node) {
	std::vector<std::vector<std::string>> subtable = node->getSubTable(i);
	if (node->isLeave()) {
		node->setLeaveValue();
		return;
	}
	std::vector<std::string> attrs = node->actualizeAttrs();
	TreeNode* childNode = dt->chooseNewAttr(attrs, subtable);
	node->addChildNode(childNode);
	for (int i = 0; i < childNode->getFeaturesSize(); ++i) {
		if (childNode->isLeave()) {
			childNode->setLeaveValue(); //����� � ��, � �� - ������� ������ ����������. �� ����, ����� ��������� �����������
			return;
		}
		else
			constructTree(i, dt, childNode); //�������� ����������� �������
	}
}

int main(int argc, char* argv[]) {
	std::vector<std::string> attrs;
	std::vector<std::vector<std::string>> examples;
	for (int i = 0; i < attrCount; ++i) {
		examples.push_back(std::vector<std::string>());
	}
	char buf[80];
	std::ifstream fin("dataset.txt");
	for (int i = 0; i < attrCount; ++i) {	// attr title
		fin >> buf;
		attrs.push_back(buf);
	}
	int i = 0;
	for (buf; fin >> buf;) {				// read examples for each title
		examples.at(i).push_back(buf);
		i = ++i % attrCount;
	}
	DecisionTree* dt = createTree(attrs, examples);
	int featuresCount = dt->root->getFeaturesSize();
	for (int i = 0; i < featuresCount; ++i) {
		constructTree(i, dt, dt->root);
	}

	std::string userAttrs[attrCount - 1];
	for (int i = 0; i < attrCount - 1; ++i) {
		std::cin >> userAttrs[i];
	}
	std::cout << dt->makeDecision(userAttrs) << std::endl;
	system("pause");
	return 0;
}

