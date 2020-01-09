#include "DiagramEdition.h"

void DiagramEdition::addClass()
{
	vecClasses.push_back(new UmlClass(tool->getEntry("className")));
}

void DiagramEdition::editClass()
{
	std::string newClassName = tool->getEntry("className");
	std::string oldClassName = tool->getEntry("editedClassName");
	auto it = vecClasses.begin();
	for (; it != vecClasses.end() && (*it)->name != oldClassName; ++it);
	if (it == vecClasses.end()) {
		//nie znaleziono
		//throw
		return;
	}
	else {
		(*it)->name = newClassName;
	}
}

void DiagramEdition::deleteClass()
{
	std::string className = tool->getEntry("className"); 
	auto it = vecClasses.begin();
	for (; it != vecClasses.end() && (*it)->name != className; ++it);
	if (it == vecClasses.end()) {
		//nie znaleziono
		//throw
		return;
	}
	else {
		delete* it;
		vecClasses.erase(it);
	}
}

void DiagramEdition::addField()
{
	std::string editedClassName = tool->getEntry("editedClassName");
	auto it = vecClasses.begin();
	for (; it != vecClasses.end() && (*it)->name != editedClassName; ++it);
	if (it == vecClasses.end()) {
		//nie znaleziono
		//throw
		return;
	}
	else {
		(*it)->fields.push_back(tool->getEntry("fieldName"));
	}
}

void DiagramEdition::editField()
{
}

void DiagramEdition::deleteField()
{
	std::string editedClassName = tool->getEntry("editedClassName");
	std::string fieldName = tool->getEntry("fieldName");
	auto it = vecClasses.begin();
	for (; it != vecClasses.end() && (*it)->name != editedClassName; ++it);
	if (it == vecClasses.end()) {
		//nie znaleziono
		//throw
		return;
	}
	else {
		auto it2 = (*it)->fields.begin();
		for (; it2 != (*it)->fields.end() && it2->name != fieldName; ++it2);
		if (it2 == (*it)->fields.end()) {
			//nie znaleziono
			//throw
			return;
		}
		else {
			(*it)->fields.erase(it2);
		}
	}
}

void DiagramEdition::addInheritance()
{
	std::string editedClassName = tool->getEntry("editedClassName");
	auto it = vecClasses.begin();
	for (; it != vecClasses.end() && (*it)->name != editedClassName; ++it);
	if (it == vecClasses.end()) {
		//nie znaleziono
		//throw
		return;
	}
	else {
		(*it)->inheritedFrom.push_back(tool->getEntry("inheritedFrom"));
	}
}

void DiagramEdition::editInheritance()
{
}

void DiagramEdition::deleteInheritance()
{
	std::string editedClassName = tool->getEntry("editedClassName");
	std::string inheritedFrom = tool->getEntry("inheritedFrom");
	auto it = vecClasses.begin();
	for (; it != vecClasses.end() && (*it)->name != editedClassName; ++it);
	if (it == vecClasses.end()) {
		//nie znaleziono
		//throw
		return;
	}
	else {
		for (int i = 0; i < (*it)->inheritedFrom.size() && (*it)->inheritedFrom[i] == inheritedFrom; i++)
			(*it)->inheritedFrom.erase((*it)->inheritedFrom.begin()+i);
	}
}
