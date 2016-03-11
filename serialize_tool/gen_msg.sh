#! /bin/bash

JS_DEFINES_1="Active_Message Client_Game_Message"
JS_DEFINES_2="Msg_Struct Game_Struct"

CPP_DEFINES="Active_Message Client_Game_Message Client_Login_Message Game_DB_Message Game_Master_Message
				Gate_Message Log_Message Msg_Struct Game_Struct"

SERVER_PATH='/root/server/server/'
DEFINE_PATH=$SERVER_PATH'serialize_tool/'
CPP_TARGET=$SERVER_PATH'msg'
JS_TARGET=$SERVER_PATH'js'
ROBOT_PATH='/root/server/robot/msg'

function gen_msg(){
	cd $DEFINE_PATH
	gen_cpp
	gen_js
	cp_file
	do_some_others
}

function gen_cpp(){
	for file in $CPP_DEFINES;do
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

function cp_file(){
	cp -rf CPP/* $CPP_TARGET
	cp -rf CPP/* $ROBOT_PATH
	cp -rf JS/* $JS_TARGET
}

function do_some_others(){
	mv -f $CPP_TARGET/Game_Struct.* $ROOT_PATH
	rm -rf CPP
	rm -rf JS
}

gen_msg

