int Good() {
    int a, b, c;
    a = 1;
    b = 2;
    c = a + b;
    return c * a - b;
}

int GoodNested() {
    int a = 10;
    for (int i = 0; i < 10; i++) {
        a += 11;
    }
    return a;
}

int GoodLowerBrackets()
{
    int a, b, c;
    a = 1;
    b = 2;
    c = a + b;
    return c * a - b;
}

char NoIndetation(char c) {
int offset = 'a' - 'A';
return c + offset;
}

char NoIndetationNested(int n) {
if (n > 50) {
return 1;
} else {
return 0;
}
}

int NoFormattingBasic(int n) { int m = 0-n; int m_sq = m * m; return m; }

int NoFormattingNested() { int count = 0; while (count < 100) { count++; } return -1 * count; }

int main(int argc, char *argv[]) {
    return 1;
}
