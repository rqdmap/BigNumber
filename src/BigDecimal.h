#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include "rqdmap.h"

using namespace std;

#define ceil(a, b) ((a + b - 1) / b)

typedef unsigned char int8;

struct BigDecimal{
private:
    int8 *num;
    int length, signal;

    void unsignedAdd(const BigDecimal &a, const BigDecimal &b, BigDecimal &res)const;
    void unsignedSub(const BigDecimal &a, const BigDecimal &b, BigDecimal &res)const;

public:
    BigDecimal();
    BigDecimal(int);
    BigDecimal(long long);
    BigDecimal(const char*);
    BigDecimal(const BigDecimal&);
    ~BigDecimal();

    friend BigDecimal specialDecimalAdd(BigDecimal &a, int8 x);

    void show()const;
    void show(const BigDecimal&)const;

    void operator = (const BigDecimal&);

    BigDecimal operator + (const BigDecimal&)const;
    BigDecimal operator - (const BigDecimal&)const;
    void operator += (const BigDecimal &);
    void operator -= (const BigDecimal &);

    bool below(cosnt BigDecimal&)const;
    bool above(cosnt BigDecimal&)const;
    bool equal(const BigDecimal&)const; 


    bool operator < (const BigDecimal&)const;
    bool operator > (const BigDecimal&)const;
    bool operator == (const BigDecimal&)const;
    bool operator <= (const BigDecimal&)const;
    bool operator >= (const BigDecimal&)const;
};

