#! /bin/bash

JS_DEFINES_1="Active_Message Client_Game_Message Client_Master_Message Game_DB_Message Game_Master_Message Master_DB_Message"
JS_DEFINES_2="Msg_Struct Game_Struct"

CPP_DEFINES_CLIENT="Msg_Struct Active_Message Client_Game_Message Client_Login_Message Client_Master_Message"
CPP_DEFINES_SERVER="Game_Struct Game_DB_Message Gate_Message Log_Message Game_Master_Message"

DEFINE_PATH=`pwd`'/'
SERVER_PATH=$DEFINE_PATH'../misc/'
CPP_TARGET=$DEFINE_PATH'../msg'
JS_TARGET=$DEFINE_PATH'../js'
ROBOT_PATH=$DEFINE_PATH'../../robot/msg'

MESSAGE_SERVER='Server_Message'
MESSAGE_CLIENT='Client_Message'

function gen_msg(){
	gen_cpp
	gen_js
	gen_msgd
	cp_file
	do_some_others
}

function gen_cpp(){
	for file in $CPP_DEFINES_SERVER;do
		./serialize_tool cpp $DEFINE_PATH $file'.define' $MESSAGE_SERVER
	done
	for file in $CPP_DEFINES_CLIENT;do
		./serialize_tool cpp $DEFINE_PATH $file'.define' $MESSAGE_CLIENT
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
	cp -rf CPP/${MESSAGE_CLIENT}${wildcard} $ROBOT_PATH
	cp -rf CPP/Message.h $ROBOT_PATH
	cp -rf JS/* $JS_TARGET
}

function do_some_others(){
	rm -rf CPP
	rm -rf JS
}

gen_msg

