#ifndef LUX_FIELDDEFINITION_H
#define LUX_FIELDDEFINITION_H

#include <string>

namespace Lux {

    class FieldDefinition {
        
    public:
        FieldDefinition(std::string field_name)
        : field_name_(field_name), index_to_("default"),
          is_indexed_(false), is_exact_(false), 
          is_displayed_(false), is_attr_indexed_(false)
        {}
        ~FieldDefinition()
        {}

        void set_index_to(std::string index_to)
        {
            // if it's for exact match, use a predefined index name
            if (is_exact()) { return; }
            index_to_ = index_to;
        }
        void set_indexed(bool is_indexed, bool is_exact = false)
        { 
            is_indexed_ = is_indexed;
            is_exact_ = is_exact;
            if (is_exact) {
                index_to_ = field_name_;
            }
        }
        void set_displayed(bool is_displayed) { is_displayed_ = is_displayed; }
        void set_attr_indexed(bool is_attr_indexed) { is_attr_indexed_ = is_attr_indexed; }
        // future work when type is in it.
        //void set_type() {}

        std::string get_field_name(void) { return field_name_; }
        std::string get_field_name(void) const { return field_name_; }
        std::string get_index_to(void) { return index_to_; }
        std::string get_index_to(void) const { return index_to_; }
        bool is_indexed(void) { return is_indexed_; }
        bool is_indexed(void) const { return is_indexed_; }
        bool is_exact(void) { return is_exact_; }
        bool is_exact(void) const { return is_exact_; }
        bool is_displayed(void) { return is_displayed_; }
        bool is_displayed(void) const { return is_displayed_; }
        bool is_attr_indexed(void) { return is_attr_indexed_; }
        bool is_attr_indexed(void) const { return is_attr_indexed_; }

    private:
        std::string field_name_;
        std::string index_to_;
        bool is_indexed_;
        bool is_exact_;
        bool is_displayed_;
        bool is_attr_indexed_;
        bool term_vector_stored_;
    };
}

#endif
