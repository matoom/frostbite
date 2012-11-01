#ifndef DATA_H
#define DATA_H

#include "data_global.h"

#include <QString>

class DATASHARED_EXPORT Data {

public:
    Data();

    int addNumbers(int num1, int num2);
    char* getString();
    void setString(const char[]);

private:

};

#endif // DATA_H
