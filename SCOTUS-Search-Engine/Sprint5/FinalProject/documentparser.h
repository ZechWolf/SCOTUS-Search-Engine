#ifndef DOCUMENTPARSER_H
#define DOCUMENTPARSER_H

#include <iostream>
#include <vector>
#include "indexhandler.h"
#include "dirent.h"

class DocumentParser
{

private:

    AvlTree<std::string> stopWords;
    std::string currentDoc;
    unsigned int docsParsed;
    unsigned int totalWords;

    void removeSpaces(std::string&);

public:

    DocumentParser();

    void htmlCleanup(std::string&);
    void htmlTagsOnly(std::string&);
    void getWords(std::string&, std::string filePath, IndexHandler*&);
    void toLower(std::string&);

    void parseDirectory(std::string, IndexHandler*&);

    unsigned int getDocsParsed();
    double getAverageWords();

    std::string displayFormat(DocRef doc);


};

#endif // DOCUMENTPARSER_H
