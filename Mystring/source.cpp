#include"Mystring.h"
#include<iostream>
int main(void)
{
	Mystring mmstr("12345678912345678912345678912345678912345678912345678912345678900"); 
	Mystring mmmstr(mmstr);
	Mystring x(mmstr + mmmstr);
	std::cout << mmstr[64] << ' ' << mmmstr[0];
	return 0;
}