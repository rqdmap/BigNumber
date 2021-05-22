



# RQD BigNumber

使用C++实现的高精度整数运算，支持四则运算与位运算。

内部数据结构使用二进制存储高精度大数:

```cpp
typedef unsigned char int8;
struct BigNumber{
    int8 *num, signal;
    int length;
}
```



## 使用指北

添加`BigNumber.*, BigDecimal.*`到项目目录下，在需要使用高精度整数的文件中加载头文件`BigNumber.h`

### 初始化

```cpp
BigNumber(); //默认初始化一个为0的大整数
BigNumber(int);
BigNumber(long long);
BigNumber(const char*);
BigNumber(const BigNumber&);
```

### 显示

```cpp
void show()const;   //默认以10进制显示
void show2()const;  //以2进制显示
void show10()const; //以10进制显示
void show16()const; //以16进制显示

//非成员函数方法
void BigNumberShow(const BigNumber &);
void BigNumberShow2(const BigNumber &);
void BigNumberShow10(const BigNumber &);
void BigNumberShow16(const BigNumber &);
```

### 运算符

```cpp
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
```

### 比较符

```cpp
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
```

### 生成随机数

```cpp
friend BigNumber RandBigNumber(int __size);  //生成__size位的非负数
```



## 运算效率

设读入拥有$n$位十进制数码的十进制数

下述操作时间复杂度$O(n^2)$: 

- 读入该数时，将该数转化为二进制进行存储
- 打印该数时，将存储的二进制转换为十进制打印
- 乘法
- 除法、取模（常数小，较快） ~~事实上除法复杂度的紧上界甚至可能达不到$O(n^2)$, 但我不会算~~

其余操作均为线性操作



读入$n=10^4$的十进制数作为测试，上述操作均只需要2s左右。



## 展望

- 为了读入一个十进制数与显示一个十进制数，我们使用了**字节处理**的方法，而不是之前按位处理的方法，这样可以将常数降低到与二进制乘法类似的程度。但是，`Decimal-Binary`的双向转换仍然是巨大的时间消耗，此处可能可以有更好的实现办法。
- 目前乘法的实现是朴素的模拟，时间复杂度$O(n^2)$，可能可以使用FFT/NTT等多项式算法进行加速。

