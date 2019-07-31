#!/bin/bash

# sync local code to wsl. and then build the project
# ./exe.sh project_name

work_path=`pwd`
cd $work_path

if [ $# -lt 1 ]; then
    echo 'pls input project name'
    exit 1
fi

#echo $work_path
project_path=$work_path/$1
#echo $project_path
if [ ! -d $project_path ]; then
    echo 'pls mkdir project dir'
    exit 1
fi

../../sync_win_code.sh

cd $project_path

g++ -Wall -g *.cpp -o main
if [ $? -eq 0 ]; then 
    ./main
fi
