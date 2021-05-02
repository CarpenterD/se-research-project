int main(int argc, char const *argv[])
{
    int x = 0;
    if ( x == 6 ){

    }

    if ( 6 == x ){

    }

    char c = 'A';
    if ( c == 'D' ){

    }

    double d = 1.0;
    if ( d == 10.0){

    }

    float f = d;
    if ( f == 10.0){

    }

    static char *msg = "HELLO";
    if ( msg == "HELLO" ) {
        // may want to detect this (in addition to compiler warning)
    }

    return 0;
}
