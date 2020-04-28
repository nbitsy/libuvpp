
#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include <string>
#include <vector>

#include "Spliter.h"

namespace XSpace
{

class Tokenizer
{
public:
    typedef std::string value_type;
    typedef std::vector<value_type> container;
    typedef container::size_type size_type;
    typedef container::iterator iterator;

public:
    Tokenizer(const char *tokens, char sp = ' ')
    {
        if (tokens)
            split(_toks, tokens, sp);
    }

    Tokenizer(const char *tokens, const char *sp = " ")
    {
        if (tokens && sp)
            split(_toks, tokens, sp);
    }

    Tokenizer(const std::string &tokens, char sp = ' ')
    {
        if (tokens.length())
            split(_toks, tokens, sp);
    }

    Tokenizer(const std::string &tokens, const std::string &sp = " ")
    {
        if (tokens.length() && sp.length())
            split(_toks, tokens, sp);
    }

    Tokenizer(const Tokenizer &tk) : _toks(tk._toks) {}
    ~Tokenizer() { clear(); }

    inline size_type size() const { return _toks.size(); }
    inline void clear() { _toks.clear(); }

    inline Tokenizer &operator=(const Tokenizer &tk)
    {
        _toks = tk._toks;
        return *this;
    }

    inline const value_type &operator[](size_type i) const
    {
        if (i < _toks.size())
            return _toks[i];

        static std::string null = "";
        return null;
    }

    inline value_type &operator[](size_type i)
    {
        if (i < _toks.size())
            return _toks[i];

        static std::string null = "";
        return null;
    }

    inline void pop_front()
    {
        if (_toks.size())
            _toks.erase(_toks.begin());
    }

    inline const std::string &front() const
    {
        return _toks.front();
    }

    inline std::string &front()
    {
        return _toks.front();
    }

    inline iterator begin() { return _toks.begin(); }
    inline iterator end() { return _toks.end(); }

    inline void pop_back() { _toks.pop_back(); }

    void erase(int s, int e)
    {
        if (s == e)
            return;

        auto i = _toks.begin();
        auto n = _toks.begin();
        std::advance(i, s);
        if (i == _toks.end())
            return;

        std::advance(n, e);
        _toks.erase(i, n);
    }

private:
    container _toks;
};

} // namespace XSpace

#endif // TOKENIZER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
