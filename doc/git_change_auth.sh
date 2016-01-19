#!/bin/sh
git filter-branch --env-filter '
 
an="$GIT_AUTHOR_NAME"
am="$GIT_AUTHOR_EMAIL"
cn="$GIT_COMMITTER_NAME"
cm="$GIT_COMMITTER_EMAIL"
 
if [ "$GIT_COMMITTER_EMAIL" = "jice1990@163.com" ]
then
    cn="jice1001"
    cm="784599938@qq.com"
fi
if [ "$GIT_AUTHOR_EMAIL" = "jice1990@163.com" ]
then
    an="jice1001"
    am="784599938@qq.com"
fi
 
export GIT_AUTHOR_NAME="$an"
export GIT_AUTHOR_EMAIL="$am"
export GIT_COMMITTER_NAME="$cn"
export GIT_COMMITTER_EMAIL="$cm"
'
