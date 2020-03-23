
#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <string>
#include <map>

namespace XSpace
{

class Tokenizer;

class Options
{
public:
    static std::string null;

public:
    Options() {}
    ~Options() {}

    // 返回遇到了停止时的位置 >= 0 所有有停止了
    int Parse(int argc, char* argv[], const char* stop = NULL);
    int Parse(const Tokenizer& opts, const char* stop = NULL);
    inline void Reset() { _opts.clear(); }
    inline int Size() const { return _opts.size(); }

    const std::string& operator[] (const char* opt) const;
    const std::string& operator[] (const std::string& opt) const;
    bool HasOption(const std::string& opt) const;

private:
    std::map<std::string, std::string> _opts;
};

} // namespace XSpace

#endif // _OPTIONS_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

