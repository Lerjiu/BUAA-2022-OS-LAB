#include<stdio.h>
int main()
{
	int n;
	scanf("%d",&n);
	int num[6]={0};
	int i=0;
	do
	{
		num[i]=n%10;
		i++;
		n/=10;
	}while(n!=0);
	int j;
	for(j=0;j<i;j++)
	{
		if(num[j]!=num[i-j-1])
			break;
	}
	if(i==j){
		printf("Y");
	}else{
		printf("N");
	}
	return 0;
}
