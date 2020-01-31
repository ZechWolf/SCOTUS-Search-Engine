#ifndef QUERY_H
#define QUERY_H

#include <string>
#include <vector>


class Query
{

    friend class QueryEngine; //Query engine class handles private data members

private:

    enum Prefix {NONE, AND, OR, NOT}; //NONE is default value

    Prefix op1; //operation 1 is either AND or OR, or NONE
    Prefix op2; //operation 2 is NOT or NONE
    std::vector<std::string> words1;
    std::string words2;

public:

    Query(): op1(), op2(), words1(), words2() {}
    Query(Prefix o1, Prefix o2): op1(o1), op2(o2), words1(), words2() {}

    bool hasAnd();
    bool hasOr();
    bool hasNot();
    bool noPrefix();

    std::vector<std::string> getWords1();
    std::string getWords2();

};

#endif // QUERY_H
