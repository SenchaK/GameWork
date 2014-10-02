#pragma once

class Exception {
public :
	typedef enum{
		MESSAGE_LEN = 256 , 
	};
protected :
	char buf[MESSAGE_LEN];
public :
	const char* getMessage(){
		return buf;
	}
};
