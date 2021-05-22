#include <string>
#include <vector>
#include <iostream>
#include "BigNumber.h"

using namespace std;

#define PRINT_TIME(x) printf("%-30s Time: %.10f\n", x, (end - start) * 1.0 / CLOCKS_PER_SEC)

int main(int argc, char *argv[]){
    clock_t start, end;

    string a, b, c;
    while(cin >> a >> b >> c){

        start = clock();
        BigNumber res, x(a.c_str()), y(c.c_str());
        end = clock();
#ifdef TIME
        PRINT_TIME("Build");
#endif
        int yy = atoi(c.c_str());

        start = clock();
        if(!strcmp(b.c_str(), "+")){
            res = x + y;
        }
        else if(!strcmp(b.c_str(), "-")){
            res = x - y;
        }
        else if(!strcmp(b.c_str(), "*")){
            res = x * y;
        }
        else if(!strcmp(b.c_str(), "/")){
            res = x / y;
        }
        else if(!strcmp(b.c_str(), "%")){
            res = x % y;
        }
        else if(!strcmp(b.c_str(), "&")){
            res = x & y;
        }
        else if(!strcmp(b.c_str(), "|")){
            res = x | y;
        }
        else if(!strcmp(b.c_str(), "^")){
            res = x ^ y;
        }
        else if(!strcmp(b.c_str(), ">>")){
            res = x >> yy;
        }
        else if(!strcmp(b.c_str(), "<<")){
            res = x << yy;
        }
        end = clock();
#ifndef TIME
        res.show();
#endif

#ifdef TIME
        PRINT_TIME("Calculte");

        start = clock();
        res.show();
        end = clock();
        PRINT_TIME("Show Decimal");
#endif

    }
    return 0;
}