#! /bin/sh

curdir=$(dirname $0)

color_red="\e[31m"
color_green="\e[32m"
color_blue="\e[34m"
color_bold="\e[;01m"
color_off="\e[0m"

cfiles=$@

res=0


logfile=$(mktemp)
touch $logfile
for file in $cfiles; do
    printf "Checking coding style in $file..."
    "$curdir/checkpatch.pl" --color --terse --no-tree --ignore NEW_TYPEDEFS,QUOTED_WHITESPACE_BEFORE_NEWLINE,GLOBAL_INITIALISERS,INITIALISED_STATIC,PREFER_ALIGNED,LONG_LINE,LONG_LINE_COMMENT,LONG_LINE_STRING,VOLATILE,LEADING_SPACE,SPACE_BEFORE_TAB,CODE_INDENT,LINE_CONTINUATIONS --file $file >> $logfile
    new_res=$?
    res=$(($res + $new_res))
done

if [ $res -eq 0 ]; then
    printf " [${color_green}OK${color_off}]\n"
    cat $logfile
else
    printf " [${color_red}KO${color_off}]\n"
    cat $logfile
fi
rm -f $logfile
exit $res
