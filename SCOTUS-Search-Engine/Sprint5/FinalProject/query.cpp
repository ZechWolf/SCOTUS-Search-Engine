#include "query.h"

bool Query::hasAnd() {

    return op1 == AND;

}

bool Query::hasOr() {

    return op1 == OR;

}

bool Query::hasNot() {

    return op2 == NOT;

}

bool Query::noPrefix() {

    return op1 == NONE;

}

std::vector<std::string> Query::getWords1() {

    return words1;

}

std::string Query::getWords2() {

    return words2;

}
