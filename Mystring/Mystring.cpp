#include"Mystring.h"
#include<iostream>

Memorypool _source_pool;
Memorypool::Memorypool() :free_head(new element[init_size]), first(free_head)
{
	for (int i = 0; i < init_size - 1; ++i) {
		(free_head + i)->next = (free_head + i + 1);
	}
	for (int i = 1; i < init_size; ++i) {
		(free_head + i)->pre = (free_head + i - 1);
	}
}  //构造函数
Memorypool::~Memorypool()
{
	delete[]first;
}
Mystring::Mystring(const char* str) :mystr_head(nullptr), mystr_tail(nullptr), sz(0)
{
	int i;
	char ch[Memorypool::piece_size + 1] = {};
	for (i = 0; str[i] != 0; ++i) {
		if (!(i % 64) && i != 0)
			apply_free(ch, 64);
		ch[i % 64] = str[i];
	}
	if (i % 64) {
		ch[i % 64] = 0;
		apply_free(ch, i % 64);
	}
	else if (i != 0) {
		apply_free(ch, 64);
	}
}
Mystring::Mystring()
{
	Mystring("");
}
Mystring::~Mystring()
{
	this->release();
}
Mystring::Mystring(const Mystring& m_str):mystr_head(nullptr),mystr_tail(nullptr),sz(0)
{
	const Memorypool::element* pele = m_str.mystr_head;
	while (pele) {
		apply_free(*pele);
		pele = pele->next;
	}

}
Mystring::Mystring(Mystring&& m_str)
{
	const Memorypool::element* pele = m_str.mystr_head;
	while (pele) {
		apply_free(*pele);
		pele = pele->next;
	}
}
char& Mystring::operator[] (size_t num)  const
{
	Memorypool::element* now = mystr_head;
	if (num > sz || num < 0) {
		//不会写
	}
	while (num > 63) {
		now = now->next;
		num -= 64;
	}
	return *(now->ele_str + num);
}
bool operator==(const Mystring& m_str1, const  Mystring& m_str2)
{
	if (m_str1.sz != m_str2.sz)return false;

}

//申请空闲块（保证成功），拷贝字符串，维护Mystring结构和改变sz
//str最大不超过piece_size（调用者解决）
Mystring& Mystring::apply_free(const char* str, size_t num)
{
	if (_source_pool.free_head) {  //还有空闲块
		sz += num;
		if (mystr_head) {
			Memorypool::element* new_ele = _source_pool.free2used(str);
			new_ele->pre = mystr_tail;
			mystr_tail->next = new_ele;
			mystr_tail = new_ele;
		}
		else {
			mystr_head = mystr_tail = _source_pool.free2used(str);
		}
	}
	else {
		//暂时不考虑用完的事情
	}
	return *this;
}
Mystring& Mystring::apply_free(const Memorypool::element& ele)
{
	if (_source_pool.free_head) {
		sz += ele.used_byte;
		if (mystr_head) {
			Memorypool::element* new_ele = _source_pool.free2used(ele);
			new_ele->pre = mystr_tail;
			mystr_tail->next = new_ele;
			mystr_tail = new_ele;
		}
		else {
			mystr_head = mystr_tail = _source_pool.free2used(ele);
		}
	}
	else {
		//暂时不考虑用完的事情
	}
	return *this;
}

//只进行拷贝字符串和转移空闲块的工作，不判断是否存在空闲块
Memorypool::element* Memorypool::free2used(const char* str)
{
	int& ub = this->free_head->used_byte;
	for (ub = 0; str[ub] != 0; ++ub) {
		(this->free_head->ele_str)[ub] = str[ub];
	}
	element* ret = this->free_head;
	this->free_head->next->pre = nullptr;
	this->free_head = this->free_head->next;
	ret->next = nullptr;
	ret->pre = nullptr;
	return ret;
}
Memorypool::element* Memorypool::free2used(const Memorypool::element& ele)
{
	element* pele = free_head;
	*pele = ele;
	free_head->next->pre = nullptr;
	free_head = free_head->next;
	pele->next = nullptr;
	return pele;
}
//释放mystring的资源并将指针和大小回空
void Mystring::release()
{
	while (mystr_head != mystr_tail) {
		mystr_tail = mystr_tail->pre;
		_source_pool.used2free(mystr_tail->next);
	}
	_source_pool.used2free(mystr_head);
	mystr_head = mystr_tail = nullptr;
	sz = 0;
}
void Memorypool::used2free(element* ele_str)
{
	ele_str->used_byte = 0;
	ele_str->pre = nullptr;
	ele_str->next = free_head;
	free_head = ele_str;
	ele_str->next->pre = ele_str;
}

Mystring operator+(const Mystring& m_str1, const Mystring& m_str2)
{
	Mystring m_str(m_str1);
	Memorypool::element* tail = m_str.mystr_tail;
	for (size_t i = tail->used_byte, j = 0, max = m_str2.sz; j < max; ++i, ++j) {
		if (i > 63) {
			i %= 64;
			m_str.apply_free("", 0);
		}
		*(tail->ele_str + i) = m_str2[j];
	}
	return m_str;
}