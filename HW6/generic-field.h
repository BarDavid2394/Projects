#ifndef GENERIC_FIELD_H
#define GENERIC_FIELD_H

#include "string.h"

class GenericField {
public:
    virtual bool match(String packet) = 0;
    virtual bool set_value(String value) = 0;
    virtual ~GenericField() {}
};

#endif

