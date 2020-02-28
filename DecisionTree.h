#pragma once
#include "TreeNode.h"
#include <algorithm>
#include <iterator>
class DecisionTree
{
public:
	TreeNode* root;
	DecisionTree();
	virtual ~DecisionTree();

	void addNode(TreeNode* curNode, TreeNode* addedNode);
	void createRootNode(TreeNode* rootNode);
	void removeNode(TreeNode* removedNode);
	std::string makeDecision(std::string userAttrs[attrCount - 1]);
	TreeNode* chooseNewAttr(std::vector<std::string> attrs, std::vector<std::vector<std::string>> features);
private:
	float calculateGainRatio(std::vector<std::string> attr, std::vector<std::string> decision, std::map<std::string, int> uniqClass, float entropy);
	float calculateSplitInfo(std::vector<std::string> attr, std::map<std::string, int> uniqFeatures);
	float calculateGain(std::vector<std::string> attr, std::vector<std::string> decision, std::map<std::string, int> uniqFeatures, std::map<std::string, int> uniqClass, float entropy);
	float calculateEntropy(float * p_S, int n);
	float** countFeaturesProbability(std::map<std::string, int> uniq, std::map<std::string, int> uniqClass, std::vector<std::string> attr, std::vector<std::string> decision);
	float* countClassesProbability(std::map<std::string, int>, int); // P(Cj) = N(CjS)/N(S)
	std::map<std::string, int> countUniqTypes(std::vector<std::string> classes);
};

