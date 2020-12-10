#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include <string>

using namespace std;

class options {
public:
    string rom;
    string controlMap;
    options(int argc, char** argv);
    void optionsError(char** argv);
    friend ostream& operator<<(ostream& os, const options& opt);
};

#endif