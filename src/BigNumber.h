#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include "rqdmap.h"
#include "BigDecimal.h"
using namespace std;

#define ceil(a, b) ((a + b - 1) / b)

typedef unsigned char int8;
struct BigNumber{
private:
    int8 *num, signal;
    int length;

    void unsignedAdd(const BigNumber &a, const BigNumber &b, BigNumber &res)const;
    void unsignedSub(const BigNumber &a, const BigNumber &b, BigNumber &res)const;
public:
    BigNumber();
    BigNumber(int);
    BigNumber(long long);
    BigNumber(const char*);
    BigNumber(const BigNumber&);
    ~BigNumber();

    friend BigNumber RandBigNumber(int __size);

    void show()const;
    void show2()const;
    void show10()const;
    void show16()const;

    void operator = (const BigNumber&);

    BigNumber operator + (const BigNumber&)const;
    BigNumber operator - (const BigNumber&)const;
    BigNumber operator * (const BigNumber&)const;
    BigNumber operator / (const BigNumber&)const;
    BigNumber operator % (const BigNumber&)const;
    BigNumber operator & (const BigNumber&)const;
    BigNumber operator | (const BigNumber&)const;
    BigNumber operator ^ (const BigNumber&)const;
    BigNumber operator >> (int)const;
    BigNumber operator << (int)const;
    void operator += (const BigNumber &);
    void operator -= (const BigNumber &);
    void operator *= (const BigNumber &);
    void operator /= (const BigNumber &);
    void operator %= (const BigNumber &);
    void operator &= (const BigNumber&);
    void operator |= (const BigNumber&);
    void operator ^= (const BigNumber&);
    void operator >>= (int);
    void operator <<= (int);

    //compare unsigned number
    bool below(cosnt BigNumber&)const;
    bool above(cosnt BigNumber&)const;
    bool equal(const BigNumber&)const; 

    //compre signed number
    bool operator < (const BigNumber&)const;
    bool operator > (const BigNumber&)const;
    bool operator == (const BigNumber&)const;
    bool operator != (const BigNumber&)const;
    bool operator <= (const BigNumber&)const;
    bool operator >= (const BigNumber&)const;
};

void BigNumberShow(const BigNumber &);
void BigNumberShow2(const BigNumber &);
void BigNumberShow10(const BigNumber &);
void BigNumberShow16(const BigNumber &);