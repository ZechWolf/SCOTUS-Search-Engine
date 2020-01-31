#include "word.h"

bool Word::operator== (const Word& w) const {

    return data == w.data;

}

bool Word::operator== (const std::string& str) const {

    return data == str;

}

bool Word::operator< (const Word& w) const {

    return data < w.data;

}

bool Word::operator< (const std::string& str) const {

    return data < str;

}

bool Word::operator> (const Word& w) const {

    return data > w.data;

}

bool Word::operator> (const std::string& str) const {

    return data > str;

}

std::ostream& operator<<(std::ostream& os, const Word& w) {

    //Ouputs the data in a readable format
    //  #docrefs,total frequency,string,filename1,frequency,...\n

    //Format example:   2,523,petition,/home/etc/etc,12345.json,5,/home/etc/etc,54321.json,10\n

    os << w.docs.size(); //first information is how many docrefs the word contains
    os << ",";
    os << w.frequency;
    os << ",";
    os << w.data;
    os << ",";

    for (size_t i = 0; i < w.docs.size(); i++) {

        os << w.docs[i].getPath();
        os << ",";
        os << w.docs[i].getFileName();
        os << ",";
        os << w.docs[i].getFreq();

        if (i == w.docs.size() - 1) os << "\n"; //end of line reached
        else os << ",";

    }

    return os;

}

void Word::addOccurrenceInCurrentDoc() {

    //For use in DocParser::getWords()
    //The only docref in the docs vec is the current doc, index 0

    docs[0].addFreq();
    frequency++;

}

void Word::addDocRef(DocRef dr) {

    docs.push_back(dr);

}

void Word::mergeData(Word w) {

    //Merges the docrefs vector of w with this

    for (auto i : w.docs) {

        this->docs.push_back(i);
        frequency += i.getFreq();

    }

}

std::string Word::getStr() {return data;}

unsigned int Word::getTotalOccurrences() const {

    return frequency;

}

size_t Word::getTotalDocs() {

    return docs.size();

}

std::vector<DocRef> Word::getDocsVec() {

    return docs;

}
