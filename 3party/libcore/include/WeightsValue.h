
#ifndef _WEIGHTSVALUE_H_
#define _WEIGHTSVALUE_H_

#include "Random.h"
#include <vector>

namespace XSpace
{

class WeightsValue
{
public:
    typedef std::vector<long> WeightsContainer_t;

public:
    WeightsValue() : Total(0) {}

    void Init(const WeightsContainer_t &weights)
    {
        for (int i = 0; i < weights.size(); ++i)
        {
            auto &v = weights[i];
            Weights.push_back(v);
            Total += v;
        }

        if (!Weights.size())
            Weights.push_back(0);
    }

    int WeightsIdx(long weight) const
    {
        if (weight >= Total || !Weights.size())
            return 0;

        for (int i = 0; i < Weights.size(); ++i)
        {
            weight -= Weights[i];
            if (weight <= 0)
                return i;
        }

        return 0;
    }

    int RandWeightsIdx() const
    {
        if (Total <= 0)
            return 0;

        long w = Random::Rand(0, Total);
        return WeightsIdx(w);
    }

    long Total;
    WeightsContainer_t Weights;
};

} // namespace XSpace

#endif // _WEIGHTSVALUE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
