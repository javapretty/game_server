#!/bin/sh

export _SRC_FILE_=$1
export _DST_FILE_H_=$2
export _DST_FILE_C_=$3
export _STRUCT_NAME_="struct"
#_STRUCT_NUMBERS_==>("int8_t" "int16_t" "int32_t" "int64_t" "uint8_t" "uint16_t" "uint32_t" "uint64_t" "double" "bool" "std::string" "other type" "std::vector")
export _STRUCT_NUMBERS_=("" "" "" "" "" "" "" "" "" "" "" "" "")
export _STRUCT_NUMBER_FLAG_=(0 0 0 0 0 0 0 0 0 0 0 0 0)
#_VECTOR_NUMBERS_==>("int8_t" "int16_t" "int32_t" "int64_t" "uint8_t" "uint16_t" "uint32_t" "uint64_t" "double" "bool" "std::string" "other type"(ÌØÊâ´æ´¢)
export _VECTOR_NUMBERS_=("" "" "" "" "" "" "" "" "" "" "" "")
export _VECTOR_NUMBER_FLAG_=(0 0 0 0 0 0 0 0 0 0 0 0)
export _STRUCT_NUMBER_TYPE="struct"
#export _DESE_FUNC_CONTENT="\tif ("
export _DESE_FUNC_CONTENT=""
export _STRUCT_MAX_INDX=0
export _CUR_INDEX_=0
export _STRUCT_NOTE_=0
export _ARRAY_=""
export _ARRAY_LEN_=""

function help()
{
	echo "Useage:$0 msg_file.txt msg_file.h msg_file.cpp"
}

function do_vectorserialize()
{
	eval seriType=\$$#
	while [ $# -gt 1 ]
    do		
		echo -e "\tuint16_t __${1}_vec_size = ${1}.size();" >> $_DST_FILE_C_
		echo -e "\tw.write_uint16(__${1}_vec_size);" >> $_DST_FILE_C_
		echo -e "\tfor(uint16_t i = 0; i < __${1}_vec_size; ++i) {" >> $_DST_FILE_C_
		echo -e "\t\tw.${seriType}(${1}[i]);" >> $_DST_FILE_C_
		echo -e "\t}\n" >> $_DST_FILE_C_
        shift
    done
}

function do_vectordeserialize()
{
	eval seriType=\$$#
	readType=$1
	shift
	while [ $# -gt 1 ]
    do		
		echo -e "\tuint16_t __${1}_vec_size;" >> $_DST_FILE_C_
		echo -e "\tif(r.read_uint16(__${1}_vec_size) )" >> $_DST_FILE_C_
		echo -e "\t\treturn -1;" >> $_DST_FILE_C_
		echo -e "\t${1}.reserve(__${1}_vec_size);" >> $_DST_FILE_C_
		echo -e "\tfor(uint16_t i = 0; i < __${1}_vec_size; ++i) {" >> $_DST_FILE_C_
		echo -e "\t\t${seriType} v;" >> $_DST_FILE_C_
		echo -e "\t\tif(r.${readType}(v) )" >> $_DST_FILE_C_
		echo -e "\t\t\treturn -1;" >> $_DST_FILE_C_
		echo -e "\t\t${1}.push_back(v);" >> $_DST_FILE_C_
		echo -e "\t}\n" >> $_DST_FILE_C_
        shift
    done
}

function do_vectorreset()
{	
	while [ $# -gt 0 ]
	do
		echo -e "\t${1}.clear();" >> $_DST_FILE_C_
		shift
	done
}

function do_serialize()
{
	eval seriType=\$$#
    while [ $# -gt 1 ]
    do
		echo -e "\tw.${seriType}(${1});" >> $_DST_FILE_C_
        shift
    done
}

function do_deserialize()
{
	eval seriType=\$$#
	_CUR_INDEX_=$1
	shift
	if [ ${_STRUCT_MAX_INDX} = ${_CUR_INDEX_} ];then
		_ARRAY_LEN_=`expr $# - 1`
		indx=0
		while [ $# -gt 1 ]
		do
			indx=`expr $indx + 1`
			if [ ${indx} = ${_ARRAY_LEN_} ];then
				_DESE_FUNC_CONTENT=${_DESE_FUNC_CONTENT}"r.${seriType}(${1})){"
			else
				_DESE_FUNC_CONTENT=${_DESE_FUNC_CONTENT}"r.${seriType}(${1}) || "
			fi
			shift
		done
		
	else
		while [ $# -gt 1 ]
		do
			_DESE_FUNC_CONTENT=${_DESE_FUNC_CONTENT}"r.${seriType}(${1}) || "
			shift
		done
	fi
}

function do_reset()
{	
	eval seriType=\$$#
	while [ $# -gt 1 ]
	do
		#int
		if [ ${seriType} = 0 ];then
			echo -e "\t${1} = 0;" >> $_DST_FILE_C_
		#double	
		elif [ ${seriType} = 1 ];then
			echo -e "\t${1} = 0.0;" >> $_DST_FILE_C_
		#bool
		elif [ ${seriType} = 2 ];then
			echo -e "\t${1} = false;" >> $_DST_FILE_C_
		#std::string
		elif [ ${seriType} = 3 ];then
			echo -e "\t${1}.clear();" >> $_DST_FILE_C_
		##special do Custom type 
		elif [ ${seriType} = 4 ];then
			echo -e "\t${1}.reset();" >> $_DST_FILE_C_	
		fi
		shift
	done
}

function do_genconstruct()
{
	echo -e "\n$_STRUCT_NAME_::$_STRUCT_NAME_(void){" >> $_DST_FILE_C_
	echo -e "\treset();" >> $_DST_FILE_C_
	echo -e "}" >> $_DST_FILE_C_
}

function do_genserialize()
{
	#1.1.generate serialize function
	echo -e "\nvoid $_STRUCT_NAME_::serialize(Block_Buffer & w) const {" >> $_DST_FILE_C_
	if [ ${_STRUCT_NUMBER_FLAG_[0]} != 0 ];then
		#echo -e "\tw.write_int8(${_STRUCT_NUMBERS_[0]});" >> $_DST_FILE_C_	
		_ARRAY_=(${_STRUCT_NUMBERS_[0]})
		do_serialize "${_ARRAY_[@]}" write_int8
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[1]} != 0 ];then
		#echo -e "\tw.write_int16(${_STRUCT_NUMBERS_[1]});" >> $_DST_FILE_C_
		_ARRAY_=(${_STRUCT_NUMBERS_[1]})
		do_serialize "${_ARRAY_[@]}" write_int16
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[2]} != 0 ];then
		#echo -e "\tw.write_int32(${_STRUCT_NUMBERS_[2]});" >> $_DST_FILE_C_
		_ARRAY_=(${_STRUCT_NUMBERS_[2]})
		do_serialize "${_ARRAY_[@]}" write_int32
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[3]} != 0 ];then
		#echo -e "\tw.write_int64(${_STRUCT_NUMBERS_[3]});" >> $_DST_FILE_C_
		_ARRAY_=(${_STRUCT_NUMBERS_[3]})
		do_serialize "${_ARRAY_[@]}" write_int64
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[4]} != 0 ];then
		#echo -e "\tw.write_uint8(${_STRUCT_NUMBERS_[4]});" >> $_DST_FILE_C_
		_ARRAY_=(${_STRUCT_NUMBERS_[4]})
		do_serialize "${_ARRAY_[@]}" write_uint8
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[5]} != 0 ];then
		#echo -e "\tw.write_uint16(${_STRUCT_NUMBERS_[5]});" >> $_DST_FILE_C_
		_ARRAY_=(${_STRUCT_NUMBERS_[5]})
		do_serialize "${_ARRAY_[@]}" write_uint16
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[6]} != 0 ];then
		#echo -e "\tw.write_uint32(${_STRUCT_NUMBERS_[6]});" >> $_DST_FILE_C_
		_ARRAY_=(${_STRUCT_NUMBERS_[6]})
		do_serialize "${_ARRAY_[@]}" write_uint32
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[7]} != 0 ];then
		#echo -e "\tw.write_uint64(${_STRUCT_NUMBERS_[7]});" >> $_DST_FILE_C_
		_ARRAY_=(${_STRUCT_NUMBERS_[7]})
		do_serialize "${_ARRAY_[@]}" write_uint64
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[8]} != 0 ];then
		#echo -e "\tw.write_double(${_STRUCT_NUMBERS_[8]});" >> $_DST_FILE_C_
		_ARRAY_=(${_STRUCT_NUMBERS_[8]})
		do_serialize "${_ARRAY_[@]}" write_double
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[9]} != 0 ];then
		#echo -e "\tw.write_bool(${_STRUCT_NUMBERS_[9]});" >> $_DST_FILE_C_
		_ARRAY_=(${_STRUCT_NUMBERS_[9]})
		do_serialize "${_ARRAY_[@]}" write_bool
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[10]} != 0 ];then
		#echo -e "\tw.write_string(${_STRUCT_NUMBERS_[10]});" >> $_DST_FILE_C_
		_ARRAY_=(${_STRUCT_NUMBERS_[10]})
		do_serialize "${_ARRAY_[@]}" write_string
	fi
	
	#special do Custom type 
	if [ ${_STRUCT_NUMBER_FLAG_[11]} != 0 ];then
		#echo -e "\tw.write_string(${_STRUCT_NUMBERS_[10]});" >> $_DST_FILE_C_
		_ARRAY_=(${_STRUCT_NUMBERS_[11]})
		
		for data in ${_ARRAY_[@]}  
		do  
			echo -e "\t${data}.serialize(w);" >> $_DST_FILE_C_
		done
	fi
	
	#special do std::vetor<type>
	if [ ${_STRUCT_NUMBER_FLAG_[12]} != 0 ];then
		if [ _VECTOR_NUMBER_FLAG_[0] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[0]})
			do_vectorserialize "${_ARRAY_[@]}" write_int8
			
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[1] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[1]})
			do_vectorserialize "${_ARRAY_[@]}" write_int16
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[2] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[2]})
			do_vectorserialize "${_ARRAY_[@]}" write_int32
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[3] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[3]})
			do_vectorserialize "${_ARRAY_[@]}" write_int64
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[4] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[4]})
			do_vectorserialize "${_ARRAY_[@]}" write_uint8
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[5] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[5]})
			do_vectorserialize "${_ARRAY_[@]}" write_uint16
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[6] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[6]})
			do_vectorserialize "${_ARRAY_[@]}" write_uint32
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[7] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[7]})
			do_vectorserialize "${_ARRAY_[@]}" write_uint64
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[8] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[8]})
			do_vectorserialize "${_ARRAY_[@]}" write_double
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[9] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[9]})
			do_vectorserialize "${_ARRAY_[@]}" write_bool
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[10] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[10]})
			do_vectorserialize "${_ARRAY_[@]}" write_string
		fi
		
		#special do Custom type
		if [ _VECTOR_NUMBER_FLAG_[11] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[11]})
			i=0
			while [ ${i} -lt ${#_ARRAY_[@]} ]
			do
				echo -e "\tuint16_t __${_ARRAY_[$i]}_vec_size = ${_ARRAY_[$i]}.size();" >> $_DST_FILE_C_
				echo -e "\tw.write_uint16(__${_ARRAY_[$i]}_vec_size);" >> $_DST_FILE_C_
				echo -e "\tfor(uint16_t i = 0; i < __${_ARRAY_[$i]}_vec_size; ++i) {" >> $_DST_FILE_C_
				echo -e "\t\t${_ARRAY_[$i]}[i].serialize(w);" >> $_DST_FILE_C_
				echo -e "\t}\n" >> $_DST_FILE_C_
				let i=i+2
			done
			
		fi	
	fi
	
	echo -e "}" >> $_DST_FILE_C_
}

function do_gendeserialize()
{
	#1.2.generate deserialize function
	echo -e "\nint $_STRUCT_NAME_::deserialize(Block_Buffer & r) {" >> $_DST_FILE_C_
	if [ ${_STRUCT_NUMBER_FLAG_[0]} != 0 ];then
		_ARRAY_=(${_STRUCT_NUMBERS_[0]})
		do_deserialize 0 "${_ARRAY_[@]}" read_int8
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[1]} != 0 ];then
		_ARRAY_=(${_STRUCT_NUMBERS_[1]})
		do_deserialize 1 "${_ARRAY_[@]}" read_int16
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[2]} != 0 ];then
		_ARRAY_=(${_STRUCT_NUMBERS_[2]})
		do_deserialize 2 "${_ARRAY_[@]}" read_int32
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[3]} != 0 ];then	
		_ARRAY_=(${_STRUCT_NUMBERS_[3]})
		do_deserialize 3 "${_ARRAY_[@]}" read_int64
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[4]} != 0 ];then
		_ARRAY_=(${_STRUCT_NUMBERS_[4]})
		do_deserialize 4 "${_ARRAY_[@]}" read_uint8
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[5]} != 0 ];then
		_ARRAY_=(${_STRUCT_NUMBERS_[5]})
		do_deserialize 5 "${_ARRAY_[@]}" read_uint16
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[6]} != 0 ];then
		_ARRAY_=(${_STRUCT_NUMBERS_[6]})
		do_deserialize 6 "${_ARRAY_[@]}" read_uint32
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[7]} != 0 ];then
		_ARRAY_=(${_STRUCT_NUMBERS_[7]})
		do_deserialize 7 "${_ARRAY_[@]}" read_uint64
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[8]} != 0 ];then
		_ARRAY_=(${_STRUCT_NUMBERS_[8]})
		do_deserialize 8 "${_ARRAY_[@]}" read_double
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[9]} != 0 ];then
		_ARRAY_=(${_STRUCT_NUMBERS_[9]})
		do_deserialize 9 "${_ARRAY_[@]}" read_bool
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[10]} != 0 ];then
		_ARRAY_=(${_STRUCT_NUMBERS_[10]})
		do_deserialize 10 "${_ARRAY_[@]}" read_string
	fi
	
	#special do Custom type
	if [ ${_STRUCT_NUMBER_FLAG_[11]} != 0 ];then
		if [ ${_STRUCT_MAX_INDX} = 11 ];then
			_ARRAY_=(${_STRUCT_NUMBERS_[11]})
			_ARRAY_LEN_=${#_ARRAY_[@]} 
			indx=0
			for data in ${_ARRAY_[@]}  
			do  
				 indx=`expr $indx + 1`
				 if [ ${indx} = ${_ARRAY_LEN_} ];then
					_DESE_FUNC_CONTENT=${_DESE_FUNC_CONTENT}"${data}.deserialize(r)){"
				 else
					_DESE_FUNC_CONTENT=${_DESE_FUNC_CONTENT}"${data}.deserialize(r) || "
				 fi
			done
		else
			_ARRAY_=(${_STRUCT_NUMBERS_[11]})
			for data in ${_ARRAY_[@]}
			do  
				_DESE_FUNC_CONTENT=${_DESE_FUNC_CONTENT}"${data}.deserialize(r) || "
			done
		fi
	fi
	
	#special do std::vetor<type>
	if [ ${_STRUCT_NUMBER_FLAG_[12]} != 0 ];then
		if [ _VECTOR_NUMBER_FLAG_[0] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[0]})
			do_vectordeserialize read_int8 "${_ARRAY_[@]}" int8_t
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[1] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[1]})
			do_vectordeserialize read_int16 "${_ARRAY_[@]}" int16_t
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[2] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[2]})
			do_vectordeserialize read_int32 "${_ARRAY_[@]}" int32_t
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[3] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[3]})
			do_vectordeserialize read_int64 "${_ARRAY_[@]}" int64_t
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[4] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[4]})
			do_vectordeserialize read_uint8 "${_ARRAY_[@]}" uint8_t
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[5] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[5]})
			do_vectordeserialize read_uint16 "${_ARRAY_[@]}" uint16_t
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[6] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[6]})
			do_vectordeserialize read_uint32 "${_ARRAY_[@]}" uint32_t
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[7] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[7]})
			do_vectordeserialize read_uint64 "${_ARRAY_[@]}" uint64_t
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[8] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[8]})
			do_vectordeserialize read_double "${_ARRAY_[@]}" double
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[9] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[9]})
			do_vectordeserialize read_bool "${_ARRAY_[@]}" bool
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[10] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[10]})
			do_vectordeserialize read_string "${_ARRAY_[@]}" std::string
		fi
		
		#special do Custom type
		if [ _VECTOR_NUMBER_FLAG_[11] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[11]})	
			i=0
			j=0;
			while [ ${i} -lt ${#_ARRAY_[@]} ]
			do
				let j=i+1
				echo -e "\tuint16_t __${_ARRAY_[$i]}_vec_size;" >> $_DST_FILE_C_
				echo -e "\tif(r.read_uint16(__${_ARRAY_[$i]}_vec_size)  )" >> $_DST_FILE_C_
				echo -e "\t\treturn -1;" >> $_DST_FILE_C_
				echo -e "\t${_ARRAY_[$i]}.reserve(__${_ARRAY_[$i]}_vec_size);" >> $_DST_FILE_C_
				echo -e "\tfor(uint16_t i = 0; i < __${_ARRAY_[$i]}_vec_size; ++i) {" >> $_DST_FILE_C_
				echo -e "\t\t${_ARRAY_[$j]} v;" >> $_DST_FILE_C_
				echo -e "\t\tif(v.deserialize(r))" >> $_DST_FILE_C_
				echo -e "\t\t\treturn -1;" >> $_DST_FILE_C_
				echo -e "\t\t${_ARRAY_[$i]}.push_back(v);" >> $_DST_FILE_C_
				echo -e "\t}\n" >> $_DST_FILE_C_
				let i=i+2
			done
		fi
	fi
	
	if [ "${_DESE_FUNC_CONTENT}" != "" ];then
		echo -e "\tif ("$_DESE_FUNC_CONTENT >> $_DST_FILE_C_
		echo -e "\t\treturn -1;\n\t}" >> $_DST_FILE_C_
	fi
	
	echo -e "\treturn 0;" >> $_DST_FILE_C_
	echo -e "}" >> $_DST_FILE_C_
		
}

function do_genreset()
{
	echo -e "\nvoid $_STRUCT_NAME_::reset(){" >> $_DST_FILE_C_
	if [ ${_STRUCT_NUMBER_FLAG_[0]} != 0 ];then
		#echo -e "\t${_STRUCT_NUMBERS_[0]} = 0;" >> $_DST_FILE_C_
		_ARRAY_=(${_STRUCT_NUMBERS_[0]})
		do_reset "${_ARRAY_[@]}" 0
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[1]} != 0 ];then
		#echo -e "\t${_STRUCT_NUMBERS_[1]} = 0;" >> $_DST_FILE_C_
		_ARRAY_=(${_STRUCT_NUMBERS_[1]})
		do_reset "${_ARRAY_[@]}" 0
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[2]} != 0 ];then
		#echo -e "\t${_STRUCT_NUMBERS_[2]} = 0;" >> $_DST_FILE_C_
		_ARRAY_=(${_STRUCT_NUMBERS_[2]})
		do_reset "${_ARRAY_[@]}" 0
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[3]} != 0 ];then
		#echo -e "\t${_STRUCT_NUMBERS_[3]} = 0;" >> $_DST_FILE_C_
		_ARRAY_=(${_STRUCT_NUMBERS_[3]})
		do_reset "${_ARRAY_[@]}" 0
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[4]} != 0 ];then
		#echo -e "\t${_STRUCT_NUMBERS_[4]} = 0;" >> $_DST_FILE_C_
		_ARRAY_=(${_STRUCT_NUMBERS_[4]})
		do_reset "${_ARRAY_[@]}" 0
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[5]} != 0 ];then
		#echo -e "\t${_STRUCT_NUMBERS_[5]} = 0;" >> $_DST_FILE_C_
		_ARRAY_=(${_STRUCT_NUMBERS_[5]})
		do_reset "${_ARRAY_[@]}" 0
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[6]} != 0 ];then
		#echo -e "\t${_STRUCT_NUMBERS_[6]} = 0;" >> $_DST_FILE_C_
		_ARRAY_=(${_STRUCT_NUMBERS_[6]})
		do_reset "${_ARRAY_[@]}" 0
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[7]} != 0 ];then
		#echo -e "\t${_STRUCT_NUMBERS_[7]} = 0;" >> $_DST_FILE_C_
		_ARRAY_=(${_STRUCT_NUMBERS_[7]})
		do_reset "${_ARRAY_[@]}" 0
	fi
	
	
	if [ ${_STRUCT_NUMBER_FLAG_[8]} != 0 ];then
		#echo -e "\t${_STRUCT_NUMBERS_[8]} = 0.0;" >> $_DST_FILE_C_
		_ARRAY_=(${_STRUCT_NUMBERS_[8]})
		do_reset "${_ARRAY_[@]}" 1
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[9]} != 0 ];then
		#echo -e "\t${_STRUCT_NUMBERS_[9]} = false;" >> $_DST_FILE_C_
		_ARRAY_=(${_STRUCT_NUMBERS_[9]})
		do_reset "${_ARRAY_[@]}" 2
	fi
	
	if [ ${_STRUCT_NUMBER_FLAG_[10]} != 0 ];then
		#echo -e "\t${_STRUCT_NUMBERS_[1]}.clear();" >> $_DST_FILE_C_
		_ARRAY_=(${_STRUCT_NUMBERS_[10]})
		do_reset "${_ARRAY_[@]}" 3
	fi
	#special do Custom type
	if [ ${_STRUCT_NUMBER_FLAG_[11]} != 0 ];then
		#echo -e "\t${_STRUCT_NUMBERS_[1]}.clear();" >> $_DST_FILE_C_
		_ARRAY_=(${_STRUCT_NUMBERS_[11]})
		do_reset "${_ARRAY_[@]}" 4
	fi
	
	#special do std::vetor<type>
	if [ ${_STRUCT_NUMBER_FLAG_[12]} != 0 ];then
		if [ _VECTOR_NUMBER_FLAG_[0] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[0]})
			do_vectorreset "${_ARRAY_[@]}"
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[1] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[1]})
			do_vectorreset "${_ARRAY_[@]}"
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[2] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[2]})
			do_vectorreset "${_ARRAY_[@]}"
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[3] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[3]})
			do_vectorreset "${_ARRAY_[@]}"
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[4] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[4]})
			do_vectorreset "${_ARRAY_[@]}"
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[5] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[5]})
			do_vectorreset "${_ARRAY_[@]}"
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[6] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[6]})
			do_vectorreset "${_ARRAY_[@]}"
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[7] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[7]})
			do_vectorreset "${_ARRAY_[@]}"
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[8] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[8]})
			do_vectorreset "${_ARRAY_[@]}"
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[9] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[9]})
			do_vectorreset "${_ARRAY_[@]}"
		fi
		
		if [ _VECTOR_NUMBER_FLAG_[10] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[10]})
			do_vectorreset "${_ARRAY_[@]}"
		fi
		
		#special do Custom type
		if [ _VECTOR_NUMBER_FLAG_[11] != 0 ];then
			_ARRAY_=(${_VECTOR_NUMBERS_[11]})	
			i=0;
			while [ ${i} -lt ${#_ARRAY_[@]} ]
			do
				echo -e "\t${_ARRAY_[$i]}.clear();" >> $_DST_FILE_C_
				let i=i+2
			done
		fi
	fi
	
	echo -e "}" >> $_DST_FILE_C_
}

function do_gencppfile()
{
	#1.construnct func
	do_genconstruct
	
	#2.serialize func
	do_genserialize
	
	#3.deserialize func
	do_gendeserialize
	
	#4.reset func
	do_genreset
}

function do_genhfile()
{
	echo -e "\n\t${_STRUCT_NAME_}(void);" >> $_DST_FILE_H_ 
	echo -e "\n\tvoid serialize(Block_Buffer & w) const;" >> $_DST_FILE_H_ 
	echo -e "\n\tint deserialize(Block_Buffer & r);" >> $_DST_FILE_H_ 
	echo -e "\n\tvoid reset(void);" >> $_DST_FILE_H_ 
	echo -e "};" >> $_DST_FILE_H_ 
}

function do_resetreadvar()
{
	_STRUCT_NAME_="struct"
	_STRUCT_NUMBERS_=("" "" "" "" "" "" "" "" "" "" "" "" "")
	_STRUCT_NUMBER_FLAG_=(0 0 0 0 0 0 0 0 0 0 0 0 0)
	_VECTOR_NUMBERS_=("" "" "" "" "" "" "" "" "" "" "" "")
	_VECTOR_NUMBER_FLAG_=(0 0 0 0 0 0 0 0 0 0 0 0)
	_STRUCT_NUMBER_TYPE="struct"
	_DESE_FUNC_CONTENT=""
	_STRUCT_MAX_INDX=0
	_CUR_INDEX_=0
	_STRUCT_NOTE_=0
	_ARRAY_=""
	_ARRAY_LEN_=""
}

function do_setmaxstructindx()
{
	if [ ${_STRUCT_MAX_INDX} -lt ${1} ];then
		_STRUCT_MAX_INDX=${1}
	fi
}

function do_readfile()
{
	cat $_SRC_FILE_|sed '/^$/d'|while read _LINE_
	do
		case $_LINE_ in
		*{*)
			echo $_LINE_ >> $_DST_FILE_H_
			_STRUCT_NAME_=`echo $_LINE_ |awk '{print $2}'`
		;;
		*int*|*double*|*bool*|*string*|*std::vector\<*)
			echo -e "\n\t"$_LINE_ >> $_DST_FILE_H_
			if [ ${_STRUCT_NOTE_} = 0 ];then
				case `echo $_LINE_ |awk '{print $1}'` in
				std::vector\<*)
					_STRUCT_NUMBER_FLAG_[12]=1
					case `echo $_LINE_ |awk '{print $1}'` in
						*int8_t*)
							_VECTOR_NUMBERS_[0]=${_VECTOR_NUMBERS_[0]}`echo $_LINE_ |awk '{print $2}'|awk -F ";" '{print $1" "}'`
							_VECTOR_NUMBER_FLAG_[0]=1
						;;
						*int16_t*)
							_VECTOR_NUMBERS_[1]=${_VECTOR_NUMBERS_[1]}`echo $_LINE_ |awk '{print $2}'|awk -F ";" '{print $1" "}'`
							_VECTOR_NUMBER_FLAG_[1]=1
						;;
						*int32_t*)
							_VECTOR_NUMBERS_[2]=${_VECTOR_NUMBERS_[2]}`echo $_LINE_ |awk '{print $2}'|awk -F ";" '{print $1" "}'`
							_VECTOR_NUMBER_FLAG_[2]=1
						;;
						*int64_t*)
							_VECTOR_NUMBERS_[3]=${_VECTOR_NUMBERS_[3]}`echo $_LINE_ |awk '{print $2}'|awk -F ";" '{print $1" "}'`
							_VECTOR_NUMBER_FLAG_[3]=1
						;;
						*uint8_t*)
							_VECTOR_NUMBERS_[4]=${_VECTOR_NUMBERS_[4]}`echo $_LINE_ |awk '{print $2}'|awk -F ";" '{print $1" "}'`
							_VECTOR_NUMBER_FLAG_[4]=1
						;;
						*uint16_t*)
							_VECTOR_NUMBERS_[5]=${_VECTOR_NUMBERS_[5]}`echo $_LINE_ |awk '{print $2}'|awk -F ";" '{print $1" "}'`
							_VECTOR_NUMBER_FLAG_[5]=1
						;;
						*uint32_t*)
							_VECTOR_NUMBERS_[6]=${_VECTOR_NUMBERS_[6]}`echo $_LINE_ |awk '{print $2}'|awk -F ";" '{print $1" "}'`
							_VECTOR_NUMBER_FLAG_[6]=1
						;;
						*uint64_t*)
							_VECTOR_NUMBERS_[7]=${_VECTOR_NUMBERS_[7]}`echo $_LINE_ |awk '{print $2}'|awk -F ";" '{print $1" "}'`
							_VECTOR_NUMBER_FLAG_[7]=1
						;;
						*double*)
							_VECTOR_NUMBERS_[8]=${_VECTOR_NUMBERS_[8]}`echo $_LINE_ |awk '{print $2}'|awk -F ";" '{print $1" "}'`
							_VECTOR_NUMBER_FLAG_[8]=1
						;;
						*bool*)
							_VECTOR_NUMBERS_[9]=${_VECTOR_NUMBERS_[9]}`echo $_LINE_ |awk '{print $2}'|awk -F ";" '{print $1" "}'`
							_VECTOR_NUMBER_FLAG_[9]=1
						;;
						*std::string*)
							_VECTOR_NUMBERS_[10]=${_VECTOR_NUMBERS_[10]}`echo $_LINE_ |awk '{print $2}'|awk -F ";" '{print $1" "}'`
							_VECTOR_NUMBER_FLAG_[10]=1
						;;
						*)#special do Custom type
							customType=`echo $_LINE_ |awk '{print $1}'`
							customType=`echo ${customType#*<}`
							customType=`echo ${customType%>*}`
							_VECTOR_NUMBERS_[11]=${_VECTOR_NUMBERS_[11]}`echo $_LINE_ |awk '{print $2}'|awk -F ";" '{print $1" "}'`
							_VECTOR_NUMBERS_[11]=${_VECTOR_NUMBERS_[11]}$customType" "
							_VECTOR_NUMBER_FLAG_[11]=1
						;;
					esac
				;;
				int8_t*)
					_STRUCT_NUMBERS_[0]=${_STRUCT_NUMBERS_[0]}`echo $_LINE_ |awk '{print $2}'|awk -F ";" '{print $1" "}'`
					do_setmaxstructindx 0
					_STRUCT_NUMBER_FLAG_[0]=1
				;;
				int16_t*)
					_STRUCT_NUMBERS_[1]=${_STRUCT_NUMBERS_[1]}`echo $_LINE_ |awk '{print $2}'|awk -F ";" '{print $1" "}'`
					do_setmaxstructindx 1
					_STRUCT_NUMBER_FLAG_[1]=1
				;;
				int32_t*)
					_STRUCT_NUMBERS_[2]=${_STRUCT_NUMBERS_[2]}`echo $_LINE_ |awk '{print $2}'|awk -F ";" '{print $1" "}'`
					do_setmaxstructindx 2
					_STRUCT_NUMBER_FLAG_[2]=1
				;;
				int64_t*)
					_STRUCT_NUMBERS_[3]=${_STRUCT_NUMBERS_[3]}`echo $_LINE_ |awk '{print $2}'|awk -F ";" '{print $1" "}'`
					do_setmaxstructindx 3
					_STRUCT_NUMBER_FLAG_[3]=1
				;;
				uint8_t*)
					_STRUCT_NUMBERS_[4]=${_STRUCT_NUMBERS_[4]}`echo $_LINE_ |awk '{print $2}'|awk -F ";" '{print $1" "}'`
					do_setmaxstructindx 4
					_STRUCT_NUMBER_FLAG_[4]=1
				;;
				uint16_t*)
					_STRUCT_NUMBERS_[5]=${_STRUCT_NUMBERS_[5]}`echo $_LINE_ |awk '{print $2}'|awk -F ";" '{print $1" "}'`
					do_setmaxstructindx 5
					_STRUCT_NUMBER_FLAG_[5]=1
				;;
				uint32_t*)
					_STRUCT_NUMBERS_[6]=${_STRUCT_NUMBERS_[6]}`echo $_LINE_ |awk '{print $2}'|awk -F ";" '{print $1" "}'`
					do_setmaxstructindx 6
					_STRUCT_NUMBER_FLAG_[6]=1
				;;
				uint64_t*)
					_STRUCT_NUMBERS_[7]=${_STRUCT_NUMBERS_[7]}`echo $_LINE_ |awk '{print $2}'|awk -F ";" '{print $1" "}'`
					do_setmaxstructindx 7
					_STRUCT_NUMBER_FLAG_[7]=1
				;;
				double*)
					_STRUCT_NUMBERS_[8]=${_STRUCT_NUMBERS_[8]}`echo $_LINE_ |awk '{print $2}'|awk -F ";" '{print $1" "}'`
					do_setmaxstructindx 8
					_STRUCT_NUMBER_FLAG_[8]=1
				;;
				bool*)
					_STRUCT_NUMBERS_[9]=${_STRUCT_NUMBERS_[9]}`echo $_LINE_ |awk '{print $2}'|awk -F ";" '{print $1" "}'`
					do_setmaxstructindx 9
					_STRUCT_NUMBER_FLAG_[9]=1
				;;
				std::string*)
					_STRUCT_NUMBERS_[10]=${_STRUCT_NUMBERS_[10]}`echo $_LINE_ |awk '{print $2}'|awk -F ";" '{print $1" "}'`
					do_setmaxstructindx 10
					_STRUCT_NUMBER_FLAG_[10]=1
				;;
				esac;
			fi
		;;
		*}*)
			#1.generate *.h file
			do_genhfile
			#2.generate *.cpp file
			do_gencppfile
			#3.reset read variable
			do_resetreadvar
		;;
		*\/\**)
			_STRUCT_NOTE_=1
			echo "/*" >> $_DST_FILE_H_
		;;
		*\*\/*)
			_STRUCT_NOTE_=0
			echo "*/" >> $_DST_FILE_H_
		;;
		*)#special do Custom type
			echo -e "\n\t"$_LINE_ >> $_DST_FILE_H_
			if [ ${_STRUCT_NOTE_} = 0 ];then
				_STRUCT_NUMBERS_[11]=${_STRUCT_NUMBERS_[11]}`echo $_LINE_ |awk '{print $2}'|awk -F ";" '{print $1" "}'`
				do_setmaxstructindx 11
				_STRUCT_NUMBER_FLAG_[11]=1
			fi
		;;
		esac
	done
}

function main()
{
	do_readfile
}

while getopts h option
do
    case "$option" in
        h)
            echo "Useage:$0 msg_file.txt msg_file.h msg_file.cpp"
            exit 1;;
    esac
done

if [ $# -ne 3 ];then
	help
	exit 1
fi

main

echo "<<<<<<<generate file over!!>>>>>>>"