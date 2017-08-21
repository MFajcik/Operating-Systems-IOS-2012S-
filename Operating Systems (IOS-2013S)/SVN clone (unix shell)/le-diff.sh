#!/bin/sh
EXPDIR=`(pwd)`
REVDIR=$EXPDIR/.le
PROJDIR=`(grep '^projdir ' $REVDIR/.config |cut -f2 -d' '|head -n 1)`
IGNCOUNT=`(grep "^ignore" $REVDIR/.config|cut -f2 -d' '|sort -u|wc -l)`
if [ -z $PROJDIR ]
   then
   echo "Unable to read project directory from .le/.config. Exiting..."
   exit 1
fi
#read -a array <<< $(grep "^ignore " $REVDIR/.config | cut -f2 -d' '|tr "\n" " ") #ignorovane eregexpy
if [ $# = 0 ]
   then
   cd $EXPDIR #obsah projdir+expdir
   COUNTER=$(ls -l | grep ^- | awk '{print $9}')  #z ls vystrihnem priecinky
   cd $PROJDIR
   COUNTER="$COUNTER $(ls -l | grep ^- | awk '{print $9}')"
   COUNTER=$(echo $COUNTER|tr " " "\n"|sort -u)
else
   COUNTER=$@
fi
for i in $COUNTER
   do
   if echo "$i"| grep "^[.]">/dev/null 2>&1
   then 
      continue
   fi
# ak je subor ignorovany------ continue na dalsi--------------------------------------------------

#   for ign in "${array[@]}"
#   do
#     if [[ $i =~ $ign ]]
#     then
#       continue 2
#     fi
#   done
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
#-------------------------------------------------------------------------------------------------      
      cond=0
      if [ -f "$EXPDIR/$i" -a ! -f "$PROJDIR/$i" ]
         then
         echo D: $i
  
      elif [ -f "$PROJDIR/$i" -a ! -f "$EXPDIR/$i" ]
         then
         echo C: $i
      elif [ -f "$PROJDIR/$i" -a -f "$EXPDIR/$i" ]
         then
   	 diff -u "$PROJDIR/$i" "$EXPDIR/$i"
	 if [ $? = 0 ]
	    then
            echo .: $i
         fi
      fi
done
##########IOS>PROJEKT_1#################
#######Made by Martin_Fajčík@FITVUT@2013
########################################
