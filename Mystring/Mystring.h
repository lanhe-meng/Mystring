#ifndef MYSTRING_H
#define MYSTRING_H
#include<iostream>

class Mystring;


class Memorypool {
	friend class Mystring;
	friend Mystring operator+(const Mystring&,const Mystring&);
private:
	enum { piece_size = 64,init_size = 100 };
	struct element {
		int used_byte;
		element* next;
		element* pre;
		char ele_str[piece_size];
		element() :used_byte(0), next(nullptr), pre(nullptr), ele_str() {}
		element(const element& ele) :used_byte(ele.used_byte), next(nullptr), pre(nullptr) {
			for (int i = 0; i < used_byte; ++i) {
				*(this->ele_str + i) = *(ele.ele_str + i);
			}
		}
		element& operator=(const element& ele){
			used_byte = ele.used_byte;
			for (int i = 0; i < used_byte; ++i) {
				*(this->ele_str + i) = *(ele.ele_str + i);
			}
			return *this;
		}
	};
	element* free2used(const char*);
	element* free2used(const Memorypool::element&);
	void used2free(element*);

	element* first;
	element* free_head;
	
public:
	Memorypool();
	~Memorypool();
};

class Mystring {
	friend class Memorypool;
	friend Mystring operator+(const Mystring&, const Mystring&);
	friend bool operator==(const Mystring&, const  Mystring&);
public:
	Mystring();
	Mystring(const char*);
	Mystring(const Mystring&);
	Mystring(Mystring&&);
	~Mystring();
	char& operator[](size_t) const;
private:
	Memorypool::element* mystr_head,* mystr_tail;
	size_t sz;
	Mystring& apply_free(const char*, size_t);
	Mystring& apply_free(const Memorypool::element&);
	void release();
};
Mystring operator+(const Mystring&,const  Mystring&);
bool operator==(const Mystring&, const  Mystring&);
#endif