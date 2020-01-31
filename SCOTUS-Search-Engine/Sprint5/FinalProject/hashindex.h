#ifndef HASHINDEX_H
#define HASHINDEX_H

#include "indexhandler.h"
#include "hashtable.h"

class HashIndex : public IndexHandler
{

private:

    HashTable index;

    static bool sortByFreq(const Word& w1, const Word& w2);

public:

    HashIndex(): index(10000) {} //start with 10k slots to avoid rehasing for a while
    ~HashIndex() override {}

    void addWord(Word) override;
    std::vector<DocRef> getWordMatches(std::string) override;
    void writeToFile(int docsParsed) override;
    void readIndexFile(std::string path) override; //implement later
    void outputWordData(std::string) override;
    void clearIndex() override;

};

#endif // HASHINDEX_H
