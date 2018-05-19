/*
 * Http_Parser_Wrap.h
 *
 *  Created on: Aug 15,2016
 *      Author: zhangyalei
 */

#ifndef HTTP_PARSER_WRAP_H_
#define HTTP_PARSER_WRAP_H_

#include "http_parser.h"
#include <string>

class Http_Parser_Wrap {
public:
	Http_Parser_Wrap();
	virtual ~Http_Parser_Wrap() { }

	void parse_http_content(const char* buf, uint32_t len);

	bool is_read_all() { return read_all_; }
	bool is_read_referer() { return read_referer_; }
	bool has_read_referer() { return referer_.size() > 0; }
	bool is_read_forward_ip() { return read_forward_ip_; }
	bool has_read_forward_ip() { return forward_ip_.size() > 0; }
	bool is_read_user_agent() { return read_user_agent_; }
	bool has_read_user_agent() { return user_agent_.size() > 0; }
	bool is_read_content_type() { return read_content_type_;  }
	bool has_read_content_type() { return content_type_.size() > 0; }
	bool is_read_content_len()  { return read_content_len_; }
	bool has_read_content_len() { return content_len_ != 0; }
	bool is_read_host()  { return read_host_;  }
	bool has_read_host() { return host_.size()>0; }

	uint32_t get_total_length() { return total_length_; }
	char* get_url() { return (char*)url_.c_str(); }
	char* get_body_content() { return (char*)body_content_.c_str(); }
	uint32_t get_body_content_len() { return (uint32_t)body_content_.length(); }
	char* get_referer() { return (char*)referer_.c_str(); }
	char* get_forward_ip() { return (char*)forward_ip_.c_str(); }
	char* get_user_agent() { return (char*)user_agent_.c_str(); }
	char get_method() { return (char) http_parser_.method; }
	char* get_content_type() { return (char*) content_type_.c_str(); }
	uint32_t  get_content_len() { return content_len_; }
	char* get_host() { return (char*) host_.c_str(); }

	void set_url(const char* url, size_t length) { url_.append(url, length); }
	void set_referer(const char* referer, size_t length) { referer_.append(referer, length); }
	void set_forward_ip(const char* forward_ip, size_t length) { forward_ip_.append(forward_ip, length); }
	void set_user_agent(const char* user_agent, size_t length) { user_agent_.append(user_agent, length); }
	void set_body_content(const char* content, size_t length) { body_content_.append(content, length); }
	void set_content_type(const char* content_type, size_t length) {  content_type_.append(content_type, length); }
	void set_content_len(uint32_t content_len) { content_len_ = content_len; }
	void set_total_length(uint32_t total_len) { total_length_ = total_len; }
	void set_host(const char* host, size_t length) { host_.append(host, length); }
	void set_read_all() { read_all_ = true; }
	void set_read_referer(bool read_referer) { read_referer_ = read_referer; }
	void set_read_forward_ip(bool read_forward_ip) { read_forward_ip_ = read_forward_ip; }
	void set_read_user_agent(bool read_user_agent) { read_user_agent_ = read_user_agent; }
	void set_read_content_type(bool read_content_type) { read_content_type_ =  read_content_type; }
	void set_read_content_len(bool read_content_len) { read_content_len_ = read_content_len; }
	void set_read_host(bool read_host) { read_host_ = read_host; }

	static int on_url(http_parser* parser, const char *at, size_t length, void* obj);
	static int on_header_field(http_parser* parser, const char *at, size_t length, void* obj);
	static int on_header_value(http_parser* parser, const char *at, size_t length, void* obj);
	static int on_header_complete (http_parser* parser, void* obj);
	static int on_body (http_parser* parser, const char *at, size_t length, void* obj);
	static int on_message_complete (http_parser* parser, void* obj);

private:
	http_parser http_parser_;
	http_parser_settings http_parser_settings_;

	bool read_all_;
	bool read_referer_;
	bool read_forward_ip_;
	bool read_user_agent_;
	bool read_content_type_;
	bool read_content_len_;
	bool read_host_;
	uint32_t total_length_;
	std::string	url_;
	std::string	body_content_;
	std::string	referer_;
	std::string forward_ip_;
	std::string user_agent_;
	std::string content_type_;
	uint32_t  content_len_;
	std::string host_;
};

#endif
