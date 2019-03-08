#ifndef _HEAD_H_
#define _HEAD_H_

#define MAXNUM 4

typedef int NUM;

extern bool Simp_Exp;
extern bool Simp_Out;

NUM GCD(NUM, NUM); //最大公约数
template <class ELEM>
void print(ELEM *array, int n, const char *str = 0); // 打印数组
template <class ELEM>
void SortA(ELEM *array, int n); // 升序排序
template <class ELEM>
void swap(ELEM &e1, ELEM &e2); // 交换

#endif
