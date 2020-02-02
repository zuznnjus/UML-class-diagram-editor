#include <curses.h>
#include "UmlClassDiagramTool.h"
#include "BaseBackend.h"
#include "MceditBackend.h"

int main()
{
	UmlClassDiagramTool *tool = new UmlClassDiagramTool();
	MceditBackend* backend = new MceditBackend(tool);
	backend->bind(".File.To read${File name:|fileName}", [&tool]() {tool->inputMode(); }, "Open file");
	backend->bind(".File.To write${File name:|fileName}", [&tool]() {tool->outputMode(); }, "Save file");
	backend->bind(".Edition mode.classes", [&tool]() {tool->printClasses(); tool->editionHandler = std::bind(&UmlClassDiagramTool::chooseClass, tool); }, "Choose class to edition");
	backend->bind(".Edition mode.fields", [&tool]() { tool->printFields(); tool->editionHandler = std::bind(&UmlClassDiagramTool::chooseField, tool); }, "Choose field to edition");
	backend->bind(".Edition mode.inheritance", [&tool]() { tool->printInheritance(); tool->editionHandler = std::bind(&UmlClassDiagramTool::chooseInheritance, tool); }, "Choose inheritance to edition");
	backend->bind(".Class.add class${Class name: |className}", [&tool]() {tool->addClass(); }, "Add class");
	backend->bind(".Class.edit class${New name: |className}", [&tool]() {tool->editClass(); }, "Edit class name");
	backend->bind(".Class.delete class", [&tool]() {tool->deleteClass(); }, "Delete class");
	backend->bind(".Field.add field${Field name: |fieldName}", [&tool]() {tool->addField(); }, "Add field");
	backend->bind(".Field.edit field${New name: |fieldName}", [&tool]() {tool->editField(); }, "Edit field name");
	backend->bind(".Field.delete field", [&tool]() {tool->deleteField();}, "Delete field");
	backend->bind(".Inheritance.add inheritance${Base class name: |inheritedFrom}", [&tool]() {tool->addInheritance(); }, "Add inheritance");
	backend->bind(".Inheritance.delete inheritance", [&tool]() {tool->deleteInheritance();  }, "Delete inheritance");
	backend->bind("<EDITION>", [&tool]() {if (tool->editionHandler) tool->editionHandler(); else tool->defaultEditionHandler(); }, "");
	backend->start();
	delete backend;
}