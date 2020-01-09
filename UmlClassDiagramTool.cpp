#include "UmlClassDiagramTool.h"
#include "ReadWriteFile.h"

#include <stdexcept>

UmlClassDiagramTool::UmlClassDiagramTool() :
	edition(this),
	window(nullptr)
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
	vecClasses = file->readFromFile(getEntry("fileName"));
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
		wprintw(window,"%s\n", vecClasses[i]->name.c_str());
	}
	wrefresh(window);
}

void UmlClassDiagramTool::printFields()
{
	wclear(window);
	auto& vecClasses = edition.getVec();
	wprintw(window, "Pola danych klas:\n");
	for (unsigned int i = 0; i < vecClasses.size(); i++) {
		wprintw(window,"%s:\n", vecClasses[i]->name.c_str());
		for (unsigned int j = 0; j < vecClasses[i]->fields.size(); j++) {
			wprintw(window, "%s\n", vecClasses[i]->fields[j].name.c_str());
		}
		wprintw(window,"\n");
	}
	wrefresh(window);
}

void UmlClassDiagramTool::printInheritance()
{
	wclear(window);
	auto& vecClasses = edition.getVec();
	wprintw(window, "Klasa : jej klasy bazowe\n");
	for (unsigned int i = 0; i < vecClasses.size(); i++) {
		if (vecClasses[i]->inheritedFrom.size() > 0) {
			wprintw(window,"%s:", vecClasses[i]->name.c_str());
		}
		for (unsigned int k = 0; k < vecClasses[i]->inheritedFrom.size(); k++) {
			wprintw(window, "%s ", vecClasses[i]->inheritedFrom[k].c_str());
		}
		if (vecClasses[i]->inheritedFrom.size() > 0) {
			wprintw(window,"\n");
		}
	}
	wrefresh(window);
}

void UmlClassDiagramTool::addClass()
{
	edition.addClass();
	printClasses();
	drawDiagram();
}

void UmlClassDiagramTool::editClass()
{
	edition.editClass();
	printClasses();
	drawDiagram();
}

void UmlClassDiagramTool::deleteClass()
{
	edition.deleteClass();
	printClasses();
	drawDiagram();
}

void UmlClassDiagramTool::addField()
{
	edition.addField();
	printFields();
	drawDiagram();
}

void UmlClassDiagramTool::deleteField()
{
	edition.deleteField();
	printFields();
	drawDiagram();
}

void UmlClassDiagramTool::addInheritance()
{
	edition.addInheritance();
	printInheritance();
	drawDiagram();
}

void UmlClassDiagramTool::deleteInheritance()
{
	edition.deleteInheritance();
	printInheritance();
	drawDiagram();
}

void UmlClassDiagramTool::checkEditionMode()
{
	int modeNumber = stoi(getEntry("modeNumber"));
	switch (modeNumber)
	{
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		break;
	}
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
	int x = COLS / 2 + width / 2, y = coordinates[win1][1]+2;
	if (win1 - win2 == -1) {
		mvwaddch(window, y - 1, x, ACS_UARROW);
		mvwvline(window, y, x, 0, space);
	}
	else if (win1 - win2 == 1) {
		mvwaddch(window, y-1, x, ACS_UARROW);
		mvwvline(window, y, x, 0, 1);
		mvwaddch(window, y += 1, x, ACS_SSBB);
		mvwhline(window, y, x += 1, 0, width / 2 + gap);
		mvwaddch(window, y, x += width / 2 + gap, ACS_SBBS);
		mvwvline(window, y-= (coordinates[win1][1] - coordinates[win2][0] + space), x, 0, coordinates[win1][1] - coordinates[win2][0] + space);
		mvwaddch(window, y-=1, x, ACS_BBSS);
		mvwhline(window, y, x = x - (width / 2 + gap), 0, width / 2 + gap);
		mvwaddch(window, y, x-=1, ACS_BSSB);
	}
	else if (win1 - win2 < 1) {
		mvwaddch(window, y-1, x, ACS_UARROW);
		mvwvline(window, y, x, 0, 1);
		mvwaddch(window, y += 1, x, ACS_SSBB);
		mvwhline(window, y, x += 1, 0, width / 2 + gap);
		mvwaddch(window, y, x += width / 2 + gap, ACS_BBSS);
		mvwvline(window, y+=1, x, 0, coordinates[win2][0] - coordinates[win1][1] - space - 1);
		mvwaddch(window, y += (coordinates[win2][0] - coordinates[win1][1] - space - 1), x, ACS_SBBS);
		mvwhline(window, y, x = x - (width / 2 + gap - 1), 0, width / 2 + gap - 1);
		mvwaddch(window, y, x-=1, ACS_BSSB);
		mvwvline(window, y += 1, x, 0, 1);
	}
	else {

	}
	gap += 2;
	//wrefresh(window);
}

void UmlClassDiagramTool::drawDiagram()
{
	auto& vecClasses = edition.getVec();
	std::vector<WINDOW*> windows;

	int y = 2, x = COLS/2;
	int height, width=0, space=3;
	int gap = 2; 
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
