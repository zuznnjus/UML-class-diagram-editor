#include "ReadWriteFile.h"
#include "tinyxml2.h"

using namespace tinyxml2;

std::vector<UmlClass*> ReadWriteFile::readFromFile(const std::string &fileName)
{
	std::vector<UmlClass*> vecClasses;
	XMLDocument inFile;
	inFile.LoadFile(fileName.c_str());
	if (inFile.ErrorID() == 0) {
		XMLNode* root = inFile.FirstChild();
		if (root != nullptr) {
			XMLElement* classes = root->FirstChildElement("UmlClass");
			while (classes != nullptr)
			{
				UmlClass* umlClass = new UmlClass();
				XMLElement* classElements = classes->FirstChildElement("name");
				umlClass->name = classElements->GetText();
				classElements = classes->FirstChildElement("fields");
				if (classElements != nullptr) {
					XMLElement* fieldElements = classElements->FirstChildElement("UmlField");
					while (fieldElements != nullptr)
					{
						UmlField* umlField = new UmlField(fieldElements->GetText());
						umlClass->fields.push_back(*umlField);
						fieldElements = fieldElements->NextSiblingElement("UmlField");
					}
				}
				classElements = classes->FirstChildElement("inheritedFrom");
				if (classElements != nullptr) {
					XMLElement* inheritanceElements = classElements->FirstChildElement("name");
					while (inheritanceElements != nullptr)
					{
						umlClass->inheritedFrom.push_back(inheritanceElements->GetText());
						inheritanceElements = inheritanceElements->NextSiblingElement("name");
					}
				}
				vecClasses.push_back(umlClass);
				classes = classes->NextSiblingElement("UmlClass");
			}
		}
	}
	else {
		throw FileNotFound();
	}

	return vecClasses;
}

void ReadWriteFile::writeToFile(const std::string &fileName,const std::vector<UmlClass*> &vecClasses)
{
	XMLDocument outFile;
	XMLNode* root = outFile.NewElement("UmlDiagram");
	outFile.InsertFirstChild(root);
	for (unsigned int i = 0; i < vecClasses.size(); i++)
	{
		XMLElement* classes = outFile.NewElement("UmlClass");
		root->InsertEndChild(classes);
		XMLElement* classElements = outFile.NewElement("name");
		classElements->SetText((vecClasses[i]->name).c_str());
		classes->InsertEndChild(classElements);

		if (vecClasses[i]->fields.size() > 0) {
			classElements = outFile.NewElement("fields");
			classes->InsertEndChild(classElements);
		}
		for (unsigned int j = 0; j < vecClasses[i]->fields.size(); j++) {
			XMLElement* fieldElements = outFile.NewElement("UmlField");
			fieldElements->SetText((vecClasses[i]->fields[j].name).c_str());
			classElements->InsertEndChild(fieldElements);
		}

		if (vecClasses[i]->inheritedFrom.size() > 0) {
			classElements = outFile.NewElement("inheritedFrom");
			classes->InsertEndChild(classElements);
		}
		for (unsigned int k = 0; k < vecClasses[i]->inheritedFrom.size(); k++) {
			XMLElement* inheritanceElements = outFile.NewElement("name");
			inheritanceElements->SetText((vecClasses[i]->inheritedFrom[k]).c_str());
			classElements->InsertEndChild(inheritanceElements);
		}
	}
	outFile.SaveFile(fileName.c_str());
} 
