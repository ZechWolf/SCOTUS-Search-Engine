#include "hashtable.h"

size_t HashTable::hash(Word w) {

    //Uses std::hash to hash the string value of the word

    std::string str = w.getStr();
    std::hash<std::string> stringHasher;
    size_t index = stringHasher(str);
    return index % length;

}

void HashTable::insert(Word w) {

    size_t index = hash(w);

    std::list<Word>& chain = buckets[index]; //reference to the list
    chain.push_back(w);


}

bool HashTable::contains(Word searchTerm) {

    size_t index = hash(searchTerm);
    std::list<Word>& chain = buckets[index];

    for (Word& iter : chain) {

        if (iter == searchTerm) return true;

    }

    return false;

}

Word* HashTable::find(Word searchTerm) {

    //Hashes the given value to get the index, then searches the list for the searchTerm
    //Return ptr to term if found, nullptr if not

    size_t index = hash(searchTerm);
    std::list<Word>& chain = buckets[index];

    for (Word& iter : chain) {

        if (iter == searchTerm) return &iter;

    }

    return nullptr;

}

void HashTable::toFile(std::ofstream& file) {

    for (auto iter : buckets) {

        if (iter.size() > 0) {

            for (auto iter2 : iter) {

                file << iter2;

            }

        }

    }

}

std::vector<Word> HashTable::toVector() {

    //Inserts all elements into a vector
    //Useful for simple iteration of elements, for printing engine stats etc.

    std::vector<Word> vec;

    for (auto iter : buckets) {

        if (iter.size() > 0) {

            for (auto iter2 : iter) {

                vec.push_back(iter2);

            }

        }

    }

    return vec;

}

size_t HashTable::size() {

    //Returns how many elements are in the table

    size_t sum = 0;

    for (auto chain : buckets) {

        sum += chain.size();

    }

    return sum;

}
