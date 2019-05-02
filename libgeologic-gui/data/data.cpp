#include "data/data.h"
#include <geologic-core/utils/customToolbox.h>
#include <iostream>

using namespace std;

Data::Data(string &n) {
    customToolbox::start("Data::Data(string)");
    name = n;
    customToolbox::end("Data::Data(string)");
}

Data::Data(const char *n) {
    customToolbox::start("Data::Data(const char *)");
    name = n;
    customToolbox::end("Data::Data(const char *)");
}

Data::~Data() {
    //cout << "Data::~Data() (" << name << ")" << endl;
}

string Data::getName() {
    return name;
}

void Data::setName(string n) {
    customToolbox::start("Data::setName");
    name = n;
    customToolbox::end("Data::setName");
}
