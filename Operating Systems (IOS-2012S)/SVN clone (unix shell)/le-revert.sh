#!/bin/sh

EXPDIR=`(pwd)`
REVDIR=$EXPDIR/.le
IGNCOUNT=`(grep "^ignore" $REVDIR/.config|cut -f2 -d' '|sort -u|wc -l)`
#read -a array <<< $(grep "^ignore " $REVDIR/.config | cut -f2 -d' '|tr "\n" " ") #ignorovane eregexpy
if [ ! -d ".le" ] 
then
   echo "ERROR: .le directory does not exists!"
   exit 1
fi
if [ "$#" = 0 ] 
then
   COUNTER=`ls`
else
   COUNTER=$@
fi
cd $REVDIR
   for i in $COUNTER
   do
   if echo "$i"| grep "^[.]">/dev/null 2>&1
   then 
      continue
   fi
#      for ign in "${array[@]}" 
#       do
#         if [[ $i =~ $ign ]]
#            then
#            continue 2
#         fi
#      done
   j=1
   while [ $j -le $IGNCOUNT ]
   do
      ign=$(grep "^ignore" $REVDIR/.config|sed 's/ignore //'|sort -u|sed -n ${j}p)
      if echo "$i" | egrep "$ign">/dev/null 2>&1
      then	
         continue 2 
      fi
     let j=j+1>/dev/null 2>&1
   done
      cp $EXPDIR/.le/$i $EXPDIR>/dev/null 2>&1 
  done
#############IOS>PROJEKT_1##############
#######Made by Martin_Fajčík@FITVUT@2013
########################################
