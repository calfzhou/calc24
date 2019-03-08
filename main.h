#ifndef _MAIN_H_
#define _MAIN_H_

Stack<ExpTree> Equs_Stack;
bool Calc_All = false;
bool Simp_Exp = true;
bool Simp_Out = true;

// ...
void deal_arg(int, char **);
bool Calc();
int input(Number *&, Number &);
int input4_24(Number *&, Number &);
bool search(ExpTree *, int, const Number &);
void progress(int inc);

#endif
