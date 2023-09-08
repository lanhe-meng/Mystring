#ifndef MYSTRING_H
#define MYSTRING_H
#include<iostream>

class Mystring;


class Memorypool {
	friend class Mystring;
	friend Mystring operator+(const Mystring&,const Mystring&);
	friend std::istream& operator>>(std::istream& in, Mystring& mstr);
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
	friend bool operator!=(const Mystring&, const Mystring&);
	friend bool operator<(const Mystring&, const Mystring&);
	friend bool operator>(const Mystring&, const Mystring&);
	friend std::ostream& operator<<(std::ostream&, const Mystring&);
	friend std::istream& operator>>(std::istream&, Mystring&);
	friend int compare(const Mystring&, const Mystring&);
public:
	Mystring();
	Mystring(const char*);
	Mystring(const Mystring&);
	Mystring(Mystring&&)noexcept;
	~Mystring();
	char& operator[](size_t) const;
	Mystring& operator=(const Mystring&);
	Mystring& operator=(Mystring&&)noexcept;
	Mystring& operator+=(const Mystring&);
	size_t length() { return this->sz; }
	size_t size() { return this->sz; }
	bool empty() { return sz > 0 ? true : false; }
	void clear() { this->release(); }
	char* c_str(char*, size_t len);
	Mystring substr(size_t beg, size_t len);
	Mystring& append(const char*);
	Mystring& insert(size_t beg,const Mystring&);
	Mystring& erase(size_t beg, size_t len);
	Mystring& replace(size_t beg, size_t len, const Mystring&);
private:
	Memorypool::element* mystr_head,* mystr_tail;
	size_t sz;
	Mystring& apply_free(const char*, size_t);
	Mystring& apply_free(const Memorypool::element&);
	void release();
};
Mystring operator+(const Mystring&,const  Mystring&);
bool operator==(const Mystring&, const  Mystring&);
bool operator!=(const Mystring&, const Mystring&);
bool operator<(const Mystring&, const Mystring&);
bool operator>(const Mystring&, const Mystring&);
std::ostream& operator<<(std::ostream&,const Mystring&);
std::istream& operator>>(std::istream&, Mystring&);
int compare(const Mystring&, const Mystring&);
#endif