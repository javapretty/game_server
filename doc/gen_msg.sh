#! /bin/bash

DEFINE_PATH='../config/struct/'
SERVER_PATH='../misc/'
JS_TARGET='../js'
ROBOT_PATH='../../robot'

function gen_msg(){
	gen_cpp
	gen_js
	gen_msgd
	cp_file
	rm_file
}

function gen_cpp(){
	./struct_tool cpp $DEFINE_PATH 'game_db_struct.xml' 'Game_Struct'
	./struct_tool cpp $DEFINE_PATH 'msg_struct.xml' 'Msg_Struct'
}

function gen_js(){
	./struct_tool js $DEFINE_PATH 'game_db_struct.xml' 'struct'
	./struct_tool js $DEFINE_PATH 'msg_struct.xml' 'struct'
}

function gen_msgd(){
	./struct_tool msg $DEFINE_PATH 'message.define' 'msg'
}

function cp_file(){
	wildcard='.*'
	cp -rf CPP/* $ROBOT_PATH
	cp -rf JS/* $JS_TARGET
}

function rm_file(){
	rm -rf CPP
	rm -rf JS
}

gen_msg