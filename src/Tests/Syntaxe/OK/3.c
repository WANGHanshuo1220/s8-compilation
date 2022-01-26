int a = 1+3;
int b = 4*6;
bool c = 1 < 2;
void main()
{
    int a = 1;
    if(c){
        int b = 2;
        a = 1 + (2 + (3 + (4 + 5)));
    }else
    {
        a = 1;
    }
    print("a: ", a);
}