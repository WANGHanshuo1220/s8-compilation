void main()
{
    int a = 0;
    int i, j;
    for(i = 0; i < 5; i = i + 1)
    {
        for(j = 0; j < 5; j = j + 1)
        {
            a = a + i + j;
        }
    }
    print("a: ", a);
}