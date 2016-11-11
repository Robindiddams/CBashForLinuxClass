#!/bin/bash

users=`cat /etc/passwd | awk -F: '{ print $1 }'`

echo $users
