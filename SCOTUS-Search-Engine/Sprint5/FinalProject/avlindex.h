#ifndef AVLINDEX_H
#define AVLINDEX_H

#include "indexhandler.h"
#include "avltree.h"
#include "word.h"

class AvlIndex : public IndexHandler
{

private:

    AvlTree<Word> index;

public:

    AvlIndex(): index() {}
    ~AvlIndex() override;

    void addWord(Word) override;
    std::vector<DocRef> getWordMatches(std::string) override;
    void writeToFile(int docsParsed) override;
    void readIndexFile(std::string path) override; //implement later
    void outputWordData(std::string) override;
    void clearIndex() override;

};

#endif // AVLINDEX_H
