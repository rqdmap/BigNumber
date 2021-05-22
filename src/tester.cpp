#include <cstdlib>
#include <cstdio>
int main(int argc, char *argv[]){
    if(argc == 1) return 0;
    system("make");

    int cnt = 0;    
    while(1){
        if(!system(argv[1])){
            printf("%d ok\n", cnt++);
        }
        else{
            printf("Oh no!\n");
            break;
        }
    }
    
    return 0;
}