int GlobalGood = 100;
    int GlobalBad = -100;

struct MyStruct
{
    int num;
    char letter;
    float decimal; 
} instance;

int Switch(int n) {
    int a;
    switch (n)
    {
        case 0:
            a = n + 1;
            break;

        case 1:
            a = n - 1;
            a = n - 1;
            a = n - 1;
            a = n - 1;
            break;

        default:
            a = -1;
            break;
    }
    return a;
}

int FloatingBlock(){
    int a = 0;
    a += 1;

    {
        int b = a * 15;
        a = b - 1;
    }

    return a;
}

void DoWhile(int n){
    int count = 0;
    do
    {
        count++;
    } while (count < n);
    return;
}

int OneLiner(int a) {
    if ( a < 0 )
        return 0-a;
    return a;
}

int main(int argc, char *argv[]) {
    return 1;
}
