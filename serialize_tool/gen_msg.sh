#! /bin/bash

JS_DEFINES_1="Active_Message Client_Game_Message"
JS_DEFINES_2="Msg_Struct Game_Struct"

CPP_DEFINES_OUTER="Active_Message Client_Game_Message Client_Login_Message Client_Master_Message Msg_Struct"
CPP_DEFINES="Game_DB_Message Gate_Message Log_Message Game_Struct"

DEFINE_PATH=`pwd`'/'
SERVER_PATH=$DEFINE_PATH'../'
CPP_TARGET=$DEFINE_PATH'../msg'
JS_TARGET=$DEFINE_PATH'../js'
ROBOT_PATH=$DEFINE_PATH'../../robot/msg'

function gen_msg(){
	gen_cpp
	gen_js
	gen_msgd
	cp_file
	do_some_others
}

function gen_cpp(){
	for file in $CPP_DEFINES;do
		./serialize_tool cpp $DEFINE_PATH $file'.define' $file
	done
	for file in $CPP_DEFINES_OUTER;do
		./serialize_tool cpp $DEFINE_PATH $file'.define' $file
	done
}

function gen_js(){
	for file in $JS_DEFINES_1;do
		./serialize_tool js $DEFINE_PATH $file'.define' 'msg_struct'
	done
	for file in $JS_DEFINES_2;do
		./serialize_tool js $DEFINE_PATH $file'.define' 'struct'
	done
}

function gen_msgd(){
	./serialize_tool msg $DEFINE_PATH Message.define msg
}

function cp_file(){
	wildcard='.*'
	cp -rf CPP/* $CPP_TARGET
	for file in $CPP_DEFINES_OUTER;do
		cp -rf CPP/${file}${wildcard} $ROBOT_PATH
	done
	cp -rf CPP/Message.h $ROBOT_PATH
	cp -rf JS/* $JS_TARGET
}

function do_some_others(){
	mv -f $CPP_TARGET/Game_Struct.* $SERVER_PATH
	rm -rf CPP
	rm -rf JS
}

gen_msg

