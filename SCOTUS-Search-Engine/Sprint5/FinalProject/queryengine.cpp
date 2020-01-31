#include "queryengine.h"
#include "porter2_stemmer-master/porter2_stemmer.h"

QueryEngine::QueryEngine()
{

}

Query QueryEngine::parseQuery(std::string str) {

    /* Query processing function
     * First extract the boolean operations, and erase them from the string
     * such that the string contains the words for the first operator followed by comma,
     * then the words for the second operator
     * EXAMPLE: AND test1 test2 NOT test3  ->  test1 test2,test3
     * Now create string stream obj and read in the words using getline, storing into the query obj
    */

    Query q;
    bool containsNot = false;

    if (str.find("AND") != std::string::npos) { //search the string for boolean operators

        q.op1 = Query::Prefix::AND;
        str.erase(0, 4); //remove the AND and the space after it

    }
    else if (str.find("OR") != std::string::npos) {

        q.op1 = Query::Prefix::OR;
        str.erase(0, 3); //remove the OR and the space after it

    }

    if (str.find("NOT") != std::string::npos) {

        q.op2 = Query::Prefix::NOT;
        str.replace(str.find("NOT") - 1, 5, ","); //replace the NOT and surrounding spaces with ','
        containsNot = true; //flag that this query contains a NOT sequence

    }

    std::stringstream simplifiedQuery(str); //stringstream of the whole query

    //Handle the query with a not operator

    std::string group1;     //group1 is the words related to the AND/OR or no operator
    std::getline(simplifiedQuery, group1, ',');

    std::stringstream g1stream(group1); //each word grouping will be read in with stringstream

    std::string word;
    while (g1stream.good()) {

        g1stream >> word;
        Porter2Stemmer::stem(word);
        q.words1.push_back(word);

    }

    if (containsNot) {

        std::string group2;     //group2 is the words related to the NOT
        std::getline(simplifiedQuery, group2);
        std::stringstream g2stream(group2);
        while (g2stream.good()) {

            g2stream >> word;
            Porter2Stemmer::stem(word);
            q.words2 = word;

        }

    }

    return q;

}
