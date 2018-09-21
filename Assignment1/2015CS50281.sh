#!/bin/bash
if [ "$#" -eq 4 ]
then
	if [ $3 == "-apriori" ]
	then ./apriori $1 $4 $2
	elif [ $3 == "-fptree" ]
	then ./fptree $1 $4 $2
	fi
elif [ "$#" -eq 2 ]
then
	if [ $2 == "-plot" ]
	then 
		python3 plot.py $1
		rm apriori1.txt apriori5.txt apriori10.txt apriori25.txt apriori50.txt apriori90.txt
        rm fptree1.txt fptree5.txt fptree10.txt fptree25.txt fptree50.txt fptree90.txt
	fi
fi