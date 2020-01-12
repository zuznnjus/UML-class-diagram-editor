#pragma once

#include "UmlClass.h"
#include <vector>

class ReadWriteFile
{
public:
	std::vector<UmlClass*> readFromFile(const std::string &fileName);
	void writeToFile(const std::string &fileName, const std::vector<UmlClass*> &vecClasses);

	class FileNotFound {};
};

