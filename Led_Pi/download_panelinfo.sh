#!/bin/bash
cd ./led_panelen
while true
do
	rm ./Paneel1.txt
	rm ./Paneel2.txt
	rm ./Paneel3.txt
	wget -q https://www.dropbox.com/s/rre1ltbg3hjuvbu/Paneel1.txt
	wget -q https://www.dropbox.com/s/l0duqrfes1fl3f2/Paneel2.txt
	wget -q https://www.dropbox.com/s/gblnjsabqutpwnx/Paneel3.txt
	sleep 60
done
