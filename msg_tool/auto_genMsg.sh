#!/bin/sh

_TARGET_DIR=msg
_FILE_ARRAY_=("Game_DB_Message.define" "Game_Master_Message.define" "Gate_Message.define" "Inner_Message.define" "Log_Message.define")

find ../${_TARGET_DIR} -type f -not \( -name 'Message.h' -or -name 'Err_Code.h' -or -name 'Msg_Define.h' \) -delete

for _FILE_NAME_ in `ls *.define`
do
	echo "-----------------------------------------------"
	_DST_FILE_H_=${_FILE_NAME_%.*}".h"
	_DST_FILE_C_=${_FILE_NAME_%.*}".cpp"

	_MACRO_H_=`echo ${_FILE_NAME_%.*}"_H_"|tr "[:lower:]" "[:upper:]"`
	_MACRO_N_H_="#ifndef "${_MACRO_H_}
	_MACRO_Y_H_="#define "${_MACRO_H_}
	
	#注释
	echo -e "/**" > ${_DST_FILE_H_} 
	echo -e " * struct ${_FILE_NAME_%.*} description\n *" >> ${_DST_FILE_H_} 
	echo -e " * [This file was auto-generated. Please Do not edit]\n *" >> ${_DST_FILE_H_} 
	echo -e " */" >> ${_DST_FILE_H_}
	
	#宏定义
	echo -e "\n${_MACRO_N_H_}" >> ${_DST_FILE_H_} 
	echo -e "${_MACRO_Y_H_}\n" >> ${_DST_FILE_H_}
	echo -e "#include \"Block_Buffer.h\"" >> ${_DST_FILE_H_}
	
	if [ "${_FILE_NAME_}" != "Msg_Struct.define" ];then
		echo -e "#include \"Msg_Struct.h\"\n\n" >> ${_DST_FILE_H_}
	fi
	
	for var in ${_FILE_ARRAY_[@]};
	do
		if [ "${_FILE_NAME_}" =  "${var}" ];then
			echo -e "#include \"Public_Struct.h\"\n\n" >> ${_DST_FILE_H_}
			break
		fi
	done
	
	echo -e "#include \"${_DST_FILE_H_}\"\n" > $_DST_FILE_C_
	
	./gen_msg.sh $_FILE_NAME_ $_DST_FILE_H_ $_DST_FILE_C_
	
	echo -e "\n\n#endif" >> ${_DST_FILE_H_}
	echo "-----------------------------------------------"
done

mv *.h ../${_TARGET_DIR}
mv *.cpp ../${_TARGET_DIR}
 
