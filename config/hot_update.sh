#update config

#path
temp_path=$(dirname "$0")
cd $temp_path
real_path=$(pwd)

#check number of params
if [ $# -lt 1 ]; then
	echo "Error: Too few params Example:sh hot_update.sh npc"
	exit 1
fi

#check params. can't have two same params
declare -a argv_list
argv_list_count=0
while [ $# -ne 0 ]
do
	a=$1
	a=${a%/*}
	argv_list[$argv_list_count]=$a
	argv_list_count=$[ $argv_list_count + 1 ]
	shift
done

for value in ${argv_list[*]}
do
	argv_count=0
	for value1 in ${argv_list[*]}
	do
		if [ $value = $value1 ]; then
			argv_count=$[ $argv_count + 1 ]
		fi
	done
	if [ $argv_count -gt 1 ]; then
		echo "Error: There is two params \"$a\""
		exit 1
	fi
done

#check directory
#for value in ${argv_list[*]}
#do
#	file_count=$(ls | grep -xc $value)
#	if [ $file_count -lt 1 ]; then		
#		echo "Error: No such directory: $value"
#		exit 1
#	fi
#done

#construct the cmd
cmd=""
for value in ${argv_list[*]}
do
	if [ -z $cmd ]; then
		cmd="\"$value\""
	else
		cmd="$cmd, \"$value\""
	fi
done

#modify cmd.json
timestamp=$[$(date +%s)+80]
echo -e "{
	\"update_timestamp\" : $timestamp,
	\"time\" : \"$(date)\",
	\"update_list\" : [$cmd]
}" > $temp_path/../update.json

#done
echo "Done: update config $cmd after 2 minute"
