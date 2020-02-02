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
	WINDOW* listWindow;
	WINDOW* diagramWindow;
	WINDOW* errorWindow;

	void assignWindow(WINDOW*) override;

	DiagramEdition edition;
	ErrorAlert alert;

	int counter;
	int index;

	void setCounter(std::string, int);
	void setIndex(int);

	std::string lastFileName; 

	int calculateHeight(int);
	int calculateWidth(int);
	void drawInheritanceLine(int**, size_t, size_t, int, int&, int);
	void drawDiagram();


public:
	UmlClassDiagramTool();
	~UmlClassDiagramTool();

	std::string getEntry(const std::string&) const override;
	void setEntry(const std::string&, const std::string&) override;

	std::function<void()> editionHandler;
	void defaultEditionHandler();

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