#include <iostream>
#include <cstring>
#include <iostream>
//#include <ostream>

class string {
public:
	char* data_ = nullptr;
	size_t sz_ = 0;
	
	string() {}
	string(const char* new_data) {
		sz_ = std::strlen(new_data);
		data_ = new char[sz_ + 1];
		std::strcpy(data_, new_data);
	}
	string(const string& str) {
		sz_ = str.sz_;
		data_ = new char[sz_ + 1];
		std::strcpy(data_, str.data_);	
	}
	string& operator=(const char* new_data) {
		delete[] data_;
		sz_ = std::strlen(new_data);
		data_ = new char[sz_ + 1];
		std::strcpy(data_, new_data);
		return *this;
	}
	string& operator=(const string& str) {
		delete[] data_;
		sz_ = str.sz_;
		data_ = new char[sz_ + 1];
		std::strcpy(data_, str.data_);
		return *this;
	}
	string operator+(const string& str) {
		string temp;						
		temp.data_ = new char[sz_ + str.sz_ + 1];
		std::strcpy(temp.data_, data_);					
		std::strcat(temp.data_, str.data_);
		delete[] data_; data_ = nullptr;
		return temp;
	}
	friend std::ostream& operator<<(std::ostream& os, const string& str) {
		return os << str.data_;
	}
	friend std::istream& operator>>(std::istream& is, string& str) {
		char buf[1024];
		is >> buf;
		str.sz_ = std::strlen(buf);
		delete[] str.data_;
		str.data_ = new char[str.sz_ + 1];
		std:strcpy(str.data_, buf);
		return is;
	}
	char operator[](const size_t index) {
		return data_[index];
	}
	bool operator<(const string& str) {
		return std::strcmp(data_, str.data_) < 0;
	}
	bool operator>(const string& str) {
		return std::strcmp(data_, str.data_) > 0;
	}
	bool operator==(const string& str) {
		return std::strcmp(data_, str.data_) == 0;
	}
	bool operator!=(const string& str) {
		return std::strcmp(data_, str.data_) != 0;
	}
	bool operator<=(const string& str) {
		return std::strcmp(data_, str.data_) <= 0;
	}
	bool operator>=(const string& str) {
		return std::strcmp(data_, str.data_) >= 0;
	}
	~string() {
		delete[] data_;
	}
	
	const char* c_str() const {
		return data_;
	}
	
};

int main() {
	string s("123"); 	std::cout << "s: " << s << std::endl;
	string s2(s);		std::cout << "s2: " << s2 << std::endl;
	s = "abc";			std::cout << "s = \"abc\": " << s << std::endl;
	s2 = s;				std::cout << "s2 = s: " << s2 << std::endl;
	s = s + s2;			std::cout << "s = s + s2: " << s << std::endl;
	std::cout << "s[2]: " << s[2] << std::endl;
	std::cout << "s: " << s << std::endl;
	std::cout << "s2: " << s2 << std::endl;
	std::cout << "s < s2: " << (s < s2) << '\n';
	std::cout << "b2 = s2 < s: " << (s2 < s) << '\n';
	std::cout << "b3 = s > s2: " << (s > s2) << '\n';
	std::cout << "b4 = s2 > s: " << (s2 > s) << '\n';
	std::cout << "b5 = (s == s2): " << (s == s2) << '\n';
	s = s2;
	std::cout << "b6 = (s == s2): " << (s == s2) << '\n';
	
	string s3;
	std::cin >> s3; std::cout << s3 << std::endl;
	
	
	
	

	
	std::cin.get();
	return 0;
}