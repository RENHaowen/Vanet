#ifndef DATA_H
#define DATA_H

#include <string>

class Data {
protected:
    std::string name;

public:
    Data(std::string &n);
    Data(const char *n);
    virtual ~Data() = 0;
    virtual void draw() = 0;
    virtual void draw(unsigned long long simu_time) = 0;
    virtual std::string getName();
    virtual void setName(std::string n);
};

#endif // DATA_H
