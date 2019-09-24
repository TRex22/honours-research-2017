#!/bin/bash

find . -type f | while read line; do
	echo $line
	cat $line | grep new
done
