#!/bin/bash

USER_NAME="JIYU"
INPUT=/tmp/menu.sh.$$

show_intro() {
	dialog --backtitle "The Book Manager" \
		--infobox "Welcome to\n$1's Book Manager" 10 30 
	sleep 2
}

show_list() {
	echo "show list"
}

show_regist() {
	title=""
	author=""
	publisher=""
	read_date=""

	dialog --ok-label "Regist" \
		--backtitle "The Book Manager" \
		--title "Regist a new book" \
		--form "Create a new book information" 15 50 0\
		"Title:"	1 1 "$title"		1 15 25 0 \
		"Author:"	2 1 "$author"		2 15 25 0 \
		"Publisher:"	3 1 "$publisher"	3 15 25 0 \
		"Date:"		4 1 "$read_date"	4 15 12 0
}

show_intro ${USER_NAME}

while true
do
	dialog --clear --backtitle "The Book Manager" \
		--title " [M A I N   M E N U]" \
		--menu "You can use the UP/DOWN arrow keys, the first \
		letter of the choice as a hot key, or the \
		number keys 1-9 to choose an option.\n \
		Choose the TASK" 15 50 4 \
		List "Show the book list in the DB" \
		Regist "Register a new book" \
		Delete "Delete a book in the DB" \
		Exit "Exit to the shell" 2>"${INPUT}"

menu_item=$(<"${INPUT}")

# make decsion
case $menu_item in
	List) show_list;;	
	Regist) show_regist;;
	Delete) show delete;;
	Exit) dialog --infobox "Bye!!" 10 25; break;
esac
done

# if a temp file found, delete it
[ -f $INPUT ] && rm ${INPUT}

clear













