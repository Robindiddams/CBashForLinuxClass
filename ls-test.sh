#!/bin/bash

#This code was written by Robin Diddams, Nils Steinbugl, Chiara Piazzolla, and Rachel Lewis
#nobody cheated
#CSI-230-03
#this is tested and works on archlinux(native)


#make an array of users
# IFS=' ' read -r -a users <<< `cat /etc/passwd | awk -F: '{ print $1 }'`

myls=(`./ls`)
mylsa=(`./ls -a`)
mylsl=(`./ls -l`)
mylsla=(`./ls -la`)

ls=(`ls`)
lsa=(`ls -a`)
lsl=(`ls -l`)
lsla=(`ls -la`)
echo "calling with no args: "
echo
ls
./ls

echo
echo

echo "calling with a few options"
echo
ls -a
./ls -a
echo
ls -l
./ls -l
echo
ls -la
./ls -la
echo
echo

echo "calling with a file"
echo
ls ls.c
./ls ls.c

#no matter what arguments i give so long as the las one is a file it will always return the path

echo "calling with an invalid file"
echo
ls ls.q.qweqwe
./ls ls.q.qweqwe
#error checking happens first, giving extra params will make no difference

echo
echo


echo "for comparisons:"
printf "ls: "
printf "%s " ${ls[@]}
printf "\n"
printf "ls: "
printf "%s " ${myls[@]}
printf "\n"
