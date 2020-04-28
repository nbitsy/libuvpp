
#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include <math.h>
#include <sstream>
#include <vector>

#include "Stream.h"
#include "TypeTraits.h"

// XXX: 使用除法时要当心，自己去确保不会除0
// XXX: 序列化时是什么类型的数据，反序列化时也应该用什么类型来装，否则结果不可预测

namespace XSpace
{

template <typename V>
struct TVector2
{
    TVector2() : X(0), Y(0) {}
    TVector2(TVector2 &other) : X(other.X), Y(other.Y) {}
    TVector2(const TVector2 &other) : X(other.X), Y(other.Y) {}
    template <typename U>
    TVector2(TVector2<U> &other) : X(other.X), Y(other.Y) {}
    template <typename U>
    TVector2(const TVector2<U> &other) : X(other.X), Y(other.Y) {}
    template <typename X, typename Y>
    TVector2(X x, Y y) : X(x), Y(y) {}

    inline TVector2 &operator=(const TVector2 &other)
    {
        this->X = other.X;
        this->Y = other.Y;
        return *this;
    }

    template <typename XY>
    inline TVector2 &operator=(const TVector2<XY> &other)
    {
        this->X = other.X;
        this->Y = other.Y;
        return *this;
    }

    inline bool IsZero() const { return this->X == 0 && this->Y == 0; }
    static inline TVector2 Zero() { return TVector2(0, 0); }
    inline void Reset() { this->X = this->Y = 0; }

    inline TVector2 operator+(const TVector2 &other) { return TVector2(this->X + other.X, this->Y + other.Y); }
    inline TVector2 operator-(const TVector2 &other) { return TVector2(this->X - other.X, this->Y - other.Y); }
    inline TVector2 operator*(const TVector2 &other) { return TVector2(this->X - other.X, this->Y * other.Y); }
    inline TVector2 operator/(const TVector2 &other) { return TVector2(this->X * other.X, this->Y / other.Y); }

    template <typename XY>
    inline TVector2 operator+(const TVector2<XY> &other) { return TVector2(this->X + other.X, this->Y + other.Y); }
    template <typename XY>
    inline TVector2 operator-(const TVector2<XY> &other) { return TVector2(this->X - other.X, this->Y - other.Y); }
    template <typename XY>
    inline TVector2 operator*(const TVector2<XY> &other) { return TVector2(this->X * other.X, this->Y * other.Y); }
    template <typename XY>
    inline TVector2 operator/(const TVector2<XY> &other) { return TVector2(this->X / other.X, this->Y / other.Y); }

    inline TVector2 &operator+=(const TVector2 &other)
    {
        this->X += other.X, this->Y += other.Y;
        return *this;
    }
    inline TVector2 &operator-=(const TVector2 &other)
    {
        this->X -= other.X, this->Y -= other.Y;
        return *this;
    }
    inline TVector2 &operator*=(const TVector2 &other)
    {
        this->X -= other.X, this->Y *= other.Y;
        return *this;
    }
    inline TVector2 &operator/=(const TVector2 &other)
    {
        this->X *= other.X, this->Y /= other.Y;
        return *this;
    }

    template <typename XY>
    inline TVector2 &operator+=(const TVector2<XY> &other)
    {
        this->X += other.X, this->Y += other.Y;
        return *this;
    }
    template <typename XY>
    inline TVector2 &operator-=(const TVector2<XY> &other)
    {
        this->X -= other.X, this->Y -= other.Y;
        return *this;
    }
    template <typename XY>
    inline TVector2 &operator*=(const TVector2<XY> &other)
    {
        this->X *= other.X, this->Y *= other.Y;
        return *this;
    }
    template <typename XY>
    inline TVector2 &operator/=(const TVector2<XY> &other)
    {
        this->X /= other.X, this->Y /= other.Y;
        return *this;
    }

    template <typename F>
    inline TVector2 &operator+=(F f)
    {
        this->X += f, this->Y += f;
        return *this;
    }
    template <typename F>
    inline TVector2 &operator-=(F f)
    {
        this->X -= f, this->Y -= f;
        return *this;
    }
    template <typename F>
    inline TVector2 &operator*=(F f)
    {
        this->X *= f, this->Y *= f;
        return *this;
    }
    template <typename F>
    inline TVector2 &operator/=(F f)
    {
        this->X /= f, this->Y /= f;
        return *this;
    }

    const std::string ToString() const
    {
        std::stringstream ss;
        ss << "(" << X << "," << Y << ")";
        return ss.str();
    }

    V X;
    V Y;
};

typedef TVector2<int> IVector2;
typedef TVector2<float> FVector2;
typedef FVector2 Vector2;

template <typename T>
static inline IOStream &operator<<(IOStream &stream, const TVector2<T> &v)
{
    stream << v.X << v.Y;
    return stream;
}
template <typename T>
static inline OStream &operator<<(OStream &stream, const TVector2<T> &v)
{
    stream << v.X << v.Y;
    return stream;
}
template <typename T>
static inline IOStream &operator>>(IOStream &stream, TVector2<T> &v)
{
    stream >> v.X >> v.Y;
    return stream;
}
template <typename T>
static inline IStream &operator>>(IStream &stream, TVector2<T> &v)
{
    stream >> v.X >> v.Y;
    return stream;
}

template <typename V>
struct TVVector2
{
    TVVector2() {}

    template <typename F>
    TVVector2 operator+(F f)
    {
        TVVector2 vv;
        std::for_each(Vector.begin(), Vector.end(), [&f, &vv](TVector2<V> &v) { vv.Vector.push_back(v + f); });
        return vv;
    }
    template <typename F>
    TVVector2 operator-(F f)
    {
        TVVector2 vv;
        std::for_each(Vector.begin(), Vector.end(), [&f, &vv](TVector2<V> &v) { vv.Vector.push_back(v - f); });
        return vv;
    }
    template <typename F>
    TVVector2 operator*(F f)
    {
        TVVector2 vv;
        std::for_each(Vector.begin(), Vector.end(), [&f, &vv](TVector2<V> &v) { vv.Vector.push_back(v * f); });
        return vv;
    }
    template <typename F>
    TVVector2 operator/(F f)
    {
        TVVector2 vv;
        std::for_each(Vector.begin(), Vector.end(), [&f, &vv](TVector2<V> &v) { vv.Vector.push_back(v / f); });
        return vv;
    }

    template <typename F>
    TVVector2 &operator+=(F f)
    {
        std::for_each(Vector.begin(), Vector.end(), [&f](TVector2<V> &v) { v += f; });
        return *this;
    }
    template <typename F>
    TVVector2 &operator-=(F f)
    {
        std::for_each(Vector.begin(), Vector.end(), [&f](TVector2<V> &v) { v -= f; });
        return *this;
    }
    template <typename F>
    TVVector2 &operator*=(F f)
    {
        std::for_each(Vector.begin(), Vector.end(), [&f](TVector2<V> &v) { v *= f; });
        return *this;
    }
    template <typename F>
    TVVector2 &operator/=(F f)
    {
        std::for_each(Vector.begin(), Vector.end(), [&f](TVector2<V> &v) { v /= f; });
        return *this;
    }

    TVVector2 operator+(const Vector2 &other)
    {
        TVVector2 vv;
        std::for_each(Vector.begin(), Vector.end(), [&other, &vv](TVector2<V> &v) { vv.Vector.push_back(v + other); });
        return vv;
    }
    TVVector2 operator-(const Vector2 &other)
    {
        TVVector2 vv;
        std::for_each(Vector.begin(), Vector.end(), [&other, &vv](TVector2<V> &v) { vv.Vector.push_back(v - other); });
        return vv;
    }
    TVVector2 operator*(const Vector2 &other)
    {
        TVVector2 vv;
        std::for_each(Vector.begin(), Vector.end(), [&other, &vv](TVector2<V> &v) { vv.Vector.push_back(v * other); });
        return vv;
    }
    TVVector2 operator/(const Vector2 &other)
    {
        TVVector2 vv;
        std::for_each(Vector.begin(), Vector.end(), [&other, &vv](TVector2<V> &v) { vv.Vector.push_back(v / other); });
        return vv;
    }

    TVVector2 &operator+=(const Vector2 &other)
    {
        std::for_each(Vector.begin(), Vector.end(), [&other](TVector2<V> &v) { v += other; });
        return *this;
    }
    TVVector2 &operator-=(const Vector2 &other)
    {
        std::for_each(Vector.begin(), Vector.end(), [&other](TVector2<V> &v) { v -= other; });
        return *this;
    }
    TVVector2 &operator*=(const Vector2 &other)
    {
        std::for_each(Vector.begin(), Vector.end(), [&other](TVector2<V> &v) { v *= other; });
        return *this;
    }
    TVVector2 &operator/=(const Vector2 &other)
    {
        std::for_each(Vector.begin(), Vector.end(), [&other](TVector2<V> &v) { v /= other; });
        return *this;
    }

    template <typename XY>
    TVVector2 operator+(const TVector2<XY> &other)
    {
        TVVector2 vv;
        std::for_each(Vector.begin(), Vector.end(), [&other, &vv](TVector2<V> &v) { vv.Vector.push_back(v + other); });
        return vv;
    }
    template <typename XY>
    TVVector2 operator-(const TVector2<XY> &other)
    {
        TVVector2 vv;
        std::for_each(Vector.begin(), Vector.end(), [&other, &vv](TVector2<V> &v) { vv.Vector.push_back(v - other); });
        return vv;
    }
    template <typename XY>
    TVVector2 operator*(const TVector2<XY> &other)
    {
        TVVector2 vv;
        std::for_each(Vector.begin(), Vector.end(), [&other, &vv](TVector2<V> &v) { vv.Vector.push_back(v * other); });
        return vv;
    }
    template <typename XY>
    TVVector2 operator/(const TVector2<XY> &other)
    {
        TVVector2 vv;
        std::for_each(Vector.begin(), Vector.end(), [&other, &vv](TVector2<V> &v) { vv.Vector.push_back(v / other); });
        return vv;
    }

    template <typename XY>
    TVVector2 &operator+=(const TVector2<XY> &other)
    {
        std::for_each(Vector.begin(), Vector.end(), [&other](TVector2<V> &v) { v += other; });
        return *this;
    }
    template <typename XY>
    TVVector2 &operator-=(const TVector2<XY> &other)
    {
        std::for_each(Vector.begin(), Vector.end(), [&other](TVector2<V> &v) { v -= other; });
        return *this;
    }
    template <typename XY>
    TVVector2 &operator*=(const TVector2<XY> &other)
    {
        std::for_each(Vector.begin(), Vector.end(), [&other](TVector2<V> &v) { v *= other; });
        return *this;
    }
    template <typename XY>
    TVVector2 &operator/=(const TVector2<XY> &other)
    {
        std::for_each(Vector.begin(), Vector.end(), [&other](TVector2<V> &v) { v /= other; });
        return *this;
    }

    std::string ToString() const
    {
        std::stringstream ss;
        ss << "[" << Vector.size() << "] =";
        std::for_each(Vector.begin(), Vector.end(), [&ss](const TVector2<V> &v) {
            ss << v.ToString() << ',';
        });
        return ss.str();
    }

    template <typename U>
    inline void PushBack(const TVector2<U> &v) { Vector.push_back(v); }
    inline void Swap(std::vector<TVector2<V>> &v) { Vector.swap(v); }

    std::vector<TVector2<V>> Vector;
};

typedef TVVector2<int> IVVector2;
typedef TVVector2<float> FVVector2;
typedef FVVector2 VVector2;

template <typename T>
static inline IOStream &operator<<(IOStream &stream, const TVVector2<T> &v)
{
    unsigned short count = v.Vector.size();
    stream << count;
    if (count > 0)
    {
        std::for_each(v.Vector.begin(), v.Vector.end(), [&stream](const TVector2<T> &v) {
            ((OStream &)stream) << v;
        });
    }
    return stream;
}
template <typename T>
static inline OStream &operator<<(OStream &stream, const TVVector2<T> &v)
{
    unsigned short count = v.Vector.size();
    stream << count;
    if (count > 0)
    {
        std::for_each(v.Vector.begin(), v.Vector.end(), [&stream](const TVector2<T> &v) {
            stream << v;
        });
    }
    return stream;
}
template <typename T>
static inline IOStream &operator>>(IOStream &stream, TVVector2<T> &v)
{
    unsigned short count;
    stream >> count;
    if (count > 0)
    {
        TVector2<T> v2;
        for (int i = 0; i < count; ++i)
        {
            (IStream &)stream >> v2;
            v.Vector.push_back(v2);
        }
    }
    return stream;
}
template <typename T>
static inline IStream &operator>>(IStream &stream, TVVector2<T> &v)
{
    unsigned short count;
    stream >> count;
    if (count > 0)
    {
        TVector2<T> v2;
        for (int i = 0; i < count; ++i)
        {
            stream >> v2;
            v.Vector.push_back(v2);
        }
    }
    return stream;
}

inline bool ApproximateEqual(double a, double b, float deviation = 0.0001)
{
    return (a >= b - deviation && a <= b + deviation);
}

// 比较函数
template <typename T>
static inline bool operator==(const TVector2<T> &a, const TVector2<T> &b)
{
    if (&a == &b)
        return true;
    if (is_integer<T>::value)
        return a.X == b.X && a.Y == b.Y;
    else
        return ApproximateEqual(a.X, b.X) && ApproximateEqual(a.Y, b.Y);
}

template <typename T, typename U>
static inline bool operator==(const TVector2<T> &a, const TVector2<U> &b)
{
    if (is_integer<T>::value && is_integer<U>::value)
        return a.X == b.X && a.Y == b.Y;
    else
        return ApproximateEqual(a.X, b.X) && ApproximateEqual(a.Y, b.Y);
}

template <typename T, typename U>
static inline bool operator<(const TVector2<T> &a, const TVector2<U> &b)
{
    return a.X < b.X && a.Y < b.Y;
}

template <typename T, typename U>
static inline bool operator>=(const TVector2<T> &a, const TVector2<U> &b)
{
    return !operator<(a, b);
}

template <typename T, typename U>
static inline bool operator>(const TVector2<T> &a, const TVector2<U> &b)
{
    return a.X > b.X && a.Y > b.Y;
}

// 估计距离函数
static inline int Distance(const IVector2 &a, const IVector2 &b)
{
    return std::abs(a.X - b.X) + std::abs(a.Y - b.Y);
}

// 估计距离函数
template <typename T, typename U>
static inline int Distance(const TVector2<T> &a, const TVector2<U> &b)
{
    return fabs(a.X - b.X) + fabs(a.Y - b.Y);
}

// 真实距离函数
static inline double RealDistance(const IVector2 &a, const IVector2 &b)
{
    return sqrt((a.X - b.X) * (a.X - b.X) + (a.Y - b.Y) * (a.Y - b.Y));
}

// 真实距离函数
template <typename T, typename U>
static inline double RealDistance(const TVector2<T> &a, const TVector2<U> &b)
{
    return sqrt((a.X - b.X) * (a.X - b.X) + (a.Y - b.Y) * (a.Y - b.Y));
}

template <typename T, typename U>
static inline bool InRadius(const TVector2<T> &a, const TVector2<U> &b, int radius)
{
    return std::abs(a.X - b.X) <= radius && std::abs(a.Y - b.Y) <= radius;
}

template <typename T, typename U>
static inline double RealInRadius(const TVector2<T> &a, const TVector2<U> &b, double radius)
{
    return RealDistance(a, b) <= radius;
}

} // namespace XSpace

namespace std
{

template <>
struct hash<XSpace::IVector2>
{
    inline size_t operator()(const XSpace::IVector2 &v) const
    {
        return v.Y * v.X;
    }
};

} // namespace std
#endif // _VECTOR2_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
