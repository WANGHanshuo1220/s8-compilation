int c;
void main()
{
    bool a = 1 > 0;
    int i;
    if(a){
        for(i = 0; i < 5; i = i + 1)
        {
            c = c + i;
        }
    }else
    {
        c = i;
    }
    print("c: ", c);
}