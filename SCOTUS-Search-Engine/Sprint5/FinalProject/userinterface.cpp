#include "userinterface.h"
#include "json.hpp"
#include <chrono>

using json = nlohmann::json;

using namespace std;

bool UserInterface::sortByFreq(const DocRef& d1, const DocRef& d2) {

    //Comp function for sorting docref vectors by frequency

    return d1.getFreq() > d2.getFreq();

}

bool UserInterface::sortWordByFreq(const Word& w1, const Word& w2) {

    return w1.getTotalOccurrences() > w2.getTotalOccurrences();

}

UserInterface::~UserInterface() {

    delete interface;

}

void UserInterface::performSearch() {

    displayResults(processQuery());

}

vector<DocRef> UserInterface::processQuery() {

    //Asks for a query and processes it

    vector<DocRef> exclusionList;

    string query;
    cout << "Please enter a valid query: ";
    cin.ignore();
    getline(cin, query);

    Query q = qe.parseQuery(query);
    vector<vector<DocRef>> fullSet; //stores doc ref vectors from each word in the query

    for (auto iter : q.getWords1()) { //iter is a string from the query

        if (interface->getWordMatches(iter).empty())
            return vector<DocRef>();
        else
            fullSet.push_back(interface->getWordMatches(iter));

    }

    if (q.hasNot()) {


        if (interface->getWordMatches(q.getWords2()).empty())
            return vector<DocRef>();
        else
            exclusionList = interface->getWordMatches(q.getWords2());

    }


    vector<DocRef> allRefs; //docrefs of all word in the same vec

    for (auto iter : fullSet) {

        for (auto iter2 : iter) {

            if (q.hasNot()) {

                if (find(exclusionList.begin(), exclusionList.end(), iter2) == exclusionList.end())
                    allRefs.push_back(iter2);

            }
            else
                allRefs.push_back(iter2);

        }

    }

    if (q.hasOr() || q.noPrefix()) {

        sort(allRefs.begin(), allRefs.end());
        allRefs.erase(unique(allRefs.begin(), allRefs.end()), allRefs.end()); //remove all duplicate values
        sort(allRefs.begin(), allRefs.end(), sortByFreq);
        return allRefs; //allrefs contains the list of docs that contains at least one of the words

    }

    else if (q.hasAnd()) {

        /* All docrefs for the list of words for the AND operation are put into a single vector
         * Vector is sorted, so that correpsonding occurrences are next to each other
         * Iterate through the vector, when a sequence of the same docrefs is found, find if
         * the length of the sequence == the number of words in the AND operation, which means the
         * document contains all words in the vector
        */

        vector<DocRef> matches; //intersection of docrefs between queries

        sort(allRefs.begin(), allRefs.end());

        size_t i = 0;
        size_t count = 0;
        while (i != allRefs.size() - 1) {

            if (allRefs.at(i) == allRefs.at(i + 1)) {

                i++;
                count++;

            }
            else {

                if (count + 1 == fullSet.size()) {

                    if (q.hasNot()) {

                        if (find(exclusionList.begin(), exclusionList.end(), allRefs.at(i)) == exclusionList.end())
                            matches.push_back(allRefs.at(i)); //only add the word if its not in the exclusion list
                    }
                    else
                        matches.push_back(allRefs.at(i));

                }
                i++;
                count = 0;

            }

        }

        sort(matches.begin(), matches.end(), sortByFreq); //sort by frequency to get relevant results
        return matches;

    }
    else
        return vector<DocRef>(); //empty vector is error return, word not found in index

}

void UserInterface::printDocInfo(DocRef doc) const {

    ifstream file;
    file.open(doc.getPath() + "/" + doc.getFileName());

    json j;
    file >> j;

    string parties;
    string year;

    if (j["local_path"].size() != 0) {

        parties = j["local_path"];
        parties = parties.substr(15, parties.size() - 19); //clean up the info to get just the text
        for (char& c : parties) {

            if (c == '_') c = ' '; //replace underscores with space to make it look nicer

        }

    }
    else parties = "File does not contain parties to the case";

    if (j["date_created"].size() != 0) {

       year = j["date_created"];
       year = year.substr(0, 4);

    }
    else {

       year = "File does not contain a year";

    }

    cout << "File name: " << doc.getFileName() << endl;
    cout << "Year: " << year << endl;
    cout << "Parties to case: " << parties << endl;
    cout << "\n";

}

void UserInterface::displayResults(std::vector<DocRef> vec) {

    cout << '\n';

    if (vec.size() == 0) {

        cout << "Error: at least one of the given search terms is not in the index, or the query returned no results" << endl;
        return;

    }

    size_t stop;
    if (vec.size() >= 15) stop = 15;
    else stop = vec.size();

    for (size_t i = 0; i < stop; i++) {

        cout << "Document Number " << i + 1 << endl;
        printDocInfo(vec.at(i));

    }

    cout << "\nEnter a document number to view, or 0 to return to menu: ";
    int choice;
    cin >> choice;
    if (choice == 0) interactiveMode();
    else if (choice <= 15) {

        choice--;
        cout << parser.displayFormat(vec.at(choice));

    }

}

void UserInterface::parseDir(std::string path) {

    if (interface == nullptr) {

        cout << "The interface is not populated. Please choose an index type before parsing" << endl;
        cout << "1) AvlTree Index" << endl;
        cout << "2) HashTable Index" << endl;

        int choice;
        cin >> choice;
        if (choice == 1) interface = new AvlIndex;
        else interface = new HashIndex;
        cout << '\n';

    }
    parser.parseDirectory(path, interface);

}

void UserInterface::indexToFile() {

    interface->writeToFile(parser.getDocsParsed());

}

void UserInterface::readIndexFile(std::string path) {

    interface->readIndexFile(path);

}

void UserInterface::displayStats() {

    ifstream pi;
    pi.open("index_persistent.txt");

    int docsParsed;
    int numWords;
    pi >> docsParsed;
    pi.ignore();
    pi >> numWords;
    pi.ignore();

    vector<Word> words;
    for (int i = 0; i < numWords; i++) {

        std::string docNum;
        std::string freq;
        std::string wordStr;
        std::string extraneous; //grab the rest of the info on the line, not used

        std::getline(pi, docNum, ',');
        std::getline(pi, freq, ',');
        std::getline(pi, wordStr, ',');
        std::getline(pi, extraneous);
        int frequency = stoi(freq);

        Word w(wordStr, frequency);
        words.push_back(w);

    }

    double average = static_cast<double>(numWords) / static_cast<double>(docsParsed);

    cout << "Opinions indexed: " << docsParsed << endl;
    cout << "Average words per doc: " << average << endl;

    sort(words.begin(), words.end(), sortWordByFreq);
    words.resize(50);
    for (Word iter : words) cout << iter.getStr() << ": " << iter.getTotalOccurrences() << endl;

}

void UserInterface::maintenanceMode() {

    int choice = 0;
    while (choice != 5) {

        cout << "Maintenance Mode" << endl;
        cout << "1) Add new opinions" << endl;
        cout << "2) Clear index" << endl;
        cout << "3) Parse the corpus" << endl;
        cout << "4) Write current index to file" << endl;
        cout << "5) Main menu" << endl;
        cin >> choice;

        if (choice == 1) {

            string path;
            cout << "Please supply a full path to the opinions folder: ";
            cin >> path;
            cout << "Parsing opinions..." << endl;
            parseDir(path);
            cout << "Parsing complete" << endl;

        }
        else if (choice == 2) {

            interface->clearIndex();
            cout << "Index has been cleared" << endl;

        }
        else if (choice == 3) {

            cout << "Parsing corpus..." << endl;
            parseDir("scotus-full");
            cout << "Parsing completed" << endl;

        }
        else if (choice == 4) {

            indexToFile();
            cout << "Index has been saved to file" << endl;

        }
        else; //choice = 5 breaks the loop

        cout << "\n"; //add a space for organization

    }

    start();

}

void UserInterface::interactiveMode() {

    int choice = 0;
    while (choice != 4) {

        cout << "Interactive Mode" << endl;
        cout << "1) Select index type and read in from persistent index" << endl;
        cout << "2) Search the corpus" << endl;
        cout << "3) Display search engine statistics" << endl;
        cout << "4) Main menu" << endl;
        cin >> choice;

        if (choice == 1) {

            int choice2 = 0;
            cout << "1) AvlTree Index" << endl;
            cout << "2) Hash Table Index" << endl;
            cin >> choice2;
            if (choice2 == 1) {

                interface = new AvlIndex;
                readIndexFile();

            }
            else {

                interface = new HashIndex;
                readIndexFile();

            }

        }
        else if (choice == 2) {

            if (interface == nullptr)
                cout << "Error: the index has not been populated yet. Please choose an option to parse or read from file before searching" << endl;
            else
                performSearch();

        }
        else if (choice == 3) {

            displayStats();

        }
        else;

        cout << "\n";

    }

    start(); //return to main menu

}

void UserInterface::start() {

    int choice = 0;

    while (choice != 3) {

        cout << "Mustang Law Firm Search Engine" << endl;
        cout << "1) Maintenance mode" << endl;
        cout << "2) Interactive mode" << endl;
        cout << "3) Exit" << endl;

        cin >> choice;

        if (choice == 1)
            maintenanceMode();
        else if (choice == 2)
            interactiveMode();
        else exit(0);

        cout << "\n";

    }

}

void UserInterface::timedSearch(string query) {

    //Times the search for a single word, used for collecting data for analysis

    interface = new HashIndex; //changed manually for each index type
    readIndexFile(); //changed manually for each size
    //parseDir("scotus-full");

    for (int i = 0; i < 10; i++) {

        auto start = std::chrono::high_resolution_clock::now();
        interface->getWordMatches(query);
        auto stop = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        cout << duration.count() << endl;

    }

}


