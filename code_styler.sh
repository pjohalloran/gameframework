#!/bin/sh

function should_style_file {
	file_exts=( '.h' '.c' '.cpp' )
	filename=$(basename "$1")
	extension="${filename##*.}"
	filename="${filename%.*}"

	for i in "${file_exts[@]}"; do
	#for (( i=0; i<=$(( ${#file_exts}}-1 )); i++ )); do
		if [ ".$extension" == "$i" ]; then
			echo "$1 is valid"
			return 0
		fi
	done

	return 1
}

function style_files {
	if [[ $1 =~ .*3rdParty.* ]]; then
		echo "Skipping 3rdParty dir"
		return
	fi

	if [ ! -d $1 ]; then
		should_style_file $file
		if [ $? -eq 0 ]; then
			echo "Styling $1"
			astyle $file --indent=spaces --style=stroustrup --unpad-paren --add-brackets --convert-tabs --lineend=linux --pad-oper --indent-namespaces --indent-switches
		else
			echo "Skipping $1"
		fi
	elif [ -d $1 ]; then
		echo "Going down into $1"
		for file in $1/*; do
			style_files $file
		done
	fi
}

curr_dir=`pwd`
tmp="$curr_dir/src"
echo "Running on $tmp"
style_files $tmp
