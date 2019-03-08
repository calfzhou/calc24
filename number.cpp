#include "head.h"
#include "number.h"

Number Number::INF = Number(0, 0);

Number &Number::HuaJian()
{
    if (Fenmu == 0)
    {
        Fenzi = 0; // inf =def= 0/0
        return *this;
    }

    NUM n = GCD(Fenzi, Fenmu);
    Fenzi /= n;
    Fenmu /= n;
    if (Fenmu < 0)
    {
        Fenmu = -Fenmu;
        Fenzi = -Fenzi;
    }
    return *this;
}

///////////////////////////////////////////////////////

ostream &operator<<(ostream &s, const Number &num)
{
    cout << " ";
    if (num.Fenmu == 0)
        return s << "INF";

    s << num.Fenzi;
    if (num.Fenmu != 1)
        s << "/" << num.Fenmu;
    return s;
}

Number operator+(const Number &num1, const Number &num2)
{
    Number num = num1;
    return num += num2;
}

Number operator-(const Number &num1, const Number &num2)
{
    Number num = num1;
    return num -= num2;
}

Number operator*(const Number &num1, const Number &num2)
{
    Number num = num1;
    return num *= num2;
}

Number operator/(const Number &num1, const Number &num2)
{
    Number num = num1;
    return num /= num2;
}

Number operator+(const Number &num)
{
    return num;
}

Number operator-(const Number &num)
{
    return Number(-num.Fenzi, num.Fenmu);
}

bool operator==(const Number &num1, const Number &num2)
{
    return num1.Fenzi == num2.Fenzi && num1.Fenmu == num2.Fenmu;
}

bool operator!=(const Number &num1, const Number &num2)
{
    return num1.Fenzi != num2.Fenzi || num1.Fenmu != num2.Fenmu;
}

bool operator<(const Number &num1, const Number &num2)
{
    return num1.Fenzi * num2.Fenmu < num1.Fenmu * num2.Fenzi;
}

bool operator>(const Number &num1, const Number &num2)
{
    return num1.Fenzi * num2.Fenmu > num1.Fenmu * num2.Fenzi;
}

bool operator<=(const Number &num1, const Number &num2)
{
    return num1.Fenzi * num2.Fenmu <= num1.Fenmu * num2.Fenzi;
}

bool operator>=(const Number &num1, const Number &num2)
{
    return num1.Fenzi * num2.Fenmu >= num1.Fenmu * num2.Fenzi;
}

Number abs(const Number &num)
{
    if (num.Fenzi >= 0)
        return Number(num.Fenzi, num.Fenmu);
    return Number(-num.Fenzi, num.Fenmu);
}
