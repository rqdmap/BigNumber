#include <cstdio>
#include <cstdlib>
#include <ctime>

char op[] = {'+', '-', '*', '/', '%', '^', '&', '|'};

int main(int argc, char* argv[]){
    srand(time(NULL));
    int t = 1, testCase = 10000;
    while(t--){
        int n = testCase;
        for(int i = 0; i < n; i++) printf("%d", rand() % 10);
        puts("");
        if(argc == 2 && argv[1][0] != 0) printf("%s\n", argv[1]);
        else printf("%c\n", op[rand() % 8]);
        n = testCase;
        for(int i = 0; i < n; i++) printf("%d", rand() % 10);
        puts("");
    }

    return 0;
}