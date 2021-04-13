int ExtraSpace() {
    int a, b, c;
     a = 1;
    b = 2;
    c = a + b;
    return c * a - b;
}

int DoubleTabbed() {
        int a, b, c;
        a = 1;
        b = 2;
        c = a + b;
        return c * a - b;
}

int HardTab() {
    int x = 20;
	int y = 4*x;
    return x + y;
}

int MissingSpace() {
    int a, b, c;
   a = 1;
    b = 2;
    c = a + b;
    return c * a - b;
}

int HalfIndent(int x) {
  int y = x*x + 2 * x + 1;
  return y;
}

int BadNesting() {
    int a = 10;
    for (int i = 0; i < 10; i++) {
    a += 11;
    }
    return a;
}

int BraceInMiddleOfNowhere()

{
    return 50;
}

int TwoStmtsOnOneLine()
{
    int a = 7; return 50;
}

void StmtInLineWithBrackets()
{ int a = 100;
return;}

void InlineAcceptable() { int a = 100; return; }

void InlineUnAcceptable() { int a = 'a'; int b = 'b'; int c = 'c'; return; }

int main(int argc, char *argv[]) {
    return 1;
}
