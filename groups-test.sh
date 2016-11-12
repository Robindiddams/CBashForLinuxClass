#!/bin/bash

#This code was written by Robin Diddams, Nils Steinbugl, Chiara Piazzolla, and Rachel Lewis
#nobody cheated
#CSI-230-03
#this is tested and works on linux mint(vm), archlinux(native), and centos(hawk)


#make an array of users
IFS=' ' read -r -a users <<< `cat /etc/passwd | awk -F: '{ print $1 }'`

echo "calling with no args: "
./groups

echo
echo

echo "calling with multiple valid users as args: "
COUNTER=0
while [  $COUNTER -lt $((${#users[@]} - 3)) ]; do
	#just calls three at a time
	#  echo The counter is $COUNTER
	./groups ${users[$COUNTER]} ${users[$(($COUNTER + 1))]} ${users[$(($COUNTER + 3))]}
	((COUNTER=COUNTER+1))
done

echo
echo

echo "Calling with an invalid user as arg"
./groups alskdfpaonweuhfj

echo
echo

echo "Comparing to groups command"

#make arrays of groups output
IFS=' ' read -r -a compgroups <<< `groups root`
IFS=' ' read -r -a compgroups2 <<< `./groups root`

#sort both arrays of outputs
arr1=( $(
	for el in "${compgroups[@]}"
	do
		echo "$el"
	done | sort) )

arr2=( $(
	for el in "${compgroups2[@]:1}" #you can remove this ":1" to make this be false
	do
		echo "$el"
	done | sort) )

# echo ${arr1[@]}
# echo ${arr2[@]}

 #compile both arrays and sort them and then get the unique items
#unique items will be the ones that exist in one set and not the other
arr3=(`echo ${arr1[@]} ${arr2[@]} | tr ' ' '\n' | sort | uniq -u`)

# echo ${#arr3[@]}

#if there are no unique items then it worked
if [ ${#arr3[@]} -gt 0 ]; then
	echo "ERROR MISSMATCH ARRAYS, groups COMMAND OUTPUT DOES NOT MATCH ./groups"
else
	echo "Comparison true, the command outputs match"
fi
