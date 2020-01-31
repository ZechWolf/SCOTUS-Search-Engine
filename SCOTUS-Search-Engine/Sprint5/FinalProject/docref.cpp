#include "docref.h"

void DocRef::addFreq() {

    frequency++;

}

unsigned int DocRef::getFreq() const {

    return frequency;

}

std::string DocRef::getFileName() const {

    return fileName;

}

std::string DocRef::getPath() const {

    return path;

}

bool DocRef::operator<(const DocRef& rhs) const { //allows sorting by filename

    return fileName < rhs.fileName;

}

bool DocRef::operator==(const DocRef& rhs) const {

    return fileName == rhs.fileName;

}
