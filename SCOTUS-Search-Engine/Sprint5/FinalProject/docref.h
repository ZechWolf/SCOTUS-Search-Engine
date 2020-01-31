#ifndef DOCREF_H
#define DOCREF_H

#include <string>

/*The word class is an aggregate class of DocRef objects
 * Each word contains an AvlTree of DocRef objects, sorted by the total appearances
 * in the given document. A reverse order traversal of the tree will list out all
 * docs containing the word in descending order of frequency - helpful for determining relevancy of results
*/

class DocRef
{

private:

    std::string fileName;
    std::string path;
    unsigned int frequency;

public:

    DocRef(): fileName(), frequency() {}
    DocRef(std::string n, unsigned int f): fileName(n), frequency(f) {}
    DocRef(std::string n, std::string p, unsigned int f): fileName(n), path(p), frequency(f) {}

    bool operator< (const DocRef&) const;
    bool operator== (const DocRef&) const;

    void addFreq();
    unsigned int getFreq() const;
    std::string getFileName() const;
    std::string getPath() const;

};

#endif // DOCREF_H
