#ifndef INDEXHANDLER_H
#define INDEXHANDLER_H

#include "word.h"
#include "docref.h"
#include <string>
#include <vector>

class IndexHandler
{

public:

    IndexHandler() {}

    virtual ~IndexHandler();

    virtual void addWord(Word) = 0;
    virtual std::vector<DocRef> getWordMatches(std::string) = 0;
    virtual void writeToFile(int docsParsed) = 0;
    virtual void readIndexFile(std::string path) = 0;
    virtual void outputWordData(std::string) = 0;
    virtual void clearIndex() = 0;

};

#endif // INDEXHANDLER_H
