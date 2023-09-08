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
Mystring::Mystring():Mystring(""){}
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
Mystring::Mystring(Mystring&& m_str) noexcept
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
Mystring& Mystring::operator=(const Mystring& mstr)
{
	this->release();
	Memorypool::element* now = mstr.mystr_head;
	while (now) {
		this->apply_free(*now);
		now = now->next;
	}
	return *this;
}
Mystring& Mystring::operator=(Mystring&& mstr) noexcept
{
	this->release();
	Memorypool::element* now = mstr.mystr_head;
	while (now) {
		this->apply_free(*now);
		now = now->next;
	}
	return *this;
}

Mystring& Mystring::operator+=(const Mystring& mstr)
{
	return *this = *this + mstr;
}


bool operator==(const Mystring& m_str1, const  Mystring& m_str2)
{
	if (m_str1.sz != m_str2.sz)return false;
	size_t i = 0;
	for (; i < m_str1.sz; ++i) {
		if (m_str1[i] != m_str2[i])return false;
	}
	return true;
}
bool operator!=(const Mystring& m_str1, const Mystring& m_str2)
{
	return !(m_str1 == m_str2);
}
bool operator<(const Mystring& m_str1, const Mystring& m_str2)
{
	size_t min = m_str1.sz < m_str2.sz ? m_str1.sz : m_str2.sz;
	size_t i = 0;
	for (; i < min; ++i) {
		if (m_str1[i] == m_str2[i])continue;
		else {
			if (m_str1[i] > m_str2[i])return false;
			return true;
		}
	}
	if (m_str1.sz < m_str2.sz)return true;
	return false;
}
bool operator>(const Mystring& m_str1, const Mystring& m_str2)
{
	if (m_str1 == m_str2 || m_str1 < m_str2)return false;
	return true;
}

std::ostream& operator<<(std::ostream& out, const Mystring& mstr)
{
	for (size_t i = 0; i < mstr.sz; ++i)out << mstr[i];
	return out;
}

std::istream& operator>>(std::istream& in, Mystring& mstr)
{
	mstr.release();
	char s[Memorypool::piece_size + 1]{};
	char ch = 0;
	size_t i = 0;
	while (in.get(ch) && ch != '\n') {
		if (i > Memorypool::piece_size - 1) {
			mstr.apply_free(s, Memorypool::piece_size);
			i = 0;
		}
		s[i] = ch;
		++i;
	}
	if (!i)return in;
	mstr.apply_free(s, i);
	return in;
}

char* Mystring::c_str(char* s, size_t len)
{
	if (len <= sz)return nullptr;
	for (size_t i = 0; i < sz; ++i)s[i] = (*this)[i];
	s[sz] = 0;
	return s;
}

Mystring Mystring::substr(size_t beg, size_t len)
{
	Mystring tmp;
	size_t end = beg + len;
	char ch[2]{};
	for (size_t i = beg; i < end; ++i) {
		*ch = (*this)[i];
		tmp += ch;
	}
	return tmp;
}

Mystring& Mystring::append(const char* s)
{
	return *this += s;
}


Mystring& Mystring::insert(size_t beg, const Mystring& mstr)
{
	Mystring tmp;
	char ch[2]{};
	for (size_t i = beg; i < sz; ++i) {
		*ch = (*this)[i];
		tmp += ch;
	}
	this->erase(beg, sz - beg);
	*this += mstr;
	*this += tmp;
	return *this;
}

Mystring& Mystring::erase(size_t beg, size_t len)
{
	if (!len)return *this;
	Mystring tmp;
	char ch[2]{};
	for (size_t i = beg + len; i < sz; ++i) {
		*ch = (*this)[i];
		tmp += ch;
	}
	for (size_t i = 0,j = beg; i < tmp.sz; ++i,++j) {
		(*this)[j] = tmp[i];
	}
	if (this->mystr_tail->used_byte > len)this->mystr_tail->used_byte -= len;
	else if (this->mystr_tail->used_byte == len) {
		Memorypool::element* tmp = mystr_tail;
		mystr_tail = mystr_tail->pre;
		_source_pool.used2free(tmp);
	}
	else {
		size_t tlen = len;
		while (this->mystr_tail->used_byte < tlen) {
			tlen -= this->mystr_tail->used_byte;
			Memorypool::element* tmp = mystr_tail;
			mystr_tail = mystr_tail->pre;
			_source_pool.used2free(tmp);
		}
		this->mystr_tail->used_byte -= tlen;;
	}
	sz -= len;
	return *this;
}

Mystring& Mystring::replace(size_t beg, size_t len, const Mystring& mstr)
{
	this->erase(beg, len);
	this->insert(beg, mstr);
	return *this;
}

int compare(const Mystring& mstr1, const Mystring& mstr2)
{
	if (mstr1 < mstr2)return -1;
	if (mstr1 == mstr2)return 0;
	return 1;
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
	if (!ele_str)return;
	ele_str->used_byte = 0;
	if (ele_str->next) ele_str->next->pre = ele_str->pre;
	if (ele_str->pre)ele_str->pre->next = ele_str->next;
	ele_str->pre = nullptr;
	ele_str->next = free_head;
	free_head->pre = ele_str;
	free_head = ele_str;
}

Mystring operator+(const Mystring& m_str1, const Mystring& m_str2)
{
	Mystring m_str(m_str1);
	if (!m_str.mystr_head)m_str.apply_free("", 0);
	for (size_t i = m_str.mystr_tail->used_byte, j = 0, max = m_str2.sz; j < max; ++i, ++j) {
		if (i > 63) {
			i %= 64;
			m_str.apply_free("", 0);
		}
		*(m_str.mystr_tail->ele_str + i) = m_str2[j];
		++m_str.mystr_tail->used_byte;
		++m_str.sz;
	}
	return m_str;
}