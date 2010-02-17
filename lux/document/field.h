#ifndef LUX_FIELD_H
#define LUX_FIELD_H

#include <string>

namespace Lux {

  /**
   * field in a Document
   */
  class Field {
  
  public:
    /**
     * destructor
     */
    ~Field(void) {}
    /**
     * create a field
     * @param name field name (reference)
     * @param value field value (reference)
     * @note singleton
     */
    static Field *create(const std::string &name, const std::string &value)
    {
      Field *field = new Field(name, value);
      return field;
    }
    /**
     * create a field
     * @param name field name (pointer)
     * @param value field value (pointer)
     * @note singleton
     */
    static Field *create(const char *name, const char *value)
    {
      std::string n(name);
      std::string v(value); 
      return Field::create(n, v);
    }
    /**
     * get the name of the field
     */
    std::string get_name(void) { return name_; };
    /**
     * get the name of the field
     */
    std::string get_name(void) const { return name_; };
    /**
     * get the value of the field
     */
    std::string get_value(void) { return value_; };
    /**
     * get the value of the field
     */
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
