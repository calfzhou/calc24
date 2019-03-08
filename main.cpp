#define _MAIN_CPP_
#include <iostream>
#include <stdlib.h>

#include "head.h"
#include "exptree.h"
#include "stack.h"
#include "main.h"

//~#define SHOW_PROGRESS_INFO

int TotalNum, CurrNum, CurrPerTotal;
static int TotalNums[] = {
    1, 5, 75, 2250,            // 1~4
    112500, 8437500, 885937500 //, 124031250000  // 5~8
};

int main(int argc, char **argv)
{
    deal_arg(argc, argv);

    while (Calc())
        ;

    return 0;
}

////////////////////////////////////////////////////

// 处理命令行参数
void deal_arg(int argc, char **argv)
{
    static const char *readme = "format: calc24 [-a] [-e] [-o] [-h]\n"
                                "-a 计算任意个数，任意目标\n"
                                "-e 输出算式时不省略括号\n"
                                "-o 输出结果时显示化简前后对比\n"
                                "-h 打印帮助\n";

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-')
            continue;
        switch (argv[i][1])
        {
        case 'a':
            Calc_All = true;
            break;
        case 'e':
            Simp_Exp = false;
            break;
        case 'o':
            Simp_Out = false;
            break;
        case 'h':
            cout << readme << flush;
            exit(0);
        }
    }
}

bool Calc()
{
    Number *numbers, goal;
    int count = Calc_All ? input(numbers, goal) : input4_24(numbers, goal);
    if (count == 0)
        return false;

    ExpTree *subexps = new ExpTree[count];
    for (int i = 0; i < count; i++)
        subexps[i] = numbers[i];
    //SortA (subexps, count);
    print(numbers, count, (const char *)"The numbers you input: ");

    Equs_Stack.clear();
    TotalNum = TotalNums[count - 1];
    CurrNum = 0;
    CurrPerTotal = -1;
    if (!search(subexps, count, goal))
        cout << "Can't find any equation." << endl;
    else
        cout << "Found " << Equs_Stack.size() << " equation(s)." << endl;
    //Equs_Stack.print ();

    delete[] numbers;
    delete[] subexps;
    return true;
}

int input4_24(Number *&numbers, Number &goal)
{
    int i, total = 4;
    NUM n;

    cout << "\n\n(calc24)Input format: <num1> <num2> <num3> <num4>\n";
    cout << "set num1 < 0 to quit." << endl;

    cin >> n; // num1
    if (n < 0)
        return 0;

    numbers = new Number[total];
    numbers[0] = abs(n);
    for (i = 1; i < total; i++)
    {
        cin >> n;
        numbers[i] = abs(n); //保存到分数数组
    }
    goal = 24;
    return total;
}

int input(Number *&numbers, Number &goal)
{
    int i, total;
    NUM n;

    cout << "\n\nInput format: <count_of_numbers> <num1> ... <goal>\n";
    cout << "set count_of_numbers <= 0 to quit." << endl;

    cin >> total;
    if (total <= 0)
        return 0;

    numbers = new Number[total];
    for (i = 0; i < total; i++)
    {
        cin >> n;
        numbers[i] = abs(n); //保存到分数数组
    }
    cin >> n;
    goal = abs(n);
    return total;
}

inline void progress(int inc)
{
    CurrNum += inc;
    // cout << CurrNum << " / " << TotalNum <<
    //     " = " << CurrNum*100/TotalNum << "%\r" << flush;
    // return;
    if (CurrPerTotal != CurrNum * 100 / TotalNum)
    {
        CurrPerTotal = CurrNum * 100 / TotalNum;
        cout << CurrNum << " / " << TotalNum << " = " << CurrPerTotal << "%\n"
             << flush;
    }
}

// search函数规模f(n) (不考虑有相同数字时的简化)
// f(n) = n*(n-1)/2*5*f(n-1), f(1) = 1
// 即f(n) = pow(5/2, n-1)*n!*(n-1)!
// f(1)~f(8) = 1, 5, 75, 2250, 112500, 8437500, 885937500, 124031250000
bool search(ExpTree *subexps, int n, const Number &goal)
{
#ifdef SHOW_PROGRESS_INFO
    if (n == 2)
        progress(TotalNums[1]);
#endif
    if (n == 1)
    {
        if (subexps[0].value() != goal)
            return false;

        if (Simp_Out)
        {
            subexps[0].simplify();
            if (Equs_Stack.push(subexps[0], comptree))
                cout << subexps[0] << endl;
        }
        else
        {
            cout << subexps[0] << " ==> " << flush;
            subexps[0].simplify();
            cout << subexps[0] << flush;
            if (Equs_Stack.push(subexps[0], comptree))
                cout << endl;
            else
                cout << " [known solution]" << endl;
        }
        return true;
    }

    bool found = false;
    ExpTree *myexps = new ExpTree[n];
    int i;
    for (i = 0; i < n; i++)
    { // do a copy
        myexps[i] = subexps[i];
    }
    SortA(myexps, n);
    //print (myexps, n, "The exps I get: ");

    int i_last = 0;
    for (i = 0; i < n - 1; i++)
    {
        if (i != 0 &&
            myexps[i].value() == myexps[i_last].value())
        {
#ifdef SHOW_PROGRESS_INFO
            progress((n - i - 1) * 5 * TotalNums[n - 2]);
#endif
            continue;
        }
        i_last = i;
        int j_last = i + 1;
        for (int j = i + 1; j < n; j++)
        {
            if (j != i + 1 &&
                myexps[j].value() == myexps[j_last].value())
            {
#ifdef SHOW_PROGRESS_INFO
                progress(5 * TotalNums[n - 2]);
#endif
                continue;
            }
            j_last = j;
            ExpTree a = myexps[i];
            ExpTree b = myexps[j];
            myexps[j] = myexps[n - 1];

            myexps[i] = a + b;
            if (search(myexps, n - 1, goal))
                found = true;

            myexps[i] = b - a;
            if (search(myexps, n - 1, goal))
                found = true;

            myexps[i] = a * b;
            if (search(myexps, n - 1, goal))
                found = true;

            myexps[i] = a / b;
            if (search(myexps, n - 1, goal))
                found = true;

            myexps[i] = b / a;
            if (search(myexps, n - 1, goal))
                found = true;

            myexps[i] = a;
            myexps[j] = b;
        }
    }

    delete[] myexps;
    return found;
}

////////////////////////////////////////////////////

NUM GCD(NUM n1, NUM n2) //最大公约数
{
    NUM n;

    while (n2)
    {
        n = n1 % n2;
        n1 = n2;
        n2 = n;
    }
    return n1;
}

template <class ELEM>
void print(ELEM *array, int n, const char *str)
{
    if (str != NULL)
        cout << str;
    for (int i = 0; i < n; i++)
    {
        cout << array[i];
        if (i < n - 1)
            cout << ", ";
    }
    cout << endl;
}

template <class ELEM>
void SortA(ELEM *array, int n)
{
    for (int i = 1; i < n; i++) // Insert i'th record
        for (int j = i; j > 0 && array[j] < array[j - 1]; j--)
            myswap(array[j], array[j - 1]);
}

template <class ELEM>
void myswap(ELEM &e1, ELEM &e2)
{
    ELEM e = e1;

    e1 = e2;
    e2 = e;
}
