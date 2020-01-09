#pragma once

#include <unordered_map>

#include "BaseTool.h"
#include "UmlClass.h"
#include "DiagramEdition.h"

class UmlClassDiagramTool: public BaseTool
{
	std::unordered_map<std::string, std::string> entries;
	WINDOW* window;
	//WINDOW* listWindow;
	
	void assignWindow(WINDOW*) override;

	DiagramEdition edition;

	int calculateHeight(int);
	int calculateWidth(int);
	void drawInheritanceLine(int**,unsigned int, unsigned int, int, int&, int);
	void drawDiagram();

	void printClasses();
	void printFields();
	void printInheritance();

public:
	UmlClassDiagramTool();
	~UmlClassDiagramTool();

	std::string getEntry(const std::string&) const override;
	void setEntry(const std::string&, const std::string&) override;

	void inputMode();
	void outputMode();

	void addClass();
	void editClass();
	void deleteClass();
	void addField();
	void deleteField();
	void addInheritance();
	void deleteInheritance();

	void checkEditionMode();
};