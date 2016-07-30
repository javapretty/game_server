#! /bin/bash

DEFINE_PATH='../config/struct/'
SERVER_PATH='../misc/'
CPP_TARGET='../msg'
JS_TARGET='../js'
ROBOT_PATH='../../robot/msg'

function gen_msg(){
	gen_cpp
	gen_js
	gen_msgd
	cp_file
	do_some_others
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
	./struct_tool msg $DEFINE_PATH 'Message.define' 'msg'
}

function cp_file(){
	wildcard='.*'
	cp -rf CPP/* $CPP_TARGET
	cp -rf CPP/'Msg_Struct'${wildcard} $ROBOT_PATH
	cp -rf CPP/'Game_Struct'${wildcard} $ROBOT_PATH
	cp -rf CPP/Message.h $ROBOT_PATH
	cp -rf JS/* $JS_TARGET
}

function do_some_others(){
	rm -rf CPP
	rm -rf JS
}

gen_msg

