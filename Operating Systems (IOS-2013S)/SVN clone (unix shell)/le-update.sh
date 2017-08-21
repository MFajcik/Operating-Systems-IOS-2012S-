#!/bin/sh

EXPDIR=`(pwd)`
REVDIR="$EXPDIR/.le"
PROJDIR=`(grep '^projdir ' $REVDIR/.config |cut -f2 -d' '|head -n 1)`
if [ -z $PROJDIR ]
   then
   echo "Unable to read project directory from ~/.le/.config. Exiting..."
   exit 1
fi
IGNCOUNT=`(grep "^ignore" $REVDIR/.config|cut -f2 -d' '|sort -u|wc -l)`
PROJEXP=0
PROJREF=0
REFEXP=0
#ignorovane eregexpy do pola ... * system ignoru v bashi 200% efektivnejsi ale v starom shelli niesu polia...
#read -a array <<< $(grep "^ignore " $REVDIR/.config | cut -f2 -d' '|tr "\n" " ") 
if [ $# = 0 ]
   then
   #prechadzam subory v projdir,expdir,refdir
   cd $REVDIR
   COUNTER="$COUNTER $(ls -l | grep ^- | awk '{print $9}')" 
   cd $EXPDIR
   COUNTER=$(ls -l | grep ^- | awk '{print $9}')  #z ls vystrihnem priecinky
   cd $PROJDIR
   COUNTER="$COUNTER $(ls -l | grep ^- | awk '{print $9}')"
   COUNTER=$(echo $COUNTER|tr " " "\n"|sort -u) 
else
   COUNTER=$@
fi

for i in $COUNTER
   do
# ak je subor ignorovany------ continue na dalsi--------------------------------------------------

 # for ign in "${array[@]}"
 #  do
 #    if [[ $i == $ign ]] ### 200% efektivnejsi sposob ale nefunguje v starom shelli > polia...
 #    then
 #      continue 2
 #    fi
 #  done
   if echo "$i"| grep "^[.]">/dev/null 2>&1
   then 
      continue
   fi
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
# nulovanie konstant
	PROJREF=0
	PROJEXP=0  #vysledky diffov
	REFEXP=0

        PROJEXT=0
        EXPEXT=0 #existencia suborov
        REFEXT=0
#        Porovnavacia cast
#-----------------------------------------------------------------------------------------
 	#Porovnanie PROJ z EXP kopiou  
   diff -u "$PROJDIR/$i" "$EXPDIR/$i" >/dev/null 2>&1
   if [ $? = 0 ]
      then
      PROJEXP=1
   fi
	#Porovnanie  PROJ z Reverznou kopiou
   diff -u "$REVDIR/$i" "$i" >$EXPDIR/difflogPREF.patch 2>&1
   if [ $? = 0 ]
      then
      PROJREF=1
   fi
	#Porovnanie EXP z Reverznou kopiou
   diff -u "$EXPDIR/$i" "$REVDIR/$i" >/dev/null 2>&1
   if [ $? = 0 ]
      then
      REFEXP=1
   fi
#existuju vobec?
   if [ -f "$PROJDIR/$i" ]
   then 
      PROJEXT=1
   fi
   
   if [ -f "$EXPDIR/$i" ]
   then 
      EXPEXT=1
   fi

   if [ -f "$REVDIR/$i" ]
   then 
      REFEXT=1
   fi
#			VETVIACA CAST
#-----------------------------------------------------------------------------------------
   if [ $PROJEXT = 1 -a $EXPEXT = 0 ] #6 existuje v PROJDIR, NEexistuje v EXPDIR 
      then
      echo C: $i
      cp $PROJDIR/$i $EXPDIR>/dev/null 2>&1
      cp $PROJDIR/$i $REVDIR>/dev/null 2>&1
   elif [ $PROJEXT = 0 -a $REFEXT = 1 ] #7 existuje v REFDIR, neexistuje v PROJDIR
      then
      echo D: $i
      rm $REVDIR/$i>/dev/null 2>&1
      rm $EXPDIR/$i>/dev/null 2>&1
   elif [ $PROJEXP = 1 -a $PROJREF = 1 ] #1
      then
      echo .: $i 
   elif [ $PROJREF = 1 ] #2
      then
      echo M: $i
   elif [ $PROJEXP = 1 ] #3
      then
         if [ $REFEXT = 0 -a $EXPEXT = 1 -a $PROJEXT = 1 ] 
            then #ak neexistuje nic sa nestane
            continue
         fi
      echo UM: $i
      cp $PROJDIR/$i $REVDIR
   elif [ $REFEXP = 1 ] #4
      then
      echo U: $i
      cp $PROJDIR/$i $REVDIR
      cp $PROJDIR/$i $EXPDIR
   elif [ $PROJREF = 0 -a $PROJEXP = 0 -a $REFEXP = 0 ] #5
      then
         #vecina chybnych stavov konci v tejto podm.
         if [ $EXPEXT = 1 -a $PROJEXT = 0 ] 
         #ak existuje iba v expdir, je ignorovany(bez tejtopodmienky sa chybne vyhodnoti #5)
            then
            continue
         elif [ $EXPEXT = 0 -a $PROJEXT = 0 -a $REFEXT = 0 ]
            then
            continue
         fi
      #zmeny v adresari projektu oproti adresaru ref.k. su ulozene v  $EXPDIR/tmp/difflogPREF.patch 
      cd $EXPDIR
      patch -f<$EXPDIR/difflogPREF.patch >/dev/null 2>&1
      if [ $? = 0 ]
         then     
         echo M+: $i
	 cp $PROJDIR/$i $REVDIR #/// skript po patchunti EXPDIRU skopci subor do REVDIRU
      else
         echo M!: $i conflict!
      fi	
   fi
done
rm $EXPDIR/*\.rej >/dev/null 2>&1
rm $EXPDIR/*\.orig >/dev/null 2>&1
rm $EXPDIR/difflogPREF.patch>/dev/null 2>&1
#############IOS>PROJEKT_1##############
#######Made by Martin_Fajčík@FITVUT@2013
########################################
