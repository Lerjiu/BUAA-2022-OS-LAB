extern int c(int num);
int b(void)
{
    int num=2;
    return num;
}
int main()
{
	int a;
	a=1;
	b();
	c(a);
	return 0;
}

