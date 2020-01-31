#include "documentparser.h"
#include "porter2_stemmer-master/porter2_stemmer.h"
#include "word.h"
#include "json.hpp"
#include <fstream>
#include <algorithm>

using json = nlohmann::json;

DocumentParser::DocumentParser()
{

    //Initialize stop words from this initalizer list

    docsParsed = 0;
    totalWords = 0;
    stopWords = {"a","able","about","above","abroad","according","accordingly","across","actually","adj","after","afterwards","again","against","ago","ahead","aint","all","allow","allows","almost","alone","along","alongside","already","also","although","always","am","amid","amidst","among","amongst","an","and","another","any","anybody","anyhow","anyone","anything","anyway","anyways","anywhere","apart","appear","appreciate","appropriate","are","arent","around","as","as","aside","ask","asking","associated","at","available","away","awfully","back","backward","backwards","be","became","because","become","becomes","becoming","been","before","beforehand","begin","behind","being","believe","below","beside","besides","best","better","between","beyond","both","brief","but","by","came","can","cannot","cant","cant","caption","cause","causes","certain","certainly","changes","clearly","cmon","co","co.","com","come","comes","concerning","consequently","consider","considering","contain","containing","contains","corresponding","could","couldnt","course","cs","currently","dare","darent","definitely","described","despite","did","didnt","different","directly","do","does","doesnt","doing","done","dont","down","downwards","during","each","edu","eg","eight","eighty","either","else","elsewhere","end","ending","enough","entirely","especially","et","etc","even","ever","evermore","every","everybody","everyone","everything","everywhere","ex","exactly","example","except","fairly","far","farther","few","fewer","fifth","first","five","followed","following","follows","for","forever","former","formerly","forth","forward","found","four","from","further","furthermore","get","gets","getting","given","gives","go","goes","going","gone","got","gotten","greetings","had","hadnt","half","happens","hardly","has","hasnt","have","havent","having","he","hed","hell","hello","help","hence","her","here","hereafter","hereby","herein","heres","hereupon","hers","herself","hes","hi","him","himself","his","hither","hopefully","how","howbeit","however","hundred","id","ie","if","ignored","ill","im","immediate","in","inasmuch","inc","inc.","indeed","indicate","indicated","indicates","inner","inside","insofar","instead","into","inward","is","isnt","it","itd","itll","its","its","itself","ive","just","k","keep","keeps","kept","know","known","knows","last","lately","later","latter","latterly","least","less","lest","let","lets","like","liked","likely","likewise","little","look","looking","looks","low","lower","ltd","made","mainly","make","makes","many","may","maybe","maynt","me","mean","meantime","meanwhile","merely","might","mightnt","mine","minus","miss","more","moreover","most","mostly","mr","mrs","much","must","mustnt","my","myself","name","namely","nd","near","nearly","necessary","need","neednt","needs","neither","never","neverf","neverless","nevertheless","new","next","nine","ninety","no","nobody","non","none","nonetheless","noone","no-one","nor","normally","not","nothing","notwithstanding","novel","now","nowhere","obviously","of","off","often","oh","ok","okay","old","on","once","one","ones","ones","only","onto","opposite","or","other","others","otherwise","ought","oughtnt","our","ours","ourselves","out","outside","over","overall","own","particular","particularly","past","per","perhaps","placed","please","plus","possible","presumably","probably","provided","provides","que","quite","qv","rather","rd","re","really","reasonably","recent","recently","regarding","regardless","regards","relatively","respectively","right","round","said","same","saw","say","saying","says","second","secondly","see","seeing","seem","seemed","seeming","seems","seen","self","selves","sensible","sent","serious","seriously","seven","several","shall","shant","she","shed","shell","shes","should","shouldnt","since","six","so","some","somebody","someday","somehow","someone","something","sometime","sometimes","somewhat","somewhere","soon","sorry","specified","specify","specifying","still","sub","such","sup","sure","take","taken","taking","tell","tends","th","than","thank","thanks","thanx","that","thatll","thats","thats","thatve","the","their","theirs","them","themselves","then","thence","there","thereafter","thereby","thered","therefore","therein","therell","therere","theres","theres","thereupon","thereve","these","they","theyd","theyll","theyre","theyve","thing","things","think","third","thirty","this","thorough","thoroughly","those","though","three","through","throughout","thru","thus","till","to","together","too","took","toward","towards","tried","tries","truly","try","trying","ts","twice","two","un","under","underneath","undoing","unfortunately","unless","unlike","unlikely","until","unto","up","upon","upwards","us","use","used","useful","uses","using","usually","v","value","various","versus","very","via","viz","vs","want","wants","was","wasnt","way","we","wed","welcome","well","well","went","were","were","werent","weve","what","whatever","whatll","whats","whatve","when","whence","whenever","where","whereafter","whereas","whereby","wherein","wheres","whereupon","wherever","whether","which","whichever","while","whilst","whither","who","whod","whoever","whole","wholl","whom","whomever","whos","whose","why","will","willing","wish","with","within","without","wonder","wont","would","wouldnt","yes","yet","you","youd","youll","your","youre","yours","yourself","yourselves","youve","zero"};

}

void DocumentParser::removeSpaces(std::string& str) {

    //Cleans up leftover spaces in words extracted from the main string

    for (size_t i = 0; i < str.size() - 1; i++) {

        if (isspace(str[i])) str.erase(i,1);

    }

}

void DocumentParser::htmlCleanup(std::string& str) {

    //Strips html tags from the text and formats for extraction of words

    size_t i = 0;

    while (true) { //loop through the string

        char& current = str[i]; //get reference to i

        if (current == '<') {

            while (true) { //removes characters until the end of the tag is reached

                str.erase(i,1);
                if (current == '>' || i == str.size() - 1) {

                    str.erase(i,1);
                    break;

                }


            }

        }

        if (current == 0) break; //end of string reached

        if (current == '\n' || current == '\r') {

            current = ' '; //replace \n and \r with space to make parsing easier
            i++;
            continue;

        }

        if (!isalpha(current) && !isspace(current)) {

            str.erase(i,1); //erase non alpha chars
            continue;

        }

        current = static_cast<char>(tolower(current));

        i++;

    }

}

void DocumentParser::htmlTagsOnly(std::string& str) {

    size_t i = 0;

    while (true) { //loop through the string

        char& current = str[i]; //get reference to i

        if (current == '<') {

            while (true) { //removes characters until the end of the tag is reached

                str.erase(i,1);
                if (current == '>' || i == str.size() - 1) {

                    str.erase(i,1);
                    break;

                }


            }

        }

        if (current == 0) break; //end of string reached

        if (current == '\n' || current == '\r') {

            current = ' '; //replace \n and \r with space to make parsing easier
            i++;
            continue;

        }

        i++;

    }

}

void DocumentParser::getWords(std::string& str, std::string filePath, IndexHandler*& index) {

    std::vector<Word> words; //contains list of words before they are added to index

    size_t lastPos = -1; //stores the pos of last occurrence of delim, starts at -1 so that lastPos + 1 == 0 for first time

    for (size_t i = 0; i < str.size(); i++) {

        if (str.at(i) == ' ' || i == (str.size() - 1)) {

            std::string wordStr;

            if (i == (str.size() - 1)) wordStr = str.substr(lastPos + 1, i - lastPos); //no space at the end to be skipped when it is last word

            else wordStr = str.substr(lastPos + 1, i - lastPos - 1); //value to be added to index

            if (wordStr == "" || wordStr == " ") { //avoid multiple spaces causing empty strings to be added

                lastPos = i;
                continue;

            }

            removeSpaces(wordStr); //formats the string for stemming

            if (stopWords.contains(wordStr)) { //first check for stop words

                lastPos = i;
                continue;

            }

            Porter2Stemmer::stem(wordStr); //stem and add word to index

            if (wordStr.size() > 2) { //words shorter than 3 characters are irrelevant

                /* Word objects here are place holders, will not be inserted into index, unless unique
                 * These word objects store the frequency in the current doc, then their data is merged
                 * with the word existing in the index already
                */

                Word word(wordStr);
                auto existing = std::find(words.begin(), words.end(), word); //linear search for value

                if (existing != words.end())  //value exists in the vector
                    existing->addOccurrenceInCurrentDoc(); //increase frequency in this document
                else {

                    word.addDocRef(DocRef(currentDoc, filePath, 1));
                    words.push_back(word); //word not found, insert into vector

                }

            }

            lastPos = i;

        }

    }

    totalWords += words.size();

    for (auto i : words)
        index->addWord(i); //add all words in the doc to the index after parsing is complete

}

void DocumentParser::parseDirectory(std::string filePath, IndexHandler*& index) {

    //Parses all files in given directory path

    bool testingMode = false; //flags whether a limit is set for how many docs will be parsed from the

    DIR* dir = opendir(filePath.c_str());
    struct dirent* dp;

    while ((dp = readdir(dir)) != nullptr) {

        //Open files and parse
        std::ifstream inFile;
        std::string fName = filePath + "/" + dp->d_name;
        inFile.open(fName);
        currentDoc = dp->d_name;

        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue; //skip over current and parent dir markers

        if (!inFile.good()) std::cout << "Error opening file " << dp->d_name << std::endl;

        json j;
        inFile >> j;
        inFile.close();

        std::string text;

        if (j["html"].size() < j["plain_text"]) text = j["plain_text"];
        else
            text = j["html"];

        htmlCleanup(text);
        getWords(text, filePath, index);

        docsParsed++;
        if (docsParsed % 1000 == 0) std::cout << "Parsed " << docsParsed << " files" << std::endl;
        if (testingMode && docsParsed == 10000) break;

    }
    closedir(dir);

}

unsigned int DocumentParser::getDocsParsed() {

    return docsParsed;

}

double DocumentParser::getAverageWords() {

    return static_cast<double>(totalWords) / static_cast<double>(docsParsed);

}

std::string DocumentParser::displayFormat(DocRef doc) {

    std::ifstream inFile;
    inFile.open(doc.getPath() + "/" + doc.getFileName());

    json j;
    inFile >> j;

    inFile.close();

    std::string text;

    if (j["html"].size() < j["plain_text"]) text = j["plain_text"];
    else
        text = j["html"];

    htmlTagsOnly(text);
    return text.substr(0,1000);


}
