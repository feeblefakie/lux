#ifndef LUX_FIELD_H
#define LUX_FIELD_H

#include <string>

namespace Lux {

  class Field {
  
  public:
    ~Field(void) {}

    static Field *create(const std::string &name, const std::string &value)
    {
      Field *field = new Field(name, value);
      return field;
    }

    static Field *create(const char *name, const char *value)
    {
      std::string n(name);
      std::string v(value); 
      return Field::create(n, v);
    }

    std::string get_name(void) { return name_; };
    std::string get_name(void) const { return name_; };
    std::string get_value(void) { return value_; };
    std::string get_value(void) const { return value_; };

  private:
    std::string name_;
    std::string value_;

    Field(const std::string &name, const std::string &value)
    : name_(name), value_(value)
    {}
  };
}

#endif
