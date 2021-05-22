#include "BigNumber.h"

#define blk (i / 8)
#define bit (i & 0x7)
#define bytes ((length + 7) / 8)

BigNumber::BigNumber(){
    num = new int8[1]();
    length = 1; signal = 1;
}

BigNumber::BigNumber(int _x){
    
    signal = _x >= 0; 
    _x = _x >= 0? _x: -_x;

    if(_x == 0){
        num = new int8[1]();
        length = 1;
        return ;
    }

    int x = _x; length = 0;
    while(x){length++; x >>= 1;}
    
    num = new int8[bytes]();
    
    x = _x;
    for(int i = 0; i < length; i++){
        num[blk] |= (x & 1) << bit;
        x >>= 1;
    }
}

BigNumber::BigNumber(ll _x){
    signal = _x >= 0; 
    _x = _x >= 0? _x: -_x;

    if(_x == 0){
        num = new int8[1]();
        length = 1;
        return ;
    }

    ll x = _x; length = 0;
    while(x){length++; x >>= 1;}

    num = new int8[bytes]();
    
    x = _x;
    for(int i = 0; i < length; i++){
        num[blk] |= (x & 1) << bit;
        x >>= 1;
    }
}

BigNumber::BigNumber(const char *s){
    int top = (int)strlen(s);
    if(top == 0){
        num = new int8[1]();
        length = 1; signal = 1;
        return ;
    }

    signal = 1;
    if(s[0] == '-') signal = 0;

    int8 fail = 0;
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

    length = 4;
    num = new int8[bytes]();

    for(int i = 0; i < top; i++){
        *this *= 10;
        *this += t[i] - '0';
    }
}

BigNumber::BigNumber(const BigNumber&b){
    length = b.length;
    num = new int8[bytes]();
    signal = b.signal;
    for(int i = 0; i < length; i += 2) num[blk] = b.num[blk];
}

/*
    return random non-negative BigNumber with __size bits;
    If you want to generate different BigNumber, call "srand()" before.
*/
BigNumber RandBigNumber(int x){
    BigNumber res;
    res.length = x;
    res.num = new int8[ceil(x, 8)]();
    res.signal = 1;

    int temp = ceil(x, 8);
    for(int i = 0; i < temp; i++) res.num[i] = rand() % 256;
    return res;
}

BigNumber::~BigNumber(){
    if(length) delete num;
    length = 0; signal = 1;
}

void BigNumber::show2()const{
    if(length == 1 && num[0] == 0){
        printf("0\n");
        return ;
    }
    if(!signal) printf("-");
    for(int i = length - 1; i >= 0; i--){
        printf("%d", (num[blk] >> bit) & 1);
        if(i % 4 == 0) printf(" ");
    }
    printf("\n");
}

void BigNumber::show10()const{
    if(length == 1 && num[0] == 0){
        printf("0\n");
        return ;
    }
    if(!signal) printf("-");

    /*
        Naive implement...
        Quite slow...
    */
    // BigDecimal res, now(1);
    // for(int i = 0; i < length; i++){
    //     if((num[blk] >> bit) & 1) res += now;
    //     now += now;
    // }
    // res.show();

    BigDecimal res;
    int blks = ceil(length, 8);
    for(int i = blks - 1; i >= 0; i--){
        res = specialDecimalAdd(res, num[i]);
    }
    res.show();
}

void BigNumber::show16()const{
    if(length == 1 && num[0] == 0){
        printf("0\n");
        return ;
    }

    if(!signal) printf("-");
    printf("0x");
    for(int i = (length - 1) / 4 * 4; i >= 0; i -= 4){
        int val = (num[blk] >> bit) & 0xf;
        if(val <= 9) printf("%u", val);
        else printf("%c", val - 10 + 'A');
    }
    puts("");
}

void BigNumber::show()const{
    this->show10();
}

void BigNumberShow2(const BigNumber &b){b.show2();}
void BigNumberShow10(const BigNumber &b){b.show10();}
void BigNumberShow16(const BigNumber &b){b.show16();}
void BigNumberShow(const BigNumber &b){b.show();}

void BigNumber::operator = (const BigNumber &b){
    if(bytes != ceil(b.length, 8)){
        if(num != NULL) delete num;
        num = new int8[ceil(b.length, 8)]();
    }
    length = b.length;
    signal = b.signal;
    for(int i = 0; i < length; i += 8) num[blk] = b.num[blk];
}


void BigNumber::unsignedAdd(const BigNumber &a, const BigNumber &b, BigNumber &res)const{
    int l1 = a.length, l2 = b.length;

    int L = max(l1, l2) + 1;
    res.num = new int8[L]();
    
    int8 carry = 0;

    for(int i = 0; i < L; i += 8){
        int val1 = 0, val2 = 0;
        if(i < l1) val1 = a.num[blk];
        if(i < l2) val2 = b.num[blk];

        int val = val1 + val2 + carry; carry = 0;

        if(val >= 256){
            carry = 1; val -= 256;
        }   

        if(val){
            res.length = i + 8;
            res.num[blk] = (int8)val;
        }
    }
    for(int i = res.length - 1; i >= 0; i--){
        if((res.num[blk] >> bit) & 1) break;
        res.length--;
    }
    if(!res.length){
        if(res.num != NULL) delete res.num;
        res.num = new int8[1]();
        res.length = 1;
        return ;
    }
}

//ENSURE: !num.below(b.num)
void BigNumber::unsignedSub(const BigNumber &a, const BigNumber &b, BigNumber &res)const{
    int l1 = a.length, l2 = b.length, L = a.length;
    res.num = new int8[ceil(L, 8)]();

    int carry = 0;

    for(int i = 0; i < L; i += 8){
        int val1 = 0, val2 = 0;
        if(i < a.length) val1 = a.num[blk];
        if(i < b.length) val2 = b.num[blk];

        int val = (val1 - val2 - carry); carry = 0;

        if(val < 0){
            carry = 1; val += 256;
        }

        if(val){
            res.length = i + 8;
            res.num[blk] = (int8)val;
        }
    }
    for(int i = res.length - 1; i >= 0; i--){
        if((res.num[blk] >> bit) & 1) break;
        res.length--;
    }
    if(!res.length){
        if(res.num != NULL) delete res.num;
        res.num = new int8[1]();
        res.length = 1;
    }
}

BigNumber BigNumber::operator + (const BigNumber &b)const{
    BigNumber res;
    if(signal == b.signal){
        unsignedAdd(*this, b, res);
        res.signal = signal;
    }
    else{
        int8 sigA = signal, sigB = b.signal;
        int8 flag = this->below(b);
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
void BigNumber::operator += (const BigNumber &b){
    *this = *this + b;
}

BigNumber BigNumber::operator - (const BigNumber &b)const{
    BigNumber bb(b);
    bb.signal = !bb.signal;
    return *this + bb;
}

void BigNumber::operator -= (const BigNumber &b){
    *this = *this - b;
}

BigNumber BigNumber::operator* (const BigNumber &b)const{
    BigNumber res;
    int l1 = length, l2 = b.length;
    int L = l1 + l2;
    res.num = new int8[ceil(L, 8)]();
    res.signal = !(signal ^ b.signal);

    //special judge, speed up Decimal-Binary-Convertion
    if(b == 10){
        return (*this << 3) + (*this << 1);
    }

    if(*this < b){
        for(int i = 0; i < length; i++){
            if((num[blk] >> bit) & 1) res += b << i;
        }
    }
    else{
        for(int i = 0; i < b.length; i++){
            if((b.num[blk] >> bit) & 1) res += *this << i;
        }
    }

    return res;
}

void BigNumber::operator *= (const BigNumber &b){
    *this = *this * b;
}

BigNumber BigNumber::operator / (const BigNumber &b)const{
    if(b.equal(BigNumber(0))){
        PRINT_FONT_RED;
        puts("Divide by 0!");
        PRINT_ATTR_REC;
        fflush(stdout);
        throw ;
    }
    if(*this == 0 || length < b.length) return BigNumber(0);

    BigNumber temp, res;
    res.signal = !(signal ^ b.signal);

    for(int i = length - 1; i >= length - b.length; i--){
        temp <<= 1; temp += (num[blk] >> bit) & 1;
    }
    
    int8 *sta = new int8[ceil((length - b.length + 1), 8)];
    //top represent the size of sta
    int top = 0;

    if(!temp.below(b)){ temp -= b; sta[top++] = 1;}

    for(int i = length - b.length - 1; i >= 0; i--){
        temp <<= 1; 
        temp += (num[blk] >> bit) & 1;

        if(!temp.below(b)){temp -= b; sta[top++] = 1;}
        else sta[top++] = 0;
    }

    res.num = new int8[ceil(top, 8)]();
    for(int i = 0; i < top; i++)
        res.num[blk] |= sta[top - 1 - i] << bit;

    res.length = top;
    
    for(int i = res.length - 1; i >= 0; i--){
        if((res.num[blk] >> bit) & 1) break;
        res.length--;
    }
    return res;
}

void BigNumber::operator /= (const BigNumber &b){*this = *this / b;}

BigNumber BigNumber::operator % (const BigNumber &b)const{
    BigNumber p = *this / b;
    return *this - p * b;
}

void BigNumber::operator %= (const BigNumber &b){
    *this = *this & b;
}

BigNumber BigNumber::operator & (const BigNumber &b)const{
    BigNumber res;
    int L = min(length, b.length);
    res.num = new int8[ceil(L, 8)]();

    for(int i = 0; i < L; i++){
        int8 val1 = 0, val2 = 0;
        if(i < length) val1 = (num[blk] >> bit) & 1;
        if(i < b.length) val2 = (b.num[blk] >> bit) & 1;
        
        int8 val = val1 & val2;
        if(val){
            res.length = i + 1;
            res.num[blk] |= 1 << bit;
        }
    }
    if(!res.length){
        if(res.num != NULL) delete res.num;
        res.num = new int8[1]();
        res.length = 1;
    }
    return res;
}

void BigNumber::operator &= (const BigNumber &b){
    *this = *this & b;
}

BigNumber BigNumber::operator | (const BigNumber &b)const{
    BigNumber res;
    int L = max(length, b.length);
    res.num = new int8[ceil(L, 8)]();

    for(int i = 0; i < L; i++){
        int8 val1 = 0, val2 = 0;
        if(i < length) val1 = (num[blk] >> bit) & 1;
        if(i < b.length) val2 = (b.num[blk] >> bit) & 1;
        

        int8 val = val1 | val2;
        if(val){
            res.length = i + 1;
            res.num[blk] |= 1 << bit;
        }
    }
    if(!res.length){
        if(res.num != NULL) delete res.num;
        res.num = new int8[1]();
        res.length = 1;
    }
    return res;
}

void BigNumber::operator |= (const BigNumber &b){
    *this = *this | b;
}

BigNumber BigNumber::operator ^ (const BigNumber &b)const{
    BigNumber res;
    int L = max(length, b.length);
    res.num = new int8[ceil(L, 8)]();

    for(int i = 0; i < L; i++){
        int8 val1 = 0, val2 = 0;
        if(i < length) val1 = (num[blk] >> bit) & 1;
        if(i < b.length) val2 = (b.num[blk] >> bit) & 1;
        
        int8 val = val1 ^ val2;
        if(val){
            res.length = i + 1;
            res.num[blk] |= 1 << bit;
        }
    }
    if(!res.length){
        if(res.num != NULL) delete res.num;
        res.num = new int8[1]();
        res.length = 1;
    }
    return res;

}

void BigNumber::operator ^= (const BigNumber &b){
    *this = *this ^ b;
}

BigNumber BigNumber::operator >> (int x)const{
    if(x < 0){
        PRINT_FONT_RED; 
        printf("SHR %d bit?\n", x);
        PRINT_ATTR_REC;
        fflush(stdout);
        throw;
    }
    BigNumber res;
    if(x >= length){
        res.length = 1;
        res.num = new int8[1]();
        res.signal = signal;
        return res;
    }

    res.length = length - x;
    res.num = new int8[ceil(res.length, 8)]();
    res.signal = signal;

    int p = x / 8; int8 r = x % 8;
    int top = ceil(res.length, 8);
    if(!r) for(int i = 0; i < top - p; i++) res.num[i] = num[i + p];
    else{
        /*
            每一块的低8-r位由后p块的高8-r位决定
            每一块的高r位由后p+1块的低r位决定
        */
       for(int i = 0; i < top; i++){
           res.num[i] |= num[i + p] >> r;
           res.num[i] |= (num[i + p + 1] & ((int8)255 >> (8 - r)) << (8 - r));
       }
    }
    return res;
}

void BigNumber::operator >>= (int x){
    *this = *this >> x;
}

BigNumber BigNumber::operator << (int x)const{
    if(x < 0 || x > 1000000){
        PRINT_FONT_RED; 
        printf("SHL %d bit?\n", x);
        PRINT_ATTR_REC;
        fflush(stdout);
        throw;
    }

    BigNumber res;
    res.length = length + x;
    res.num = new int8[ceil(res.length, 8)]();
    res.signal = signal;

    int p = x / 8; int8 r = x % 8;
    int top = ceil(res.length, 8);
    if(!r) for(int i = p; i < top; i++) res.num[i] = num[i - p];
    else{
        /*
            每一块的低r位由前p+1块的高r位决定
            每一块的高8-r位由前p块的低8-r位决定
        */
        res.num[p] |= (num[0] & ((int8)255 >> r)) << r;
        for(int i = p + 1; i < top; i++){
            res.num[i] |= num[i - p - 1] >> (8 - r);
            res.num[i] |= (int8)((num[i - p] & ((int8)255 >> r)) << r);
        }
        res.num[top - 1] &= (1 << (((res.length -1) & 0x7) + 1)) - 1;
    } 
    return res;
}

void BigNumber::operator <<= (int x){
    *this = *this << x;
}

bool BigNumber::below(cosnt BigNumber &b)const{
    if(length < b.length) return true;
    else if(length > b.length) return false;
    else for(int i = length - 1; i >= 0; i -= 8){
        if(num[i / 8] == b.num[i / 8]) continue;
        return num[blk] < b.num[blk];
    }
    return false;
}

bool BigNumber::above(cosnt BigNumber &b)const{
    return b.below(*this);
}

bool BigNumber::equal(cosnt BigNumber &b)const{
    return !(this->below(b)) && !(b.below(*this));
}

bool BigNumber::operator < (const BigNumber& b)const{
    if(signal && !b.signal) return false;
    else if(!signal && b.signal) return true;
    else if(signal && b.signal) return this->below(b);
    else return this->above(b);
}

bool BigNumber::operator > (const BigNumber& b)const{
    return b < *this;
}

bool BigNumber::operator == (const BigNumber& b)const{
    return !(b < *this) && !(*this < b);
}

bool BigNumber::operator != (const BigNumber& b)const{
    return !(*this == b);
}

bool BigNumber::operator <= (const BigNumber& b)const{
    return *this < b || *this == b;
}

bool BigNumber::operator >= (const BigNumber& b)const{
    return *this > b || *this == b;
}

