/*
 * Http_Parser_Wrap.cpp
 *
 *  Created on: Aug 15,2016
 *      Author: zhangyalei
 */

#include <string.h>
#include "Http_Parser_Wrap.h"

#define MAX_REFERER_LEN	32

Http_Parser_Wrap::Http_Parser_Wrap() : read_all_(false),
	read_referer_(false), read_forward_ip_(false), read_user_agent_(false),
	read_content_type_(false), read_content_len_(false), read_host_(false),
	total_length_(0), content_len_(0) {}


void Http_Parser_Wrap::parse_http_content(const char* buf, uint32_t len) {
	http_parser_init(&http_parser_, HTTP_REQUEST);
	memset(&http_parser_settings_, 0, sizeof(http_parser_settings_));
	http_parser_settings_.on_url = on_url;
	http_parser_settings_.on_header_field = on_header_field;
	http_parser_settings_.on_header_value = on_header_value;
	http_parser_settings_.on_headers_complete = on_header_complete;
	http_parser_settings_.on_body = on_body;
	http_parser_settings_.on_message_complete = on_message_complete;
	http_parser_settings_.object = this;

	read_all_ = false;
	read_referer_ = false;
	read_forward_ip_ = false;
	read_user_agent_ = false;
	read_content_type_ = false;
	read_content_len_ = false;
	read_host_ = false;
	total_length_ = 0;
	url_.clear();
	body_content_.clear();
	referer_.clear();
	forward_ip_.clear();
	user_agent_.clear();
	content_type_.clear();
	content_len_ = 0;
	host_.clear();

	http_parser_execute(&http_parser_, &http_parser_settings_, buf, len);
}

int Http_Parser_Wrap::on_url(http_parser* parser, const char *at, size_t length, void* obj) {
	((Http_Parser_Wrap*)obj)->set_url(at, length);
	return 0;
}

int Http_Parser_Wrap::on_header_field(http_parser* parser, const char *at, size_t length, void* obj) {
	if (!((Http_Parser_Wrap*)obj)->has_read_referer()) {
		if (strncasecmp(at, "Referer", 7) == 0) {
			((Http_Parser_Wrap*)obj)->set_read_referer(true);
		}
	}

	if (!((Http_Parser_Wrap*)obj)->has_read_forward_ip()) {
		if (strncasecmp(at, "X-Forwarded-For", 15) == 0) {
			((Http_Parser_Wrap*)obj)->set_read_forward_ip(true);
		}
	}

	if (!((Http_Parser_Wrap*)obj)->has_read_user_agent()) {
		if (strncasecmp(at, "User-Agent", 10) == 0) {
			((Http_Parser_Wrap*)obj)->set_read_user_agent(true);
		}
	}

	if (!((Http_Parser_Wrap*)obj)->has_read_content_type()) {
		if (strncasecmp(at, "Content-Type", 12) == 0) {
			((Http_Parser_Wrap*)obj)->set_read_content_type(true);
		}
	}

	if(!((Http_Parser_Wrap*)obj)->has_read_content_len()) {
		if(strncasecmp(at, "Content-Length", 14) == 0) {
			((Http_Parser_Wrap*)obj)->set_read_content_len(true);
		}
	}

	if(!((Http_Parser_Wrap*)obj)->has_read_host()) {
		if(strncasecmp(at, "Host", 4) == 0) {
			((Http_Parser_Wrap*)obj)->set_read_host(true);
		}
	}
	return 0;
}

int Http_Parser_Wrap::on_header_value(http_parser* parser, const char *at, size_t length, void* obj) {
	if (((Http_Parser_Wrap*)obj)->is_read_referer()) {
		size_t referer_len = (length > MAX_REFERER_LEN) ? MAX_REFERER_LEN : length;
		((Http_Parser_Wrap*)obj)->set_referer(at, referer_len);
		((Http_Parser_Wrap*)obj)->set_read_referer(false);
	}

	if (((Http_Parser_Wrap*)obj)->is_read_forward_ip()) {
		((Http_Parser_Wrap*)obj)->set_forward_ip(at, length);
		((Http_Parser_Wrap*)obj)->set_read_forward_ip(false);
	}

	if (((Http_Parser_Wrap*)obj)->is_read_user_agent()) {
		((Http_Parser_Wrap*)obj)->set_user_agent(at, length);
		((Http_Parser_Wrap*)obj)->set_read_user_agent(false);
	}

	if (((Http_Parser_Wrap*)obj)->is_read_content_type()) {
		((Http_Parser_Wrap*)obj)->set_content_type(at, length);
		((Http_Parser_Wrap*)obj)->set_read_content_type(false);
	}

	if(((Http_Parser_Wrap*)obj)->is_read_content_len()) {
		std::string str_content_len(at, length);
		((Http_Parser_Wrap*)obj)->set_content_len(str_content_len.length());
		((Http_Parser_Wrap*)obj)->set_read_content_len(false);
	}

	if(((Http_Parser_Wrap*)obj)->is_read_host()) {
		((Http_Parser_Wrap*)obj)->set_host(at, length);
		((Http_Parser_Wrap*)obj)->set_read_host(false);
	}
	return 0;
}

int Http_Parser_Wrap::on_header_complete(http_parser* parser, void* obj) {
	((Http_Parser_Wrap*)obj)->set_total_length(parser->nread + (uint32_t) parser->content_length);
	return 0;
}

int Http_Parser_Wrap::on_body(http_parser* parser, const char *at, size_t length, void* obj) {
	((Http_Parser_Wrap*)obj)->set_body_content(at, length);
	return 0;
}

int Http_Parser_Wrap::on_message_complete(http_parser* parser, void* obj) {
	((Http_Parser_Wrap*)obj)->set_read_all();
	return 0;
}
