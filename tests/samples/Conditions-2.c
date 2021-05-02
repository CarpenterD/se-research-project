#include <stdbool.h>

int useInts(n) {
    if (n > 10) {
        return 1;
    } else {
        return 0;
    }
}

bool useBool(n) {
    if (n > 10) {
        return true;
    } else {
        return false;
    }
}

bool inverted(n) {
    if (n > 10) {
        return false;
    } else {
        return true;
    }
}

int extraElseIf(n) {
    if (n > 10) {
        return 1;
    } else if (n < 10) {
        return 1;
    } else {
        return 0;
    }
}

int extraChildren(n) {
    if (n > 10) {
        int x = 1000;
        return 1;
    } else {
        return 0;
    }
}

double notInts(int score) {
    if (score < 0) {
        return -102.7;
    } else {
        return 99.9;
    }
}

double notBools(int score) {
    if (score < 0) {
        return 10;
    } else {
        return 0;
    }
}

int main(int argc, char const *argv[])
{
    if (argc > 10) {
        return 1;
    }

    if (argc < 5)
        return 1;
    else
        return 0;
}
