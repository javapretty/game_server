#! /bin/bash

DEFINE_PATH='../config/structs/'
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
	./serialize_tool cpp $DEFINE_PATH 'game_struct.xml' 'Game_Struct'
	./serialize_tool cpp $DEFINE_PATH 'outer_msg.xml' 'Client_Message'
	./serialize_tool cpp $DEFINE_PATH 'inner_msg.xml' 'Server_Message'
}

function gen_js(){
	./serialize_tool js $DEFINE_PATH 'game_struct.xml' 'struct'
	./serialize_tool js $DEFINE_PATH 'inner_msg.xml' 'msg_struct'
	./serialize_tool js $DEFINE_PATH 'outer_msg.xml' 'msg_struct'
}

function gen_msgd(){
	./serialize_tool msg $DEFINE_PATH 'Message.define' 'msg'
}

function cp_file(){
	wildcard='.*'
	cp -rf CPP/* $CPP_TARGET
	cp -rf CPP/'Client_Message'${wildcard} $ROBOT_PATH
	cp -rf CPP/'Game_Struct'${wildcard} $ROBOT_PATH
	cp -rf CPP/Message.h $ROBOT_PATH
	cp -rf JS/* $JS_TARGET
}

function do_some_others(){
	rm -rf CPP
	rm -rf JS
}

gen_msg

