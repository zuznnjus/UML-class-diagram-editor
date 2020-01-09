#pragma once
#include <string>
#include <vector>
#include "UmlField.h"
class UmlClass
{
public:
	std::string name;
	std::vector<UmlField> fields;
	std::vector<std::string> inheritedFrom;

	UmlClass();
	UmlClass(std::string name);
	~UmlClass();
};

