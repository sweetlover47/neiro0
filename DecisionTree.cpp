#include "DecisionTree.h"
DecisionTree::DecisionTree()
{
	root = NULL;
}

DecisionTree::~DecisionTree()
{
}

void DecisionTree::addNode(TreeNode * curNode, TreeNode * addedNode)
{

}

void DecisionTree::createRootNode(TreeNode * rootNode)
{
	root = rootNode;
}

void DecisionTree::removeNode(TreeNode * removedNode)
{
	//?
}

std::string DecisionTree::makeDecision(std::string userAttrs[attrCount - 1])
{
	int j = 0, k = 0;
	TreeNode* node = root;
	std::string str;
	while (j < attrCount - 1) {
		std::vector<std::string> edges = node->getFeatureEdges();
		for (int i = 0; i < edges.size(); ++i)
			if (edges[i] == userAttrs[k]) {
				node = node->getChild(i);
				break;
			}
		str = node->getAttrValue();
		if (str == "Outlook") k = 0;
		else if (str == "Temperature") k = 1;
		else if (str == "Humidity") k = 2;
		else if (str == "Wind") k = 3;
		else if (str == "Yes" || str == "No") return str;
		++j;
	}

	return str;
}

TreeNode* DecisionTree::chooseNewAttr(std::vector<std::string> attrs, std::vector<std::vector<std::string>> features) {
	const int attrSize = features.size();
	std::map<std::string, int> uniqClass = countUniqTypes(features.at(attrSize - 1)); //uniq classes and its count
	int n_S = features.at(0).size(); //count of raws
	float* p_S = countClassesProbability(uniqClass, n_S);
	float entropy = calculateEntropy(p_S, uniqClass.size());
	float* gainRatio = new float[attrSize - 1];
	for (int i = 0; i < attrSize - 1; ++i) {
		gainRatio[i] = calculateGainRatio(features.at(i), features.at(attrSize-1), uniqClass, entropy);
	}
	float* max_attr = std::max_element(gainRatio, gainRatio + attrSize);
	std::string newAttr = attrs[std::distance(gainRatio, max_attr)];
	
	std::map<std::string, int> m = countUniqTypes(features.at(std::distance(gainRatio, max_attr)));
	std::vector<std::string> uniqFeatureEdges;
	for (auto it = m.begin(); it != m.end(); ++it) {
		uniqFeatureEdges.push_back(it->first);
	}
	TreeNode* newNode = new TreeNode(newAttr);
	newNode->initNode(features, uniqFeatureEdges, attrs);

	return newNode;
}

float DecisionTree::calculateGainRatio(std::vector<std::string> attr, std::vector<std::string> decision, std::map<std::string, int> uniqClass, float entropy) {
	std::map<std::string, int> uniqFeatures = countUniqTypes(attr);
	float gain = calculateGain(attr, decision, uniqFeatures, uniqClass, entropy);
	float splitinfo = calculateSplitInfo(attr, uniqFeatures);
	return gain / splitinfo;
}

float DecisionTree::calculateSplitInfo(std::vector<std::string> attr, std::map<std::string, int> uniqFeatures) {
	float split = 0.0;
	for (auto it = uniqFeatures.begin(); it != uniqFeatures.end(); ++it) {
		float p = (float)it->second / attr.size();
		split -= (p * log2(p));
	}
	return split;
}

float DecisionTree::calculateGain(std::vector<std::string> attr, std::vector<std::string> decision, std::map<std::string, int> uniqFeatures, std::map<std::string, int> uniqClass, float entropy) {
	float** p_Si = countFeaturesProbability(uniqFeatures, uniqClass, attr, decision);
	float* featuresEntropy = new float[uniqFeatures.size()];
	for (int i = 0; i < uniqFeatures.size(); ++i) //count Entropy(Decision|Wind = i)
		featuresEntropy[i] = calculateEntropy(p_Si[i], uniqClass.size()); //calculate entropy for each feature
	float gain = entropy;
	int i = 0;
	for (auto it = uniqFeatures.begin(); it != uniqFeatures.end(); ++it, ++i) {
		gain -= (((float) it->second / attr.size()) * featuresEntropy[i]);
	}
	return gain; //result is correct, checked
}

float DecisionTree::calculateEntropy(float* p_S, int n) {
	float entropy = 0.0; //из другого источника энтропия считается суммой по классам, а не по аттрибутам
	for (int i = 0; i < n; ++i) {
		if (p_S[i] != 0)
			entropy += (p_S[i] * log2(p_S[i])); //else log2()=0, entropy has not changed
	}
	entropy *= (-1);
	return entropy;
}

float** DecisionTree::countFeaturesProbability(std::map<std::string, int> uniq, std::map<std::string, int> uniqClass, std::vector<std::string> attr, std::vector<std::string> decision) {
	//для каждого элемента из uniq находим количество строк с первым классом, со вторым классом и тд
	float** p_Si = new float*[uniq.size()]();
	for (int i = 0; i < uniq.size(); ++i)
		p_Si[i] = new float[uniqClass.size()]();
	int i = 0;
	for (auto it = uniq.begin(); it != uniq.end(); ++it, ++i) {
		for (auto attr_it = attr.begin(), dec_it = decision.begin(); attr_it != attr.end(); ++attr_it, ++dec_it) {
			if (it->first == (*attr_it)) {
				int index = std::distance(uniqClass.begin(), uniqClass.find(*dec_it));
				p_Si[i][index] += 1;
			}
		}
		for (int j = 0; j < uniqClass.size(); ++j) { //теперь когда посчитали количество классов (YES, NO) для каждого признака, делим на общее количество примеров этого признака (YES+NO)
			p_Si[i][j] = p_Si[i][j] / it->second;
		}
	}
	return p_Si;
}

float* DecisionTree::countClassesProbability(std::map<std::string, int> classes, int n_S) {
	float* p_S = (float*)calloc(classes.size(), sizeof(float));
	int i = 0;
	for (auto it = classes.begin(); it != classes.end(); ++it) {
		p_S[i++] = (float)it->second / n_S;
	}
	return p_S;
}

std::map<std::string, int> DecisionTree::countUniqTypes(std::vector<std::string> typeSet) { //count classes or count features
	std::map<std::string, int> uniqFeatures;
	std::map<std::string, int>::iterator it;
	for (size_t i = 0; i < typeSet.size(); ++i) {
		if ((it = uniqFeatures.find(typeSet.at(i))) == uniqFeatures.end())	//if uniqFeat not contains feature from class then add to uniqFeat
			uniqFeatures.insert(std::pair<std::string, int>(typeSet.at(i), 1));
		else
			uniqFeatures[typeSet.at(i)] = ++(it->second);					//else increment count of the class features
	}
	return uniqFeatures;
}
