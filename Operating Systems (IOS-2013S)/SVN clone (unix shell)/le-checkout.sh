#!/bin/sh
#$1 == PROJDIR
EXPDIR=`(pwd)`
if [ -d ".le" ]
then
        rm $EXPDIR/.le/* >/dev/null 2>&1
        #ignorovane eregexpy
#        read -a array <<< $(grep "^ignore " $EXPDIR/.le/.config | cut -f2 -d' '|tr "\n" " ") 
else	
	mkdir ".le"
fi

if [ ! -d "$1" ]
then
	echo ERROR: Invalid  path to project directory.
        exit 1
fi
cd $1
#kopcenie
if [ -f "$EXPDIR/.le/.config" ]
then
   IGNCOUNT=`(grep "^ignore" $EXPDIR/.le/.config |cut -f2 -d' '|sort -u|wc -l)`
fi
for i in `ls`
   do
# ak je subor ignorovany------ continue na dalsi--------------------------------------------------
 #  for ign in "${array[@]}"
 #     do
 #     if [[ $i =~ $ign ]]
 #     then   
 #       continue 2
 #     fi
 #  done
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
   cp ./$i $EXPDIR/.le>/dev/null 2>&1
   cp ./$i $EXPDIR>/dev/null 2>&1
done
#zapis do configu
if [ -f "$EXPDIR/.le/.config" ]
   then
   grep '^projdir' $EXPDIR/.le/.config>/dev/null 2>&1
   if [ $? -ne 0 ]
      then
      echo "projdir $1">>$EXPDIR/.le/.config
   else
      sed "s@projdir.*@projdir $1@" $EXPDIR/.le/.config>$EXPDIR/.le/.config.tmp
      cat $EXPDIR/.le/.config.tmp>$EXPDIR/.le/.config
      rm $EXPDIR/.le/.config.tmp
   fi
else
   echo "projdir $1">$EXPDIR/.le/.config
fi
#############IOS>PROJEKT_1##############
#######Made by Martin_Fajčík@FITVUT@2013
########################################
