#include "UmlClassDiagramTool.h"
#include "ReadWriteFile.h"

#include <stdexcept>

#define errorMsg "First you have to choose item to edition\n \t(go to -> EditionMode)"

UmlClassDiagramTool::UmlClassDiagramTool() :
	edition(this),
	window(nullptr),
	listWindow(nullptr),
	diagramWindow(nullptr),
	errorWindow(nullptr),
	counter(0),
	index(0)
{
}

UmlClassDiagramTool::~UmlClassDiagramTool()
{
}

std::string UmlClassDiagramTool::getEntry(const std::string& key) const
{
	try {
		return entries.at(key);
	}
	catch (std::out_of_range&) {
		throw BaseTool::UndefinedEntry();
	}
}

void UmlClassDiagramTool::setEntry(const std::string& key, const std::string& value)
{
	entries[key] = value;
}

void UmlClassDiagramTool::assignWindow(WINDOW* windowptr)
{
	int x, y;
	window = windowptr;
	getmaxyx(window, y, x);
	listWindow = derwin(window, y, x / 3, 0, 0);
	diagramWindow = derwin(window, y, x - x / 3, 0, x / 3);
}

void UmlClassDiagramTool::inputMode()
{
	auto& vecClasses = edition.getVec();
	ReadWriteFile* file = new ReadWriteFile();
	try {
		lastFileName = getEntry("fileName");
		vecClasses = file->readFromFile(lastFileName);
	}
	catch (ReadWriteFile::FileNotFound) {
		wclear(window);
		errorWindow = alert.alertMsg(window, "Wrong file name, try one more time");
		return;
	}
	wclear(window);
	drawDiagram();
	delete file;
}

void UmlClassDiagramTool::outputMode()
{
	auto& vecClasses = edition.getVec();
	ReadWriteFile* file = new ReadWriteFile();
	std::string fileName = getEntry("fileName");
	if (fileName == "") fileName = lastFileName;
	file->writeToFile(fileName, vecClasses);
	errorWindow = alert.alertMsg(window, "Zapisano do pliku " + fileName);
	setEntry("IS_SAVED", "YES");
	delete file;
}

void UmlClassDiagramTool::printClasses()
{
	wclear(listWindow);
	auto& vecClasses = edition.getVec();
	wprintw(listWindow, "Utworzone klasy:\n");
	for (size_t i = 0; i < vecClasses.size(); i++) {
		wprintw(listWindow, "%s\n", vecClasses[i]->name.c_str());
	}
	wrefresh(listWindow);
}

void UmlClassDiagramTool::printAllFields()
{
	wclear(listWindow);
	auto& vecClasses = edition.getVec();
	wprintw(listWindow, "Pola danych klas:\n");
	for (size_t i = 0; i < vecClasses.size(); i++) {
		wprintw(listWindow, "%s:\n", vecClasses[i]->name.c_str());
		for (size_t j = 0; j < vecClasses[i]->fields.size(); j++) {
			wprintw(listWindow, "%s\n", vecClasses[i]->fields[j].name.c_str());
		}
		wprintw(listWindow, "\n");
	}
	wrefresh(listWindow);
}

void UmlClassDiagramTool::printFields()
{
	wclear(listWindow);
	auto& vecClasses = edition.getVec();
	std::string chosenClass;

	try {
		chosenClass = getEntry("chosenClass");
	}
	catch (BaseTool::UndefinedEntry) {
		errorWindow = alert.alertMsg(window, errorMsg);
		return;
	}

	unsigned int i = 0;
	for (; i < vecClasses.size() && vecClasses[i]->name != chosenClass; i++);
	if (i == vecClasses.size()) return;
	else {
		wprintw(listWindow, "Pola klasy %s:\n", vecClasses[i]->name.c_str());
		for (size_t j = 0; j < vecClasses[i]->fields.size(); j++) {
			wprintw(listWindow, "%s\n", vecClasses[i]->fields[j].name.c_str());
		}
		wprintw(listWindow, "\n");
	}
	wrefresh(listWindow);
}

void UmlClassDiagramTool::printAllInheritance()
{
	wclear(listWindow);
	auto& vecClasses = edition.getVec();
	wprintw(listWindow, "Klasa : jej klasy bazowe\n");
	for (size_t i = 0; i < vecClasses.size(); i++) {
		if (vecClasses[i]->inheritedFrom.size() > 0) {
			wprintw(listWindow, "%s:", vecClasses[i]->name.c_str());
		}
		for (size_t k = 0; k < vecClasses[i]->inheritedFrom.size(); k++) {
			wprintw(listWindow, "%s ", vecClasses[i]->inheritedFrom[k].c_str());
		}
		if (vecClasses[i]->inheritedFrom.size() > 0) {
			wprintw(listWindow, "\n");
		}
	}
	wrefresh(listWindow);
}

void UmlClassDiagramTool::printInheritance()
{
	wclear(listWindow);
	auto& vecClasses = edition.getVec();
	std::string chosenClass;

	try {
		chosenClass = getEntry("chosenClass");
	}
	catch (BaseTool::UndefinedEntry) {
		errorWindow = alert.alertMsg(window, errorMsg);
		return;
	}

	size_t i = 0;
	for (; i < vecClasses.size() && vecClasses[i]->name != chosenClass; i++);
	if (i == vecClasses.size()) return;
	wprintw(listWindow, "Klasy bazowe dla klasy %s:\n", vecClasses[i]->name.c_str());
	for (size_t k = 0; k < vecClasses[i]->inheritedFrom.size(); k++) {
		wprintw(listWindow, "%s ", vecClasses[i]->inheritedFrom[k].c_str());
	}
	if (vecClasses[i]->inheritedFrom.size() > 0) {
		wprintw(listWindow, "\n");
	}
	wrefresh(listWindow);
}

void UmlClassDiagramTool::addClass()
{
	try {
		edition.addClass();
		printClasses();
		drawDiagram();
	}
	catch (UndefinedEntry) {
		errorWindow = alert.alertMsg(window, errorMsg);
	}
	catch (DiagramEdition::EmptyItemName) {
		errorWindow = alert.alertMsg(window, "Class name can't be empty!\n");
	}
}

void UmlClassDiagramTool::editClass()
{
	try {
		edition.editClass();
		printClasses();
		drawDiagram();
	}
	catch (UndefinedEntry) {
		errorWindow = alert.alertMsg(window, errorMsg);
	}
}

void UmlClassDiagramTool::deleteClass()
{
	try {
		edition.deleteClass();
		printClasses();
		drawDiagram();
	}
	catch (UndefinedEntry) {
		errorWindow = alert.alertMsg(window, errorMsg);
	}
}

void UmlClassDiagramTool::addField()
{
	try {
		edition.addField();
		printAllFields();
		drawDiagram();
	}
	catch (UndefinedEntry) {
		errorWindow = alert.alertMsg(window, errorMsg);
	}
}

void UmlClassDiagramTool::editField()
{
	try {
		edition.editField();
		printAllFields();
		drawDiagram();
	}
	catch (UndefinedEntry) {
		errorWindow = alert.alertMsg(window, errorMsg);
	}

}

void UmlClassDiagramTool::deleteField()
{
	try {
		edition.deleteField();
		printAllFields();
		drawDiagram();
	}
	catch (UndefinedEntry) {
		errorWindow = alert.alertMsg(window, errorMsg);
	}
}

void UmlClassDiagramTool::addInheritance()
{
	try {
		edition.addInheritance();
		printAllInheritance();
		drawDiagram();
	}
	catch (UndefinedEntry) {
		errorWindow = alert.alertMsg(window, errorMsg);
	}
}

void UmlClassDiagramTool::deleteInheritance()
{
	try {
		edition.deleteInheritance();
		printAllInheritance();
		drawDiagram();
	}
	catch (UndefinedEntry) {
		errorWindow = alert.alertMsg(window, errorMsg);
	}
}

void UmlClassDiagramTool::setCounter(std::string key, int size)
{
	if (key == "<LARROW>") {
		--counter;
		if (counter == -1) counter = size - 1;
	}
	else if (key == "<RARROW>") {
		++counter;
		if (counter == size) counter = 0;
	}
}

void UmlClassDiagramTool::setIndex(int size)
{
	if (size > 0) {
		if (counter >= 0)
			index = counter % size;
		else {
			index = size + (counter % size);
		}
	}
}

void UmlClassDiagramTool::chooseClass()
{
	auto& vecClasses = edition.getVec();
	std::string chosenClass;
	int size = vecClasses.size(), width = 2;
	std::string key = getEntry("KEY");

	if (size == 0) {
		errorWindow = alert.alertMsg(window, "\tNo items created\n");
		return;
	}

	for (int i = 0; i < size; i++) {
		if (int(vecClasses[i]->name.length()) > width) width = vecClasses[i]->name.length() + 1;
	}

	WINDOW* win = derwin(window, size, 4, 1, width);

	setCounter(key, size);
	setIndex(size);

	if (key == "<ENTER>") {
		chosenClass = vecClasses[index]->name;
		setEntry("chosenClass", chosenClass);
		counter = 0;
		index = 0;
		wclear(win);
		wrefresh(win);
		mvwprintw(window, size + 2, 0, "Wybrana klasa do edycji to: %s", chosenClass.c_str());
		wrefresh(window);
	}
	else {
		wclear(win);
		mvwaddch(win, index, 0, ACS_LARROW);
		wrefresh(win);
	}

	delwin(win);
}

void UmlClassDiagramTool::chooseField()
{
	auto& vecClasses = edition.getVec();
	std::string key = getEntry("KEY");
	std::string chosenField, chosenClass = getEntry("chosenClass");

	size_t i = 0;
	for (; i < vecClasses.size() && vecClasses[i]->name != chosenClass; i++);

	int size = vecClasses[i]->fields.size(), width = 2;

	if (size == 0) {
		errorWindow = alert.alertMsg(window, "\tNo items created\n");
		return;
	}

	for (int j = 0; j < size; j++) {
		if (int(vecClasses[i]->fields[j].name.length()) > width) width = vecClasses[i]->fields[j].name.length() + 1;
	}

	WINDOW* win = derwin(window, size, 4, 1, width);

	setCounter(key, size);
	setIndex(size);

	if (key == "<ENTER>") {
		chosenField = vecClasses[i]->fields[index].name;
		setEntry("chosenField", chosenField);
		counter = 0;
		index = 0;
		mvwprintw(window, size + 2, 0, "Wybrane pole do edycji to: %s", chosenField.c_str());
		wrefresh(window);
	}

	wclear(win);
	mvwaddch(win, index, 0, ACS_LARROW);
	wrefresh(win);
	delwin(win);
}

void UmlClassDiagramTool::chooseInheritance()
{
	auto& vecClasses = edition.getVec();
	std::string chosenClass = getEntry("chosenClass"), chosenInheritance;

	size_t i = 0;
	for (; i < vecClasses.size() && vecClasses[i]->name != chosenClass; i++);


	int size = vecClasses[i]->inheritedFrom.size(), width = 2;
	if (size == 0) {
		errorWindow = alert.alertMsg(window, "\tNo items created\n");
		return;
	}

	std::string key = getEntry("KEY");

	for (int j = 0; j < size; j++) {
		if (int(vecClasses[i]->inheritedFrom[j].length()) > width) width = vecClasses[i]->fields[j].name.length() + 1;
	}

	WINDOW* win = derwin(window, size, 4, 1, width);

	setCounter(key, size);
	setIndex(size);

	if (key == "<ENTER>") {
		chosenInheritance = vecClasses[i]->inheritedFrom[index];
		setEntry("chosenInheritance", chosenInheritance);
		counter = 0;
		index = 0;
		mvwprintw(window, size + 2, 0, "Wybrane dziedziczenie do edycji to: %s", chosenInheritance.c_str());
		wrefresh(window);
	}

	wclear(win);
	mvwaddch(win, index, 0, ACS_LARROW);
	wrefresh(win);
	delwin(win);
}


int UmlClassDiagramTool::calculateHeight(int i)
{
	auto& vecClasses = edition.getVec();
	int height = vecClasses[i]->fields.size() + 4;
	return height;
}

int UmlClassDiagramTool::calculateWidth(int i)
{
	auto& vecClasses = edition.getVec();
	int width = 20;
	for (size_t i = 0; i < vecClasses.size(); i++) {
		if (int(vecClasses[i]->name.length()) > width) width = vecClasses[i]->name.length() + 2;
		for (size_t j = 0; j < vecClasses[i]->fields.size(); j++) {
			if (int(vecClasses[i]->fields[j].name.length()) > width) width = vecClasses[i]->fields[j].name.length() + 2;
		}
	}
	return width;
}

void UmlClassDiagramTool::drawInheritanceLine(int** coordinates, size_t win1, size_t win2, int width, int& gap, int space)
{

	int x = getmaxx(diagramWindow) / 3 + width, y = coordinates[win1][1];

	int height = coordinates[win1][1] - coordinates[win2][0];
	if (height < 0) {
		mvwaddch(diagramWindow, y -= 2, x, ACS_LARROW);
		mvwhline(diagramWindow, y, x += 1, 0, gap - 3);
		mvwaddch(diagramWindow, y, x += (gap - 3), ACS_BBSS);
		mvwvline(diagramWindow, y += 1, x, 0, abs(height) + space - 1);
		mvwaddch(diagramWindow, y += (abs(height) + space - 1), x, ACS_SBBS);
		mvwhline(diagramWindow, y, x -= (gap - 2), 0, gap - 2);
		gap += 2;
	}
	else {
		mvwaddch(diagramWindow, y -= 2, x, ACS_LARROW);
		mvwhline(diagramWindow, y, x += 1, 0, gap - 3);
		mvwaddch(diagramWindow, y, x += (gap - 3), ACS_SBBS);
		mvwvline(diagramWindow, y -= (height - space), x, 0, height - space);
		mvwaddch(diagramWindow, y, x, ACS_BBSS);
		mvwhline(diagramWindow, y, x -= (gap - 2), 0, gap - 2);
		gap += 2;
	}
	wrefresh(diagramWindow);
}

void UmlClassDiagramTool::drawDiagram()
{
	auto& vecClasses = edition.getVec();
	std::vector<WINDOW*> windows;
	wclear(diagramWindow);
	wrefresh(diagramWindow);

	int y = 0, x;
	int height, width = 0, space = 3;
	int gap = 4;
	x = getmaxx(diagramWindow) / 3;

	int size = vecClasses.size();
	int** coordinates = new int* [size];
	for (int i = 0; i < size; i++) {
		coordinates[i] = new int[2];
	}

	for (size_t win = 0; win < vecClasses.size(); win++) {
		height = calculateHeight(win);
		width = calculateWidth(win);
		windows.emplace_back(derwin(diagramWindow, height, width, y, x));

		if (win != 0) {
			coordinates[win][0] = coordinates[win - 1][1] + space;
			coordinates[win][1] = coordinates[win][0] + height;
		}
		else {
			coordinates[win][0] = y;
			coordinates[win][1] = height;
		}

		y += height + space;
		box(windows[win], 0, 0);
		mvwprintw(windows[win], 1, (width - vecClasses[win]->name.length()) / 2, vecClasses[win]->name.c_str());
		if (vecClasses[win]->fields.size() > 0)
			mvwhline(windows[win], 2, 1, 0, width - 2);

		int h = 3;
		for (size_t j = 0; j < vecClasses[win]->fields.size(); j++) {
			mvwprintw(windows[win], h++, 1, vecClasses[win]->fields[j].name.c_str());
		}
		wnoutrefresh(windows[win]);
	}
	doupdate();

	for (size_t win1 = 0; win1 < vecClasses.size(); ++win1) {
		for (size_t win2 = 0; win2 < vecClasses.size(); ++win2) {
			if (win1 != win2) {
				for (size_t i = 0; i < vecClasses[win2]->inheritedFrom.size(); ++i) {
					if (vecClasses[win1]->name == vecClasses[win2]->inheritedFrom[i]) {
						drawInheritanceLine(coordinates, win1, win2, width, gap, space);
					}
				}
			}
		}
	}

	for (size_t win{}; win < vecClasses.size(); ++win)
		delwin(windows[win]);

	wrefresh(window);
}


void UmlClassDiagramTool::defaultEditionHandler()
{
	if (errorWindow) {
		wclear(errorWindow);
		wrefresh(errorWindow);
		delwin(errorWindow);
		errorWindow = nullptr;
	}
	if (getEntry("KEY") == "<RESIZE>") {
		drawDiagram();
	}
}


