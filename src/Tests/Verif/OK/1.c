int start = 0, end = 5;

void main() 
{
	int i, s = start, e = end;
	int sum = 10;
	for(i = s; i < e; i = i + 1){
		if(sum > 0)
		{
			sum = sum - i;
			print("sum: ", sum, "\n");
		}else
		{
			sum = sum + 1;
			print("sum: ", sum, "\n");
		}
	}
	print("sum: ", sum, "\n");
}
