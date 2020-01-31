#ifndef WORD_H
#define WORD_H

#include <iostream>
#include <fstream>
#include <vector>
#include "avltree.h"
#include "docref.h"

//Stores words parsed from the data

class Word
{

private:

    std::string data;
    std::vector<DocRef> docs;
    unsigned int frequency;

public:

    Word(): data(), docs(), frequency() {}
    Word(std::string d, unsigned int f = 0): data(d), docs(), frequency(f) {}

    bool operator==(const std::string&) const;
    bool operator==(const Word&) const;
    bool operator<(const Word&) const;
    bool operator<(const std::string&) const;
    bool operator>(const Word&) const;
    bool operator>(const std::string&) const;

    friend std::ostream& operator<<(std::ostream&, const Word&);

    void addOccurrenceInCurrentDoc();
    void addDocRef(DocRef);
    void mergeData(Word);

    std::string getStr();

    unsigned int getTotalOccurrences() const;
    size_t getTotalDocs();
    std::vector<DocRef> getDocsVec();


};

#endif // WORD_H
