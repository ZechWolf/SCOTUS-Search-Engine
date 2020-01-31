#include "hashindex.h"
#include <algorithm>
#include <chrono>

bool HashIndex::sortByFreq(const Word& w1, const Word& w2) {

    //Used to sort words in descending freq order for outputting top 50 words

    return w1.getTotalOccurrences() > w2.getTotalOccurrences();

}

void HashIndex::addWord(Word w) {

    //Add a word to the index
    //Increase the frequency if already there

    if (index.contains(w)) { //Index already contains word

        index.find(w)->mergeData(w); //merging handles adding frequency to a document

    }
    else { //insert new word object into the index

        index.insert(w);

    }

}

void HashIndex::writeToFile(int docsParsed) {

    std::ofstream outFile;
    outFile.open("index_persistent.txt");
    outFile << docsParsed << '\n';
    outFile << index.size() << '\n';
    index.toFile(outFile);
    outFile.close();

}

void HashIndex::readIndexFile(std::string path) {

    std::ifstream inFile;
    inFile.open(path);

    int docsParsed;
    int numWords;
    inFile >> docsParsed;
    inFile.ignore();
    inFile >> numWords;
    inFile.ignore();


    for (int i = 0; i < numWords; i++) {

        std::string docNum;
        std::string freq;
        std::string wordStr;

        std::getline(inFile, docNum, ',');
        std::getline(inFile, freq, ',');
        std::getline(inFile, wordStr, ',');
        int numDocs = stoi(docNum);
        int frequency = stoi(freq);

        Word word(wordStr, frequency);

        for (int i = 0; i < numDocs; i++) {

            std::string filePath;
            std::string fileName;
            std::string freq;

            std::getline(inFile, filePath, ',');
            std::getline(inFile, fileName, ',');

            if (i != numDocs - 1)
                std::getline(inFile, freq, ',');
            else
                std::getline(inFile, freq);

            DocRef dr(fileName, filePath, static_cast<unsigned int>(stoi(freq)));
            word.addDocRef(dr);

        }

        index.insert(word);


    }

    inFile.close();

}

std::vector<DocRef> HashIndex::getWordMatches(std::string search) {

    //Main searching function, returns all docrefs matching the given string

    if (index.contains(Word(search))) {

        return index.find(Word(search))->getDocsVec();

    }
    else return std::vector<DocRef>(); //return default, empty vec if not found

}

void HashIndex::outputWordData(std::string word) { //outputs index stats for the demo, not used in final project

    std::cout << "AvlTree index node count: " << index.size() << std::endl;

    Word* w = index.find(Word(word));
    if (w == nullptr) {

        std::cout << "Word not found in index" << std::endl;
        return;

    }

    std::cout << "Number of documents containing " << word << ": " << w->getTotalDocs() << std::endl;
    std::cout << "Total occurrences of " << word << ": " << w->getTotalOccurrences() << std::endl;

}

void HashIndex::clearIndex() {

    index = HashTable(10000);

}
