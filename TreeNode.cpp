#include "TreeNode.h"

TreeNode::TreeNode()
{
	
}

TreeNode::TreeNode(std::string attrValue)
{
	this->attrValue = attrValue;
}

TreeNode::~TreeNode()
{
}

void TreeNode::addChildNode(TreeNode * child)
{
	childNodes.push_back(child);
}

void TreeNode::initNode(std::vector<std::vector<std::string>> table, std::vector<std::string> featureEdges, std::vector<std::string> attrs) {
	this->featureEdges = featureEdges;
	this->table = table;
	for (int i = 0; i < attrs.size(); ++i) { //иначе не хочет
		this->attrs.push_back(attrs[i]);
	}
}

int TreeNode::getFeaturesSize()
{
	return featureEdges.size();
}

std::vector<std::vector<std::string>> TreeNode::getSubTable(int iFeature)
{
	std::vector<std::vector<std::string>> t = std::vector<std::vector<std::string>>(table);
	int j;
	for (auto it = attrs.begin(); it != attrs.end(); ++it) {
		if (*it == attrValue) {
			j = std::distance(attrs.begin(), it);
			break;
		}
	}
	std::string feature = featureEdges.at(iFeature);
	auto it = t.at(j).begin();
	auto prev = it;
	while (it != t.at(j).end()) {
		if (*it != feature) {
			int dist = std::distance(t.at(j).begin(), it);
			for (int i = 0; i < t.size(); ++i) {
				if (i == j)
					prev = t.at(i).erase(t.at(i).begin() + dist);
				else
					t.at(i).erase(t.at(i).begin() + dist);
			}
		}
		else prev = it + 1;
		it = prev;

	}
	t.erase(t.begin()+j);
	return t;
}

std::vector<std::string> TreeNode::actualizeAttrs()
{
	std::vector<std::string> a = std::vector<std::string>(attrs);
	for (auto it = a.begin(); it != a.end(); ++it) {
		if (*it == attrValue) {
			a.erase(it);
			break;
		}
	}
	return a;
}

bool TreeNode::isLeave()
{
	if (table.size() == 1) return true;
	int n = table.size() - 1;
	std::string str = table.at(n).at(0);
	for (auto it = table.at(n).begin() + 1; it != table.at(n).end(); ++it)
		if (*it != str) return false;
	return true;
}

void TreeNode::setLeaveValue()
{
	attrValue = table[table.size() - 1][0];
}

std::vector<std::string> TreeNode::getFeatureEdges()
{
	return featureEdges;
}

TreeNode * TreeNode::getChild(int i)
{
	return childNodes[i];
}

std::string TreeNode::getAttrValue()
{
	return attrValue;
}

