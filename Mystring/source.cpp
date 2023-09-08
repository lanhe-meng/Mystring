#include"Mystring.h"
#include<iostream>
using namespace std;
int main(void)
{
	Mystring mmstr("12345678912345678912345678912345678912345678912345678912345678900"); 
	Mystring mmmstr(mmstr);
	Mystring s1("abc");
	Mystring s2("abb");
	cout << compare(s1, s1) << endl;
	return 0;
}