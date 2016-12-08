#!/bin/bash
# // #This code was written by Robin Diddams, Nils Steinbugl, Chiara Piazzolla, and Rachel Lewis
# // #nobody cheated
# // #CSI-230-03
# // this was tested and works on ubuntu(64bit-native), and probably linux mint :/


# make a file to copy
echo "this is a file" > testfile.txt

# copy the file
./cp testfile.txt heyy.txt

# cleanup
rm heyy.txt && rm testfile.txt

# try to cp a nono file
./cp dirtythings.qqq heyy.txt

# make a directory with a file in it
mkdir beans && touch beans/bigbean.txt

# see directory
ls

# copy directory
./cp -r beans cptest

# see into new directory
ls cptest/

#remove cptest
rm -r cptest/ beans/
