# 24 Game Solver

![calc24](./doc/calc24.ico)

[24 Game](https://en.wikipedia.org/wiki/24_Game) is an arithmetical card game
in which the objective is to find a way to manipulate four integers so that the end result is 24.
For example, for the card with the numbers 4, 7, 8, 8,
a possible solution is `4 * (7 - 8 / 8) = 24`.

This code was written in 2004 with Visual Studio 6.0.
It tries to find **all diverse** solutions for the 24 game (and similar) problem.

It only outputs **diverse** solutions.
If one solution can be transformed to another one using commutative law and association low
(i.e. change the order of calculation), only one of the two will be printed.
For example, `(4 - (8 - 7)) * 8 = 24` and `8 * (4 + 7 - 8) = 24` are all possible solutions,
but they have no significant difference, so they will be considered same.
The program applies some kind of simplifications to the equation,
the two will be simplified to the same version, and that one will be printed.
Use `-o` command line option to output every solutions, and their simplified form.

## Build

``` bash
# normal build
g++ -o calc24.out *.cpp

# or enables calculation progress display
g++ -o calc24.out *.cpp -DSHOW_PROGRESS_INFO
```

## Usage

Use `-h` to list possible command line options.

``` console
$ ./calc24.out -h
format: calc24 [-a] [-e] [-o] [-h]
-a By default this program solves the 24 problem.
   Use this option for arbitrary target and number of integers.
-e Show all brackets when print solution equation.
-o List all possible equations and their simplified forms.
-h Print this help.
```

For example, to solve the puzzle with numbers 4, 7, 8, 8:

``` console
$ ./calc24.out


(calc24)Input format: <num1> <num2> <num3> <num4>
set num1 < 0 to quit.
4 7 8 8
The numbers you input: 4, 7, 8, 8
8 * (4 + 7 - 8) = 24
7 * 8 - 4 * 8 = 24
4 * (7 - 8 / 8) = 24
Found 3 equation(s).


(calc24)Input format: <num1> <num2> <num3> <num4>
set num1 < 0 to quit.
-1
```

For more examples, check [examples.txt](./doc/examples.txt).

And [this file](./doc/24-game-solutions.txt) lists solutions for every 24 game puzzle.
