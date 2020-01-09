#pragma once
#include <vector>
#include "UmlClass.h"
#include "BaseTool.h"


class DiagramEdition
{
	BaseTool *const tool;
	std::vector<UmlClass*> vecClasses;

public:
	DiagramEdition(BaseTool *const tool) : tool(tool) {}

	std::vector<UmlClass*>& getVec()  { return vecClasses; }

	void addClass();
	void editClass();
	void deleteClass();
	void addField();
	void editField();
	void deleteField();
	void addInheritance();
	void editInheritance();
	void deleteInheritance();

};