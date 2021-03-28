#include <stdio.h>

int FuncWithSeparateDecl(int n);

int FuncWithSeparateDecl(int n){
    for (int i=0; i < n; i++){
        printf("%d\n", n);
    }
    return n;
}

double FunctionWithIfs(int x){
    int y = x*x + 1;

    if (1) {
        printf("In the 'IF' statement\n");
    }

    if (x==0) {
        return 0;
    }
    return y/x;
}

int ReallyBigOne(int a, int b, char c) {
    // long function with many statements
    char first = c;
    int n = a + b;

    n++;

    n = n * n + a - b;
    printf("This is only a test %d %d %d\n", a, b, n);
    
    char alpha = first;
    alpha = 'a';
    alpha = 'b';
    alpha = 'c';
    alpha = 'd';
    alpha = 'e';
    alpha = 'f';
    alpha = 'g';
    alpha = 'h';
    alpha = 'i';
    alpha = 'j';
    alpha = 'k';
    alpha = 'l';
    alpha = 'm';
    alpha = 'n';
    alpha = 'o';
    alpha = 'p';
    alpha = 'q';
    alpha = 'r';
    alpha = 's';
    alpha = 't';
    alpha = 'u';
    alpha = 'v';
    alpha = 'w';
    alpha = 'x';
    alpha = 'y';
    alpha = 'z';

    /* a
       multi-line
       comment
    */

    int myVar = n;

    myVar -= -1;

    // another comment
    double percentage = 1000.0;

    // return
    return 42;
}

int main(int argc, char *argv[]) {

}