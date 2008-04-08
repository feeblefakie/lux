#ifndef LUX_FIELD_H
#define LUX_FIELD_H

#include "FieldDefinition.h"
#include <string>

namespace Lux {

    class Field {
    
    public:
        ~Field() {}

        static Field *create(const std::string &name, const std::string &value,
                             FieldDefinition *fd = NULL)
        {
            Field *field = new Field(name, value, fd);
            return field;
        }

        static Field *create(const char *name, const char *value,
                             FieldDefinition *fd = NULL)
        {
            std::string n(name);
            std::string v(value); 
            return Field::create(n, v, fd);
        }

        std::string get_name(void) { return name_; };
        std::string get_name(void) const { return name_; };
        std::string get_value(void) { return value_; };
        std::string get_value(void) const { return value_; };

        const FieldDefinition *def(void) { return def_; };
        const FieldDefinition *def(void) const { return def_; };

    private:
        std::string name_;
        std::string value_;
        const FieldDefinition *def_; // this must point to a static object

        Field(const std::string &name, const std::string &value, FieldDefinition *fd)
        : name_(name), value_(value), def_(fd)
        {}
    };
}

#endif
