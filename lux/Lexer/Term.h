#ifndef LUX_LEXER_TERM_H
#define LUX_LEXER_TERM_H

#include <string>

namespace Lux {

    class Term {

    public:
        Term(std::string text = "", unsigned int length = 0, unsigned int offset = 0)
        : text_(text), length_(length), offset_(offset)
        {}
        Term operator+(Term &term)
        {
            std::string text = text_ + term.text_;
            unsigned int length = length_ + term.length_;
            unsigned int offset = offset_;
            return Term(text, length, offset);
        }
        Term &operator+=(Term &term)
        {
            text_ += term.text_;
            length_ += term.length_;
            // no change in offset_
            return *this;
        }
        // bool equals(Token &token);
        // any other in the future
    
        std::string text_;
        unsigned int length_;
        unsigned int offset_;
    };
}

#endif
