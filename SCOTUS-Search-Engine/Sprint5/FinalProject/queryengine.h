#ifndef QUERYENGINE_H
#define QUERYENGINE_H

#include <string>
#include "query.h"
#include <bits/stdc++.h>

class QueryEngine
{

public:

    QueryEngine();

    Query parseQuery(std::string);


};

#endif // QUERYENGINE_H
