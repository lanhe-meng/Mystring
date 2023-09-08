#include"Mystring.h"
#include<iostream>
using namespace std;
int main(void)
{
	Mystring mmstr("12345678912345678912345678912345678912345678912345678912345678900"); 
	Mystring mmmstr(mmstr);
	cout << mmmstr.replace(1,5, "123");
	return 0;
}