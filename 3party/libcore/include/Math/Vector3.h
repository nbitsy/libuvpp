
#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include "Stream.h"
#include "Vector2.h"
#include <sstream>

// XXX: 使用除法时要当心，自己去确保不会除0

namespace XSpace
{

template <typename V>
struct TVector3
{
    TVector3() : X(0), Y(0), Z(0) {}
    TVector3(TVector3 &other) : X(other.X), Y(other.Y), Z(other.Z) {}
    TVector3(const TVector3 &other) : X(other.X), Y(other.Y), Z(other.Z) {}
    template <typename U>
    TVector3(TVector3<U> &other) : X(other.X), Y(other.Y), Z(other.Z) {}
    template <typename U>
    TVector3(const TVector3<U> &other) : X(other.X), Y(other.Y), Z(other.Z) {}
    template <typename X, typename Y, typename Z>
    TVector3(X x, Y y, Z z) : X(x), Y(y), Z(z) {}
    template <typename U>
    TVector3(TVector2<U> &other) : X(other.X), Y(other.Y), Z(0) {}

    template <typename XYZ>
    inline TVector3 &operator=(const TVector3<XYZ> &other)
    {
        this->X = other.X;
        this->Y = other.Y;
        this->Z = other.Z;
        return *this;
    }

    template <typename XY>
    inline TVector3 &operator=(const TVector2<XY> &other)
    {
        this->X = other.X;
        this->Y = other.Y;
        this->Z = 0;
        return *this;
    }

    template <typename T>
    inline TVector3<T> operator()(TVector2<T> &v)
    {
        return TVector3(v.X, v.Y, 0);
    }

    inline bool IsZero() const { return this->X == 0 && this->Y == 0 && this->Z == 0; }
    static inline TVector3 Zero() { return TVector3(0, 0, 0); }
    inline void Reset() { this->X = this->Y = this->Z = 0; }

    template <typename T>
    inline const TVector2<T> _2() const { return TVector2<T>(this->X, this->Y); }
    inline const IVector2 I2() const { return IVector2(this->X, this->Y); }
    inline const FVector2 F2() const { return FVector2(this->X, this->Y); }

    inline TVector3 operator+(const TVector3 &other) { return TVector3(this->X + other.X, this->Y + other.Y, this->Z + other.Z); }
    inline TVector3 operator-(const TVector3 &other) { return TVector3(this->X - other.X, this->Y - other.Y, this->Z - other.Z); }
    inline TVector3 operator*(const TVector3 &other) { return TVector3(this->X - other.X, this->Y * other.Y, this->Z * other.Z); }
    inline TVector3 operator/(const TVector3 &other) { return TVector3(this->X * other.X, this->Y / other.Y, this->Z / other.Z); }

    template <typename XY>
    inline TVector3 operator+(const TVector3<XY> &other) { return TVector3(this->X + other.X, this->Y + other.Y, this->Z + other.Z); }
    template <typename XY>
    inline TVector3 operator-(const TVector3<XY> &other) { return TVector3(this->X - other.X, this->Y - other.Y, this->Z - other.Z); }
    template <typename XY>
    inline TVector3 operator*(const TVector3<XY> &other) { return TVector3(this->X * other.X, this->Y * other.Y, this->Z * other.Z); }
    template <typename XY>
    inline TVector3 operator/(const TVector3<XY> &other) { return TVector3(this->X / other.X, this->Y / other.Y, this->Z / other.Z); }

    inline TVector3 &operator+=(const TVector3 &other)
    {
        this->X += other.X, this->Y += other.Y;
        this->Z += this->Z;
        return *this;
    }
    inline TVector3 &operator-=(const TVector3 &other)
    {
        this->X -= other.X, this->Y -= other.Y;
        this->Z -= this->Z;
        return *this;
    }
    inline TVector3 &operator*=(const TVector3 &other)
    {
        this->X -= other.X, this->Y *= other.Y;
        this->Z *= this->Z;
        return *this;
    }
    inline TVector3 &operator/=(const TVector3 &other)
    {
        this->X *= other.X, this->Y /= other.Y;
        this->Z /= this->Z;
        return *this;
    }

    template <typename XY>
    inline TVector3 &operator+=(const TVector3<XY> &other)
    {
        this->X += other.X, this->Y += other.Y;
        this->Z += other.Z;
        return *this;
    }
    template <typename XY>
    inline TVector3 &operator-=(const TVector3<XY> &other)
    {
        this->X -= other.X, this->Y -= other.Y;
        this->Z -= other.Z;
        return *this;
    }
    template <typename XY>
    inline TVector3 &operator*=(const TVector3<XY> &other)
    {
        this->X *= other.X, this->Y *= other.Y;
        this->Z *= other.Z;
        return *this;
    }
    template <typename XY>
    inline TVector3 &operator/=(const TVector3<XY> &other)
    {
        this->X /= other.X, this->Y /= other.Y;
        this->Z /= other.Z;
        return *this;
    }

    template <typename F>
    inline TVector3 &operator+=(F f)
    {
        this->X += f, this->Y += f;
        this->Z += f;
        return *this;
    }
    template <typename F>
    inline TVector3 &operator-=(F f)
    {
        this->X -= f, this->Y -= f;
        this->Z -= f;
        return *this;
    }
    template <typename F>
    inline TVector3 &operator*=(F f)
    {
        this->X *= f, this->Y *= f;
        this->Z *= f;
        return *this;
    }
    template <typename F>
    inline TVector3 &operator/=(F f)
    {
        this->X /= f, this->Y /= f;
        this->Z /= f;
        return *this;
    }

    const std::string ToString() const
    {
        std::stringstream ss;
        ss << "(" << X << "," << Y << "," << Z << ")";
        return ss.str();
    }

    V X;
    V Y;
    V Z;
};

typedef TVector3<int> IVector3;
typedef TVector3<float> FVector3;
typedef FVector3 Vector3;

template <typename T>
static inline IOStream &operator<<(IOStream &stream, const TVector3<T> &v)
{
    stream << v.X << v.Y << v.Z;
    return stream;
}
template <typename T>
static inline OStream &operator<<(OStream &stream, const TVector3<T> &v)
{
    stream << v.X << v.Y << v.Z;
    return stream;
}
template <typename T>
static inline IOStream &operator>>(IOStream &stream, TVector3<T> &v)
{
    stream >> v.X >> v.Y >> v.Z;
    return stream;
}
template <typename T>
static inline IStream &operator>>(IStream &stream, TVector3<T> &v)
{
    stream >> v.X >> v.Y >> v.Z;
    return stream;
}

template <typename V>
struct TVVector3
{
    TVVector3() {}

    template <typename F>
    TVVector3 operator+(F f)
    {
        TVVector3 vv;
        std::for_each(Vector.begin(), Vector.end(), [&f, &vv](TVector3<V> &v) { vv.Vector.push_back(v + f); });
        return vv;
    }
    template <typename F>
    TVVector3 operator-(F f)
    {
        TVVector3 vv;
        std::for_each(Vector.begin(), Vector.end(), [&f, &vv](TVector3<V> &v) { vv.Vector.push_back(v - f); });
        return vv;
    }
    template <typename F>
    TVVector3 operator*(F f)
    {
        TVVector3 vv;
        std::for_each(Vector.begin(), Vector.end(), [&f, &vv](TVector3<V> &v) { vv.Vector.push_back(v * f); });
        return vv;
    }
    template <typename F>
    TVVector3 operator/(F f)
    {
        TVVector3 vv;
        std::for_each(Vector.begin(), Vector.end(), [&f, &vv](TVector3<V> &v) { vv.Vector.push_back(v / f); });
        return vv;
    }

    template <typename F>
    TVVector3 &operator+=(F f)
    {
        std::for_each(Vector.begin(), Vector.end(), [&f](TVector3<V> &v) { v += f; });
        return *this;
    }
    template <typename F>
    TVVector3 &operator-=(F f)
    {
        std::for_each(Vector.begin(), Vector.end(), [&f](TVector3<V> &v) { v -= f; });
        return *this;
    }
    template <typename F>
    TVVector3 &operator*=(F f)
    {
        std::for_each(Vector.begin(), Vector.end(), [&f](TVector3<V> &v) { v *= f; });
        return *this;
    }
    template <typename F>
    TVVector3 &operator/=(F f)
    {
        std::for_each(Vector.begin(), Vector.end(), [&f](TVector3<V> &v) { v /= f; });
        return *this;
    }

    TVVector3 operator+(const Vector3 &other)
    {
        TVVector3 vv;
        std::for_each(Vector.begin(), Vector.end(), [&other, &vv](TVector3<V> &v) { vv.Vector.push_back(v + other); });
        return vv;
    }
    TVVector3 operator-(const Vector3 &other)
    {
        TVVector3 vv;
        std::for_each(Vector.begin(), Vector.end(), [&other, &vv](TVector3<V> &v) { vv.Vector.push_back(v - other); });
        return vv;
    }
    TVVector3 operator*(const Vector3 &other)
    {
        TVVector3 vv;
        std::for_each(Vector.begin(), Vector.end(), [&other, &vv](TVector3<V> &v) { vv.Vector.push_back(v * other); });
        return vv;
    }
    TVVector3 operator/(const Vector3 &other)
    {
        TVVector3 vv;
        std::for_each(Vector.begin(), Vector.end(), [&other, &vv](TVector3<V> &v) { vv.Vector.push_back(v / other); });
        return vv;
    }

    TVVector3 &operator+=(const Vector3 &other)
    {
        std::for_each(Vector.begin(), Vector.end(), [&other](TVector3<V> &v) { v += other; });
        return *this;
    }
    TVVector3 &operator-=(const Vector3 &other)
    {
        std::for_each(Vector.begin(), Vector.end(), [&other](TVector3<V> &v) { v -= other; });
        return *this;
    }
    TVVector3 &operator*=(const Vector3 &other)
    {
        std::for_each(Vector.begin(), Vector.end(), [&other](TVector3<V> &v) { v *= other; });
        return *this;
    }
    TVVector3 &operator/=(const Vector3 &other)
    {
        std::for_each(Vector.begin(), Vector.end(), [&other](TVector3<V> &v) { v /= other; });
        return *this;
    }

    template <typename XY>
    TVVector3 operator+(const TVector3<XY> &other)
    {
        TVVector3 vv;
        std::for_each(Vector.begin(), Vector.end(), [&other, &vv](TVector3<V> &v) { vv.Vector.push_back(v + other); });
        return vv;
    }
    template <typename XY>
    TVVector3 operator-(const TVector3<XY> &other)
    {
        TVVector3 vv;
        std::for_each(Vector.begin(), Vector.end(), [&other, &vv](TVector3<V> &v) { vv.Vector.push_back(v - other); });
        return vv;
    }
    template <typename XY>
    TVVector3 operator*(const TVector3<XY> &other)
    {
        TVVector3 vv;
        std::for_each(Vector.begin(), Vector.end(), [&other, &vv](TVector3<V> &v) { vv.Vector.push_back(v * other); });
        return vv;
    }
    template <typename XY>
    TVVector3 operator/(const TVector3<XY> &other)
    {
        TVVector3 vv;
        std::for_each(Vector.begin(), Vector.end(), [&other, &vv](TVector3<V> &v) { vv.Vector.push_back(v / other); });
        return vv;
    }

    template <typename XY>
    TVVector3 &operator+=(const TVector3<XY> &other)
    {
        std::for_each(Vector.begin(), Vector.end(), [&other](TVector3<V> &v) { v += other; });
        return *this;
    }
    template <typename XY>
    TVVector3 &operator-=(const TVector3<XY> &other)
    {
        std::for_each(Vector.begin(), Vector.end(), [&other](TVector3<V> &v) { v -= other; });
        return *this;
    }
    template <typename XY>
    TVVector3 &operator*=(const TVector3<XY> &other)
    {
        std::for_each(Vector.begin(), Vector.end(), [&other](TVector3<V> &v) { v *= other; });
        return *this;
    }
    template <typename XY>
    TVVector3 &operator/=(const TVector3<XY> &other)
    {
        std::for_each(Vector.begin(), Vector.end(), [&other](TVector3<V> &v) { v /= other; });
        return *this;
    }

    std::string ToString() const
    {
        std::stringstream ss;
        ss << "[" << Vector.size() << "] =";
        std::for_each(Vector.begin(), Vector.end(), [&ss](const TVector3<V> &v) {
            ss << v.ToString() << ',';
        });
        return ss.str();
    }

    template <typename U>
    inline void PushBack(const TVector3<U> &v) { Vector.push_back(v); }
    inline void Swap(std::vector<TVector3<V>> &v) { Vector.swap(v); }

    std::vector<TVector3<V>> Vector;
};

typedef TVVector3<int> IVVector3;
typedef TVVector3<float> FVVector3;
typedef FVVector3 VVector3;

template <typename T>
static inline IOStream &operator<<(IOStream &stream, const TVVector3<T> &v)
{
    unsigned short count = v.Vector.size();
    stream << count;
    if (count > 0)
    {
        std::for_each(v.Vector.begin(), v.Vector.end(), [&stream](const TVector3<T> &v) {
            ((OStream &)stream) << v;
        });
    }
    return stream;
}
template <typename T>
static inline OStream &operator<<(OStream &stream, const TVVector3<T> &v)
{
    unsigned short count = v.Vector.size();
    stream << count;
    if (count > 0)
    {
        std::for_each(v.Vector.begin(), v.Vector.end(), [&stream](const TVector3<T> &v) {
            stream << v;
        });
    }
    return stream;
}
template <typename T>
static inline IOStream &operator>>(IOStream &stream, TVVector3<T> &v)
{
    unsigned short count;
    stream >> count;
    if (count > 0)
    {
        TVector3<T> v3;
        for (int i = 0; i < count; ++i)
        {
            (IStream &)stream >> v3;
            v.Vector.push_back(v3);
        }
    }
    return stream;
}
template <typename T>
static inline IStream &operator>>(IStream &stream, TVVector3<T> &v)
{
    unsigned short count;
    stream >> count;
    if (count > 0)
    {
        TVector3<T> v3;
        for (int i = 0; i < count; ++i)
        {
            stream >> v3;
            v.Vector.push_back(v3);
        }
    }
    return stream;
}

// 比较函数
template <typename T>
static inline bool operator==(const TVector3<T> &a, const TVector3<T> &b)
{
    if (&a == &b)
        return true;
    if (is_integer<T>::value)
        return a.X == b.X && a.Y == b.Y && a.Z == b.Z;
    else
        return ApproximateEqual(a.X, b.X) && ApproximateEqual(a.Y, b.Y) && ApproximateEqual(a.Z, b.Z);
}

template <typename T, typename U>
static inline bool operator==(const TVector3<T> &a, const TVector3<U> &b)
{
    if (&a == &b)
        return true;
    if (is_integer<T>::value && is_integer<U>::value)
        return a.X == b.X && a.Y == b.Y && a.Z == b.Z;
    else
        return ApproximateEqual(a.X, b.X) && ApproximateEqual(a.Y, b.Y) && ApproximateEqual(a.Z, b.Z);
}

template <typename T, typename U>
static inline bool operator<(const TVector3<T> &a, const TVector3<U> &b)
{
    return a.X < b.X && a.Y < b.Y && a.Z < b.Z;
}

template <typename T, typename U>
static inline bool operator>=(const TVector3<T> &a, const TVector3<U> &b)
{
    return !operator<(a, b);
}

template <typename T, typename U>
static inline bool operator>(const TVector3<T> &a, const TVector3<U> &b)
{
    return a.X > b.X && a.Y > b.Y && a.Z > b.Z;
}

// 真实距离函数
template <typename T, typename U>
static inline double RealDistance(const TVector3<T> &a, const TVector3<U> &b)
{
    return sqrt((a.X - b.X) * (a.X - b.X) + (a.Y - b.Y) * (a.Y - b.Y) + (a.Z - b.Z) * (a.Z - b.Z));
}

template <typename T, typename U>
static inline bool InRadius(const TVector3<T> &a, const TVector3<U> &b, int radius)
{
    return std::abs(a.X - b.X) <= radius && std::abs(a.Y - b.Y) <= radius && std::abs(a.Z - b.Z) <= radius;
}

template <typename T, typename U>
static inline double RealInRadius(const TVector3<T> &a, const TVector3<U> &b, double radius)
{
    return RealDistance(a, b) <= radius;
}

} // namespace XSpace

namespace std
{

template <>
struct hash<XSpace::IVector3>
{
    inline size_t operator()(const XSpace::IVector3 &v) const
    {
        return v.X ^ v.Y * (137) ^ v.Z * 1173;
    }
};

} // namespace std

#endif // _VECTOR3_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
