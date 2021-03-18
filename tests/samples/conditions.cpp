int main(){

    int num = 1;
    int myBool = true;
    //not matched
    bool a = num == 5;
    //bool comparison
    bool b = myBool == true;
    bool c = (num==2) != true;
    //logic with literals
    if (myBool || true){
        return 1;
    }

    return 0;
}