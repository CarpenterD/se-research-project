void OneLinerWithInlineOpenBracket() {
    return;
}

void OneLinerWithNextLineBrackets()
{
    return;
}

int InlineFunction() { return 0; }

int MultipleLinesNoBlank(int l) {
    int i, j, k;
    i = 1;
    j = 2;
    k = i*j+l;
    return k;
}

int MultipleLinesWithBlanks(int n) {
    int a = 10;
    int b = a*n;


    return ++b;
}

char CommentsAndBlanks(int n) {
    // useless function
    char c = 'c';
    
    // return d
    return c+1;
}

int main(int argc, char *argv[]) {

}