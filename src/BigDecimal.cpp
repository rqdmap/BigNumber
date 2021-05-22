#include "BigDecimal.h"

#define blk (i / 2)
#define bit ((i & 1) << 2)
#define bytes ((length + 1) / 2)

BigDecimal::BigDecimal(){
    num = new int8[1]();
    length = 1; signal = 1;
}

BigDecimal::BigDecimal(int _x){
    signal = _x >= 0; 
    _x = _x >= 0? _x: -_x;

    if(_x == 0){
        num = new int8[1]();
        length = 1;
        return ;
    }

    int x = _x; length = 0;
    while(x){length++; x /= 10;}

    num = new int8[bytes]();
    
    x = _x;
    for(int i = 0; i < length; i++){
        num[blk] |= (x % 10) << bit;
        x /= 10;
    }
}

BigDecimal::BigDecimal(ll _x){
    signal = _x >= 0; 
    _x = _x >= 0? _x: -_x;

    if(_x == 0){
        num = new int8[1]();
        length = 1;
        return ;
    }

    ll x = _x; length = 0;
    while(x){length++; x /= 10;}

    num = new int8[bytes]();
    
    x = _x;
    for(int i = 0; i < length; i++){
        num[blk] |= (x % 10) << bit;
        x /= 10;
    }
}

BigDecimal::BigDecimal(const char *s){
    int top = (int)strlen(s);
    if(top == 0){
        num = new int8[1]();
        length = 1; signal = 1;
        return ;
    }

    signal = 1;
    if(s[0] == '-') signal = 0;

    int fail = 0;
    for(int i = signal == 0; i < top; i++){
        if(s[i] >= '0' && s[i] <= '9') continue;
        fail = 1;
        break;
    }

    if(fail){
        PRINT_FONT_RED;
        puts("Read invalid char!");
        PRINT_ATTR_REC;
        fflush(stdout);
        throw;
    }

    const char *t = s + (signal == 0);
    top -= signal == 0;

    length = top;
    num = new int8[bytes]();

    for(int i = 0; i < length; i++)
        num[blk] |= (t[length - 1 - i] - '0') << bit;
}


BigDecimal::BigDecimal(const BigDecimal &b){
    length = b.length;
    num = new int8[bytes]();
    signal = b.signal;
    for(int i = 0; i < length; i += 2) num[blk] = b.num[blk];
}

BigDecimal::~BigDecimal(){
    if(num != NULL) delete num;
}

void BigDecimal::show()const{
    if(length == 1 && num[0] == 0){
        printf("0\n");
        return ;
    }
    if(!signal) printf("-");
    for(int i = length - 1; i >= 0; i--) 
        printf("%u", (num[blk] >> bit) & 0xf);
    puts("");
}

void BigDecimal::show(const BigDecimal &b)const{
    b.show();
}


void BigDecimal::operator = (const BigDecimal &b){
    if(bytes != ceil(b.length, 2)){
        if(num != NULL) delete num;
        num = new int8[ceil(b.length, 2)]();
    }
    length = b.length;
    signal = b.signal;
    for(int i = 0; i < length; i += 2) num[blk] = b.num[blk];
}


void BigDecimal::unsignedAdd(const BigDecimal &a, const BigDecimal &b, BigDecimal &res)const{
    int l1 = a.length, l2 = b.length;
    int L = max(l1, l2) + 1;
    res.num = new int8[L]();
    
    int carry = 0;

    for(int i = 0; i < L; i++){
        int val1 = 0, val2 = 0;
        if(i < l1) val1 = (a.num[blk] >> bit) & 0xf;
        if(i < l2) val2 = (b.num[blk] >> bit) & 0xf;

        int val = val1 + val2 + carry;
        carry = 0;
        if(val >= 10){
            carry = 1;
            val %= 10;
        }

        if(val){
            res.length = i + 1;
            res.num[blk] |= val << bit;
        }
    }
    if(!res.length){
        if(res.num != NULL) delete res.num;
        res.num = new int8[1]();
        res.length = 1;
    }
}

//应当保证当前num值不小于b.num值!
void BigDecimal::unsignedSub(const BigDecimal &a, const BigDecimal &b, BigDecimal &res)const{
    int l1 = a.length, l2 = b.length, L = a.length;
    res.num = new int8[L]();

    int carry = 0;
    for(int i = 0; i < L; i++){
        int val1 = (a.num[blk] >> bit) & 0xf;
        int val2 = 0;
        if(i < b.length) val2 = (b.num[blk] >> bit) & 0xf;

        int val = val1 - carry - val2;
        carry = 0;
        if(val < 0){
            val += 10;
            carry = 1;
        }

        if(val){
            res.length = i + 1;
            res.num[blk] |= val << bit;
        }
    }
    if(!res.length){
        if(res.num != NULL) delete res.num;
        res.num = new int8[1]();
        res.length = 1;
    }
}

BigDecimal BigDecimal::operator + (const BigDecimal &b)const{
    BigDecimal res;

    if(signal == b.signal){
        unsignedAdd(*this, b, res);
        res.signal = signal;
    }
    else{
        int sigA = signal, sigB = b.signal;
        int flag = this->below(b);
        if(signal){
            if(flag){res.signal = 0; unsignedSub(b, *this, res);}
            else{res.signal = 1; unsignedSub(*this, b, res);}
        }
        else{
            if(flag){res.signal = 1; unsignedSub(b, *this, res);}
            else{res.signal = 0; unsignedSub(*this, b, res);}
        }
    }
    return res;
}


void BigDecimal::operator += (const BigDecimal &b){
    *this = *this + b;
}

BigDecimal BigDecimal::operator - (const BigDecimal &b)const{
    BigDecimal bb(b);
    bb.signal = !bb.signal;
    return *this + bb;
}

void BigDecimal::operator -= (const BigDecimal &b){
    *this = *this - b;
}

bool BigDecimal::below(cosnt BigDecimal &b)const{
    if(length < b.length) return true;
    else if(length > b.length) return false;
    else for(int i = length - 1; i >= 0; i -= 2){
        if(num[blk] == b.num[blk]) continue;
        return num[blk] < b.num[blk];
    }
    return false;
}

bool BigDecimal::above(cosnt BigDecimal &b)const{
    return b.below(*this);
}

bool BigDecimal::equal(cosnt BigDecimal &b)const{
    return !(this->below(b)) && !(b.below(*this));
}

bool BigDecimal::operator < (const BigDecimal& b)const{
    if(signal && !b.signal) return false;
    else if(!signal && b.signal) return true;
    else if(signal && b.signal) return this->below(b);
    else return this->above(b);
}

bool BigDecimal::operator > (const BigDecimal& b)const{
    return b < *this;
}

bool BigDecimal::operator == (const BigDecimal& b)const{
    return !(b < *this) && !(*this < b);
}

bool BigDecimal::operator <= (const BigDecimal& b)const{
    return *this < b || *this == b;
}

bool BigDecimal::operator >= (const BigDecimal& b)const{
    return *this > b || *this == b;
}

/*
    return a * 256 + x;
    (Temperarily only used in show10() for speeding up)
*/
BigDecimal specialDecimalAdd(BigDecimal &a, int8 x){
    // DEBUG; a.show(); printf("%d\n", x);
    BigDecimal res;
    res.length = a.length + 3;
    res.num = new int8[ceil(res.length, 2)]();
    res.signal = 1;

    int8 val1, val2, val3, val, carry = 0;
    for(int i = 0; i < res.length; i++){
        val1 = 0; if(i < a.length) val1 = ((a.num[i / 2] >> ((i & 1) << 2)) & 0xf) * 6;
        val2 = 0; if(i - 1 >= 0 && (i - 1) < a.length) val2 = ((a.num[(i - 1) / 2] >> (((i - 1) & 1) << 2)) & 0xf) * 5;
        val3 = 0; if(i - 2 >= 0 && (i - 2) < a.length) val3 = ((a.num[(i - 2) / 2] >> (((i - 2) & 1) << 2)) & 0xf) * 2;

        val = val1 + val2 + val3 + carry; carry = 0;

        // printf("%d %d %d %d %d\n", i, val1, val2, val3, val);

        carry = val / 10; val %= 10;
        res.num[blk] |= val << bit;
    }
    for(int i = res.length - 1; i >= 0; i--){
        if((res.num[blk] >> bit) & 0xf) break;
        res.length--;
    }
    // printf("%d: %d %d\n", res.length, res.num[0], res.num[1]);
    // DEBUG; (res + x).show();
    // exit(0);
    return res + x;
}