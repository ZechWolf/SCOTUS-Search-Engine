#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <list>
#include <vector>
#include <fstream>
#include "word.h"

class HashTable
{

private:

    size_t length;
    std::vector<std::list<Word>> buckets; //dynamically allocated pointer array of linked lists

    size_t hash(Word w);

public:

    HashTable(): length(), buckets() {}
    HashTable(size_t l): length(l), buckets(l) {}

    void insert(Word w);
    bool contains(Word w);
    Word* find(Word searchTerm);

    void toFile(std::ofstream&);
    std::vector<Word> toVector();

    size_t size();

};

#endif // HASHTABLE_H
