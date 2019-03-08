#ifndef _NUMBER_H_
#define _NUMBER_H_
// 定义操作数类
#include <iostream>

using namespace std;

class Number
{ // 分数 class
  private:
    inline bool IsINF() const { return Fenmu == 0; }
    Number &HuaJian(); // 分数化简
  public:
    NUM Fenzi; //分子
    NUM Fenmu; //分母

    static Number INF;

    Number(NUM n1 = 0, NUM n2 = 1)
    {
        Fenzi = n1;
        Fenmu = n2;
        HuaJian();
    }
    ~Number() {}

    Number &operator+=(const Number &num); // 分数加法
    Number &operator-=(const Number &num); // 分数减法
    Number &operator*=(const Number &num); // 分数乘法
    Number &operator/=(const Number &num); // 分数除法
};

inline Number &Number::operator+=(const Number &num)
{
    //  a     c     a * d + b * c  //
    // --- + --- = --------------- //
    //  b     d         b * d      //
    Fenzi = Fenzi * num.Fenmu + Fenmu * num.Fenzi;
    Fenmu = Fenmu * num.Fenmu;
    return HuaJian();
}

inline Number &Number::operator-=(const Number &num)
{
    //  a     c     a * d - b * c  //
    // --- - --- = --------------- //
    //  b     d         b * d      //
    Fenzi = Fenzi * num.Fenmu - Fenmu * num.Fenzi;
    Fenmu = Fenmu * num.Fenmu;
    return HuaJian();
}

inline Number &Number::operator*=(const Number &num)
{
    //  a     c     a * c  //
    // --- * --- = ------- //
    //  b     d     b * d  //
    Fenzi = Fenzi * num.Fenzi;
    Fenmu = Fenmu * num.Fenmu;
    return HuaJian();
}

inline Number &Number::operator/=(const Number &num)
{
    //  a     c     a * d  //
    // --- / --- = ------- //
    //  b     d     b * c  //
    Fenzi = Fenzi * num.Fenmu;
    Fenmu = Fenmu * num.Fenzi;
    return HuaJian();
}

//分数函数
ostream &operator<<(ostream &, const Number &); // 打印分数

Number operator+(const Number &, const Number &);
Number operator-(const Number &, const Number &);
Number operator*(const Number &, const Number &);
Number operator/(const Number &, const Number &);
Number operator+(const Number &);
Number operator-(const Number &);
bool operator==(const Number &, const Number &);
bool operator!=(const Number &, const Number &);
bool operator<(const Number &, const Number &);
bool operator>(const Number &, const Number &);
bool operator<=(const Number &, const Number &);
bool operator>=(const Number &, const Number &);

Number abs(const Number &);

#endif
