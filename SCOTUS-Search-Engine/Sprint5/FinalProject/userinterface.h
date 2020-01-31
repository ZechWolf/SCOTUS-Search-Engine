#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <iostream>
#include "indexhandler.h"
#include "documentparser.h"
#include "avlindex.h"
#include "queryengine.h"
#include "hashindex.h"


class UserInterface
{

private:

    DocumentParser parser;
    IndexHandler* interface;
    QueryEngine qe;

    static bool sortWordByFreq(const Word&, const Word&);
    static bool sortByFreq(const DocRef&, const DocRef&);

    //MENU FUNCTIONS//
    void maintenanceMode();
    void interactiveMode();

    //PARSING//
    void parseDir(std::string path);

    //QUERY HANDLING//
    std::vector<DocRef> processQuery();
    void performSearch();
    void printDocInfo(DocRef) const;
    void displayResults(std::vector<DocRef>);
    void displayStats();

    //FILE IO//
    void indexToFile();
    void readIndexFile(std::string path = "index_persistent.txt");

public:

    UserInterface(): parser(), interface(), qe() {}
    ~UserInterface();

    void start();
    void timedSearch(std::string query);

};

#endif // USERINTERFACE_H
