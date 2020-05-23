
#ifndef _SET_H_
#define _SET_H_

#include <set>
#include <vector>

#include "Config.h"

namespace XSpace
{

namespace Set
{
    template <typename T>
    static bool Diff(const T& oset, const T& nset, _OUT T& inc, _OUT T& dec);

    template <typename T>
    static void DiffSortedDec(const T& oset, const T& nset, _OUT T& dec)
    {
        for (auto o = oset.begin(); o != oset.end(); ++o)
        {
            bool in = false;
            typename T::const_reference v = *o;

            for (auto n = nset.begin(); n != nset.end(); ++n)
            {
                typename T::const_reference vv = *n;
                if (v <= vv)
                {
                    if (v == vv)
                        in = true;

                    break;
                }
            }

            if (!in)
                dec.insert(dec.end(), v);
        }
    }

    template <typename T>
    static bool Diff(const T& oset, const T& nset, _OUT T& inc, _OUT T& dec)
    {
        inc.clear();
        dec.clear();

        size_t os = oset.size();
        size_t ns = nset.size();

        if (!os && !ns)
            return false;

        if (!os && ns)
        {
            inc = nset;
            return true;
        }

        if (os && !ns)
        {
            dec = oset;
            return true;
        }

        T ooset = oset;
        T nnset = nset;

        std::sort(ooset.begin(), ooset.end());
        std::sort(nnset.begin(), nnset.end());

        DiffSortedDec(ooset, nnset, dec);
        DiffSortedDec(nnset, ooset, inc);

        return inc.size() || dec.size();
    }

    template <typename T>
    static bool Diff(const std::set<T>& oset, const std::set<T>& nset, _OUT std::set<T>& inc, _OUT std::set<T>& dec)
    {
        inc.clear();
        dec.clear();

        size_t os = oset.size();
        size_t ns = nset.size();

        if (!os && !ns)
            return false;

        if (!os && ns)
        {
            inc = nset;
            return true;
        }

        if (os && !ns)
        {
            dec = oset;
            return true;
        }

        std::set<T> ooset = oset;
        std::set<T> nnset = nset;

        DiffSortedDec(ooset, nnset, dec);
        DiffSortedDec(nnset, ooset, inc);

        return inc.size() || dec.size();
    }

    template <typename T>
    static void Merge(const std::vector<T>& oset, const std::vector<T>& nset, std::vector<T>& ret)
    {
        ret.clear();
        size_t os = oset.size();
        size_t ns = nset.size();
        if (!os && !ns)
            return;

        if (!os && ns)
            {
                ret = nset;
                return;
            }

        if (os && !ns)
        {
            ret = oset;
            return;
        }

        std::vector<T> ooset = oset;
        std::vector<T> nnset = nset;
        std::sort(ooset.begin(), ooset.end());
        std::sort(nnset.begin(), nnset.end());

        size_t i = 0;
        size_t j = 0;
        for (; i < os && j < ns; )
        {
            const T& ov = ooset[i];
            const T& nv = nnset[j];
            if (ov == nv)
            {
                ++i;
                ++j;
                ret.push_back(ov);
            }
            else if (ov < nv)
            {
                ++i;
                ret.push_back(ov);
            }
            else
            {
                ++j;
                ret.push_back(nv);
            }
        }

        if (i < ooset.size())
        {
            auto it = ooset.begin();
            std::advance(it, i);
            ret.insert(ret.end(), it, ooset.end());
        }
    
        if (j < nnset.size())
        {
            auto it = nnset.begin();
            std::advance(it, j);
            ret.insert(ret.end(), it, nnset.end());
        }

        return;
    }

    // XXX: oset会被替换成nset
    template <typename T>
    static bool Replace(std::vector<T>& oset, const std::vector<T>& nset, std::vector<T>& inc, std::vector<T>& dec)
    {
        if (!Diff(oset, nset, inc, dec))
            return false;
            
        oset = nset;
        return false;
    }

} // namespace Set

} // namespace XSpace

#endif // _SET_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
