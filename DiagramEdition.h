#pragma once
#include <vector>
#include "UmlClass.h"
#include "BaseTool.h"


class DiagramEdition
{
	BaseTool *const tool;
	std::vector<UmlClass*> vecClasses;

	void addClass();
	void editClass();
	void deleteClass();
	void addField();
	void editField();
	void deleteField();
	void addInheritance();
	void deleteInheritance();

	friend class UmlClassDiagramTool;

public:
	DiagramEdition(BaseTool *const tool) : tool(tool) {}
	std::vector<UmlClass*>& getVec()  { return vecClasses; }
};