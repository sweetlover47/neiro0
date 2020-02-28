#pragma once
#include<string>
#include <vector>
#include <map>
#include <math.h>
#define attrCount 5

class TreeNode
{
public:
	TreeNode();
	TreeNode(std::string attrValue);
	virtual ~TreeNode();
	void addChildNode(TreeNode *child);
	void initNode(std::vector<std::vector<std::string>> table, std::vector<std::string> featureEdges, std::vector<std::string> attrs);
	int getFeaturesSize();
	std::vector<std::vector<std::string>> getSubTable(int iFeature);
	std::vector<std::string> actualizeAttrs();
	bool isLeave();
	void setLeaveValue();
	std::vector<std::string> getFeatureEdges();
	TreeNode* getChild(int i);
	std::string getAttrValue();
private:
	std::vector<std::string> featureEdges;
	std::vector<std::vector<std::string>> table;
	std::vector<std::string> attrs;
	std::string attrValue;
	std::vector<TreeNode*> childNodes;
};

