#!/bin/bash

rsync -az /mnt/c/src/other/mytest /home/work --exclude=.git --exclude=.vscode --exclude=*.log --exclude=*.exe