/*
 * Http_Client.h
 *
 *  Created on: Aug 15,2016
 *      Author: zhangyalei
 */

#ifndef HTTP_CLIENT_H_
#define HTTP_CLIENT_H_

#include <string>
#include <curl/curl.h>
#include "Public_Define.h"

class Http_Client
{
public:
	Http_Client(void);
	~Http_Client(void);
    
	CURLcode post(const std::string & str_url, const std::string & str_post, std::string & str_response);
	CURLcode get(const std::string & str_url, std::string & str_response);
	std::string upload_byte_file(const std::string &str_url, void* data, int data_len);
	bool download_byte_file(const std::string &str_url, Data_Info* data_info);
};

#endif
