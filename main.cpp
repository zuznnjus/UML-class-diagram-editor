#include <curses.h>
#include "UmlClassDiagramTool.h"
#include "BaseBackend.h"
#include "MceditBackend.h"

int main()
{
	UmlClassDiagramTool *tool = new UmlClassDiagramTool();
	MceditBackend* backend = new MceditBackend(tool);
	backend->bind(".plik.do odczytu${Podaj nazwe pliku do odczytu:|fileName}", [&tool]() {tool->inputMode(); }, "opis");
	backend->bind(".plik.do zapisu${Podaj nazwe pliku do zapisu:|fileName}", [&tool]() {tool->outputMode(); }, "opis");
	backend->bind(".trybEdycji.klasy${Podaj nazwe edytowanej klasy:|editedClassName}", [&tool]() {}, "opis");
	backend->bind(".trybEdycji.pola${Podaj nazwe edytowanej klasy:|editedClassName}", [&tool]() {}, "opis");
	backend->bind(".trybEdycji.dziedzczenie${Podaj nazwe edytowanej klasy:|editedClassName}", [&tool]() {}, "opis");
	backend->bind(".klasa.dodaj klase${Nazwa: |className}", [&tool]() {tool->addClass(); }, "");
	backend->bind(".klasa.edytuj klase${Nazwa: |className}", [&tool]() {tool->editClass(); }, "");
	backend->bind(".klasa.usun klase${Nazwa: |className}", [&tool]() {tool->deleteClass(); }, "");
	backend->bind(".pole.dodaj pole${Nazwa: |fieldName}", [&tool]() {tool->addField(); }, "opis");
	//backend->bind(".pole.edytuj nazwe pola${Nazwa: |fieldName}", [&tool]() {tool->deleteField(); }, "opis");
	backend->bind(".pole.usun pole${Nazwa: |fieldName}", [&tool]() {tool->deleteField();}, "opis");
	backend->bind(".dziedziczenie.dodaj dziedziczenie${Nazwa: |inheritedFrom}", [&tool]() {tool->addInheritance(); }, "opis");
	backend->bind(".dziedziczenie.usun dziedziczenie${Nazwa: |inheritedFrom}", [&tool]() {tool->deleteInheritance();  }, "opis");
	backend->bind("<EDITION>", [&tool]() {mvprintw(1, 0, "%s", tool->getEntry("KEY").c_str()); refresh();  }, "");
	backend->start();
	delete backend;
}

//edit name etc??
//draw inher plik3.xml
//gdy sie nie miesci
//ostatni podpkt
//zamiast wclear 