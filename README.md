## Problem statement
    (1) Given two polynomials, check whether both are equal or not.
    (2) Polynomials can have any degree in x and y (two variables allowed: (x,y)).
    (3) Polynomials can have computations like: (3x-2+1) or (4x)*(6x)
    (4) Valid inputs: "(x-3)*(x-4)/(x^12-7xy-6x-10)" "x^76-100xy+10x^y-89xy^3" "(x+1)*(x-1)*(4x-1)/(x^3+1)"

## Included files
    (1) tl_update.cpp (main code)
    (2) gcc_version.png (compiler configuration)

## Instructions to run the program
    (1) Get inside the directory poly_process
    (2) Execute the command: g++ -o poly_output tl_updated.cpp
    (3) Run using command: ./poly_output <polynomial 1> <polynomial 2>
    (4) Test run: ./poly_output "(5x+4)*3x" "15x^2+12x"
