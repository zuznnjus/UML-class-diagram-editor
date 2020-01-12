#pragma once

#include <unordered_map>
#include <functional>

#include "BaseTool.h"
#include "UmlClass.h"
#include "DiagramEdition.h"
#include "ErrorAlert.h"

class UmlClassDiagramTool: public BaseTool
{
	std::unordered_map<std::string, std::string> entries;
	WINDOW* window;
	//WINDOW* listWindow;
	
	void assignWindow(WINDOW*) override;

	DiagramEdition edition;
	ErrorAlert alert;

	int counter;
	int index;

	void setCounter(std::string, int);
	void setIndex(int);

	int calculateHeight(int);
	int calculateWidth(int);
	void drawInheritanceLine(int**,unsigned int, unsigned int, int, int&, int);
	void drawDiagram();

public:
	UmlClassDiagramTool();
	~UmlClassDiagramTool();

	std::string getEntry(const std::string&) const override;
	void setEntry(const std::string&, const std::string&) override;

	std::function<void()> editionHandler;

	void inputMode();
	void outputMode();

	void addClass();
	void editClass();
	void deleteClass();
	void addField();
	void editField();
	void deleteField();
	void addInheritance();
	void deleteInheritance();

	void chooseClass();
	void chooseField();
	void chooseInheritance();

	void printClasses();
	void printFields();
	void printAllFields();
	void printInheritance();
	void printAllInheritance();
};