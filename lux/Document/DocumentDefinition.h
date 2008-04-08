#ifndef LUX_DOCUMENTDEFINITION_H
#define LUX_DOCUMENTDEFINITION_H

#include "FieldDefinition.h"
#include <string>
#include <vector>

namespace Lux {

    typedef std::vector<FieldDefinition> FDContainer;
    typedef FDContainer::const_iterator FDIterator;

    class DocumentDefinition {
        
    public:
        DocumentDefinition(std::string doc_name)
        : doc_name_(doc_name) 
        {}
        ~DocumentDefinition()
        {}

        DocumentDefinition(const DocumentDefinition &doc_def)
        : doc_name_(doc_def.doc_name_), field_definitions_(doc_def.field_definitions_)
        {
            fd_iterator_ = field_definitions_.begin();
        }

        DocumentDefinition &operator=(const DocumentDefinition &rhs)
        {
            doc_name_ = rhs.doc_name_;
            field_definitions_ = rhs.field_definitions_;
            fd_iterator_ = field_definitions_.begin();
            return *this;
        }

        void add(FieldDefinition field_definition) 
        {
            field_definitions_.push_back(field_definition);
            fd_iterator_ = field_definitions_.begin();
        }

        std::string get_doc_name() { return doc_name_; }
        std::string get_doc_name() const { return doc_name_; }

        bool has_next()
        {
            return static_cast<const DocumentDefinition&>(*this).has_next();
        }

        bool has_next() const
        {
            if (fd_iterator_ == field_definitions_.end()) {
                return false;
            }
            return true;
        }

        const FieldDefinition get_next() { return *fd_iterator_++; }
        const FieldDefinition get_next() const { return *fd_iterator_++; }
        void init_iter() { fd_iterator_ = field_definitions_.begin(); }
        void init_iter() const { fd_iterator_ = field_definitions_.begin(); }

    private:
        std::string doc_name_;
        FDContainer field_definitions_;
        mutable FDIterator fd_iterator_;
    };
}

#endif
