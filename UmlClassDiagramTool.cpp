#include "UmlClassDiagramTool.h"
#include "ReadWriteFile.h"

#include <stdexcept>

UmlClassDiagramTool::UmlClassDiagramTool() :
	edition(this),
	window(nullptr),
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
	window = windowptr;
}

void UmlClassDiagramTool::inputMode()
{
	auto& vecClasses = edition.getVec();
	ReadWriteFile* file = new ReadWriteFile();
	try {
		vecClasses = file->readFromFile(getEntry("fileName"));
	}
	catch (ReadWriteFile::FileNotFound) {
		alert.fileAlert();
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
	file->writeToFile(getEntry("fileName"), vecClasses);
	setEntry("IS_SAVED","YES");
	delete file;
}

void UmlClassDiagramTool::printClasses()
{
	wclear(window);
	auto& vecClasses = edition.getVec();
	wprintw(window, "Utworzone klasy:\n");
	for (unsigned int i = 0; i < vecClasses.size(); i++) {
		wprintw(window, "%s\n", vecClasses[i]->name.c_str());
	}
	wrefresh(window);
}

void UmlClassDiagramTool::printAllFields()
{
	wclear(window);
	auto& vecClasses = edition.getVec();
	wprintw(window, "Pola danych klas:\n");
	for (unsigned int i = 0; i < vecClasses.size(); i++) {
		wprintw(window, "%s:\n", vecClasses[i]->name.c_str());
		for (unsigned int j = 0; j < vecClasses[i]->fields.size(); j++) {
			wprintw(window, "%s\n", vecClasses[i]->fields[j].name.c_str());
		}
		wprintw(window, "\n");
	}
	wrefresh(window);
}

void UmlClassDiagramTool::printFields()
{
	wclear(window);
	auto& vecClasses = edition.getVec();
	std::string chosenClass;

	try {
		chosenClass = getEntry("chosenClass");
	}
	catch (BaseTool::UndefinedEntry) {
		alert.alertMsg();
		return;
	}

	unsigned int i = 0;
	for (; i < vecClasses.size() && vecClasses[i]->name != chosenClass; i++);
	if (i == vecClasses.size()) return;
	else {
		wprintw(window, "Pola klasy %s:\n", vecClasses[i]->name.c_str());
		for (unsigned int j = 0; j < vecClasses[i]->fields.size(); j++) {
			wprintw(window, "%s\n", vecClasses[i]->fields[j].name.c_str());
		}
		wprintw(window, "\n");
	}
	wrefresh(window);
}

void UmlClassDiagramTool::printAllInheritance()
{
	wclear(window);
	auto& vecClasses = edition.getVec();
	wprintw(window, "Klasa : jej klasy bazowe\n");
	for (unsigned int i = 0; i < vecClasses.size(); i++) {
		if (vecClasses[i]->inheritedFrom.size() > 0) {
			wprintw(window, "%s:", vecClasses[i]->name.c_str());
		}
		for (unsigned int k = 0; k < vecClasses[i]->inheritedFrom.size(); k++) {
			wprintw(window, "%s ", vecClasses[i]->inheritedFrom[k].c_str());
		}
		if (vecClasses[i]->inheritedFrom.size() > 0) {
			wprintw(window, "\n");
		}
	}
	wrefresh(window);
}

void UmlClassDiagramTool::printInheritance()
{
	wclear(window);
	auto& vecClasses = edition.getVec();
	std::string chosenClass;

	try {
		chosenClass = getEntry("chosenClass");
	}
	catch (BaseTool::UndefinedEntry) {
		alert.alertMsg();
		return;
	}

	unsigned int i = 0;
	for (; i < vecClasses.size() && vecClasses[i]->name != chosenClass; i++);
	if (i == vecClasses.size()) return;
	wprintw(window, "Klasy bazowe dla klasy %s:\n", vecClasses[i]->name.c_str());
	for (unsigned int k = 0; k < vecClasses[i]->inheritedFrom.size(); k++) {
		wprintw(window, "%s ", vecClasses[i]->inheritedFrom[k].c_str());
	}
	if (vecClasses[i]->inheritedFrom.size() > 0) {
		wprintw(window, "\n");
	}
	wrefresh(window);
}

void UmlClassDiagramTool::addClass()
{
	try {
		edition.addClass();
		printClasses();
		drawDiagram();
	}
	catch (UndefinedEntry) {
		alert.alertMsg();
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
		alert.alertMsg();
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
		alert.alertMsg();
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
		alert.alertMsg();
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
		alert.alertMsg();
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
		alert.alertMsg();
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
		alert.alertMsg();
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
		alert.alertMsg();
	}
}

void UmlClassDiagramTool::setCounter(std::string key,int size)
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
	
	for (int i = 0; i < size; i++) {
		if (int(vecClasses[i]->name.length()) > width) width = vecClasses[i]->name.length()+1;
	}

	WINDOW* win = derwin(window, size, 4, 1, width);

	setCounter(key, size);
	setIndex(size);

	if (key == "<ENTER>") {
		chosenClass = vecClasses[index]->name;
		setEntry("chosenClass", chosenClass);
		counter = 0;
		index = 0;
		mvwprintw(window, size + 2, 0, "Wybrana klasa do edycji to: %s", chosenClass.c_str());
		wrefresh(window);
	}

	wclear(win);
	mvwaddch(win, index, 0, ACS_LARROW);
	wrefresh(win);
	delwin(win);
}

void UmlClassDiagramTool::chooseField()
{
	auto& vecClasses = edition.getVec();
	std::string key = getEntry("KEY");
	std::string chosenField, chosenClass = getEntry("chosenClass") ;

	unsigned int i = 0;
	for (; i < vecClasses.size() && vecClasses[i]->name != chosenClass; i++);
	
	int size = vecClasses[i]->fields.size(), width = 2;

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

	unsigned int i = 0;
	for (; i < vecClasses.size() && vecClasses[i]->name != chosenClass; i++);


	int size = vecClasses[i]->inheritedFrom.size(), width = 2;
	if (size == 0) return;
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
	for (unsigned int i = 0; i < vecClasses.size(); i++) {
		if (int(vecClasses[i]->name.length()) > width) width = vecClasses[i]->name.length() + 2;
		for (unsigned int j = 0; j < vecClasses[i]->fields.size(); j++) {
			if (int(vecClasses[i]->fields[j].name.length()) > width) width = vecClasses[i]->fields[j].name.length() + 2;
		}
	}
	return width;
}

void UmlClassDiagramTool::drawInheritanceLine(int** coordinates, unsigned int win1, unsigned int win2, int width, int& gap, int space)
{

	int x = COLS / 2 + width, y = coordinates[win1][1];

	int height = coordinates[win1][1] - coordinates[win2][0];
	if (height < 0) {
		mvwaddch(window, y -= 2, x, ACS_LARROW);
		mvwhline(window, y, x += 1, 0, gap-3);
		mvwaddch(window, y , x+=(gap - 3), ACS_BBSS);
		mvwvline(window, y+=1, x, 0, abs(height)+space-1);
		mvwaddch(window, y+= (abs(height) + space - 1), x, ACS_SBBS);
		mvwhline(window, y, x -= (gap-2), 0, gap - 2);
		gap += 2;
	}
	else {
		mvwaddch(window, y -= 2, x, ACS_LARROW);
		mvwhline(window, y, x += 1, 0, gap - 3);
		mvwaddch(window, y, x += (gap - 3), ACS_SBBS);
		mvwvline(window, y -= (height - space), x, 0, height - space);
		mvwaddch(window, y, x, ACS_BBSS);
		mvwhline(window, y, x -= (gap - 2), 0, gap - 2);
		gap += 2;
	}
}

void UmlClassDiagramTool::drawDiagram()
{
	auto& vecClasses = edition.getVec();
	std::vector<WINDOW*> windows;

	int y=0, x = COLS/2;
	int height, width=0, space=3;
	int gap = 4; 
	int size = vecClasses.size();
	int** coordinates = new int*[size];
	for (int i = 0; i < size; i++) {
		coordinates[i] = new int[2];
	}
	

	for (unsigned int win=0; win < vecClasses.size(); win++) {
		height = calculateHeight(win);
		width = calculateWidth(win);
		windows.emplace_back(derwin(window, height, width, y, x));

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
		mvwprintw(windows[win], 1, (width- vecClasses[win]->name.length()) / 2, vecClasses[win]->name.c_str());
		if(vecClasses[win]->fields.size()>0)
		mvwhline(windows[win], 2, 1, 0, width-2);

		int h = 3;
		for (unsigned int j = 0; j < vecClasses[win]->fields.size(); j++) {
			mvwprintw(windows[win], h++, 1, vecClasses[win]->fields[j].name.c_str());
		}
		wnoutrefresh(windows[win]);
	}
	doupdate();

	for (unsigned int win1 = 0; win1 < vecClasses.size(); ++win1) {
		for (unsigned int win2 = 0; win2 < vecClasses.size(); ++win2) {
			if(win1!=win2) {
				for (unsigned int i = 0; i < vecClasses[win2]->inheritedFrom.size(); ++i) {
					if (vecClasses[win1]->name==vecClasses[win2]->inheritedFrom[i]) {
						drawInheritanceLine(coordinates,win1,win2,width,gap,space);
					}
				}
			}
		}
	}
	
	for (unsigned int win{}; win < vecClasses.size(); ++win)
		delwin(windows[win]);
}

