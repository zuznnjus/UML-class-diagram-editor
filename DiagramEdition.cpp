#include "DiagramEdition.h"

void DiagramEdition::addClass()
{
	std::string className = tool->getEntry("className");
	if (className == "") {
		throw EmptyItemName();
	}
	vecClasses.push_back(new UmlClass(className));
}

void DiagramEdition::editClass()
{
	std::string newClassName = tool->getEntry("className");
	std::string chosenClass = tool->getEntry("chosenClass");
	
	auto it = vecClasses.begin();
	for (; it != vecClasses.end() && (*it)->name != chosenClass; ++it);
	if (it == vecClasses.end()) {
		return;
	}
	else {
		(*it)->name = newClassName;
	}
}

void DiagramEdition::deleteClass()
{
	std::string chosenClass=tool->getEntry("chosenClass");

	auto it = vecClasses.begin();
	for (; it != vecClasses.end() && (*it)->name != chosenClass; ++it);
	if (it == vecClasses.end()) {
		return;
	}
	else {
		delete* it;
		vecClasses.erase(it);
	}
}

void DiagramEdition::addField()
{
	std::string chosenClass = tool->getEntry("chosenClass");

	auto it = vecClasses.begin();
	for (; it != vecClasses.end() && (*it)->name != chosenClass; ++it);
	if (it == vecClasses.end()) {
		return;
	}
	else {
		(*it)->fields.push_back(tool->getEntry("fieldName"));
	}
}

void DiagramEdition::editField()
{;
	std::string newFieldName = tool->getEntry("fieldName");
	std::string oldFieldName = tool->getEntry("chosenField");

	std::string chosenClass = tool->getEntry("chosenClass");
	
	auto it = vecClasses.begin();
	for (; it != vecClasses.end() && (*it)->name != chosenClass; ++it);
	if (it == vecClasses.end()) {
		return;
	}
	else {
		auto it2 = (*it)->fields.begin();
		for (; it2 != (*it)->fields.end() && it2->name != oldFieldName; ++it2);
		if (it2 == (*it)->fields.end()) {
			return;
		}
		else {
			it2->name = newFieldName;
		}
	}
}

void DiagramEdition::deleteField()
{
	std::string fieldName = tool->getEntry("chosenField");
	std::string chosenClass = tool->getEntry("chosenClass");

	auto it = vecClasses.begin();
	for (; it != vecClasses.end() && (*it)->name != chosenClass; ++it);
	if (it == vecClasses.end()) {
		return;
	}
	else {
		auto it2 = (*it)->fields.begin();
		for (; it2 != (*it)->fields.end() && it2->name != fieldName; ++it2);
		if (it2 == (*it)->fields.end()) {
			return;
		}
		else {
			(*it)->fields.erase(it2);
		}
	}
}

void DiagramEdition::addInheritance()
{
	std::string chosenClass = tool->getEntry("chosenClass");

	auto it = vecClasses.begin();
	for (; it != vecClasses.end() && (*it)->name != chosenClass; ++it);
	if (it == vecClasses.end()) {
		return;
	}
	else {
		(*it)->inheritedFrom.push_back(tool->getEntry("inheritedFrom"));
	}
}

void DiagramEdition::deleteInheritance()
{
	std::string inheritedFrom = tool->getEntry("chosenInheritance");
	std::string chosenClass = tool->getEntry("chosenClass");

	auto it = vecClasses.begin();
	for (; it != vecClasses.end() && (*it)->name != chosenClass; ++it);
	if (it == vecClasses.end()) {
		return;
	}
	else {
		for (size_t i = 0; i < (*it)->inheritedFrom.size() && (*it)->inheritedFrom[i] == inheritedFrom; i++)
			(*it)->inheritedFrom.erase((*it)->inheritedFrom.begin()+i);
	}
}

