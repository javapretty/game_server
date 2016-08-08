#! /bin/bash

DEFINE_PATH='../config/struct/'
JS_TARGET='../js'

function gen_msg(){
	gen_js
	gen_msgid
	cp_file
	rm_file
}

function gen_js(){
	./struct_tool js $DEFINE_PATH 'game_struct.xml' 'struct'
	./struct_tool js $DEFINE_PATH 'msg_struct.xml' 'struct'
}

function gen_msgid(){
	./struct_tool msg $DEFINE_PATH 'message.define' 'msg'
}

function cp_file(){
	wildcard='.*'
	cp -rf JS/* $JS_TARGET
	cp -rf '../config/struct/game_struct.xml' '../../robot/config'
	cp -rf '../config/struct/msg_struct.xml' '../../robot/config'
}

function rm_file(){
	rm -rf JS
}

gen_msg