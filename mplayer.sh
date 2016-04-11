#!/bin/bash

do_log() {

 while true
 do
  read -d ';'
  if [ ! -z "$REPLY" ]; then
   track_old=$track_now;
   track_olds=$track_nows;
   track_oldx=$track_nowx;

   track_now=$(echo "$REPLY"| grep "ICY Info"|sed s/ICY\ Info:\ StreamTitle=\'//g|sed s/\'$//g);
   [ "$track_old" = "$track_now" ] || (echo $track_now  > /tmp/streamtitle)

   track_nows=$(echo "$REPLY"| grep "Name"|sed s/Name\//g|sed s/$//g);
   [ "$track_olds" = "$track_nows" ] || (echo $track_nows > /tmp/streamname)

   track_nowx=$(echo "$REPLY"| grep "Bitrate:"|sed s/Bitrate:\//g|sed s/$//g);
   [ "$track_oldx" = "$track_nowx" ] || (echo $track_nowx> /tmp/bitrate)

   iconv -f utf8 -t cp866 /tmp/streamtitle -o /tmp/streamtitle.cp1251_b
   iconv -f utf8 -t cp866 /tmp/streamname -o /tmp/streamname.cp1251
   
   cat /tmp/streamtitle.cp1251_b /tmp/streamname.cp1251 > /tmp/streamtitle.cp1251
   
   cat /tmp/streamtitle.cp1251 > /tmp/title.cp1251

   fi
#   sleep 1;
 done
}


    while true
    do
        echo "           " > /tmp/streamname
        echo "           " > /tmp/streamname.cp1251
        echo "           " > /tmp/streamtitle
        echo "           " > /tmp/streamtitle.cp1251
        echo " " > /tmp/bitrate
        echo " " > /tmp/newchannel.flg
        echo " " > /tmp/title.cp1251

#	killall watchdog.sh
#	/automedia/watchdog.sh &

#        killall mplayer
#         mplayer -slave -af equalizer=5:5:5:5:5:3:2:2:2:2 -af extrastereo=1.5 -af volnorm=1:0.15 -cache "$2" -cache-min "$3" "$1" | do_log
	echo " " > /tmp/play
         mplayer -ao pulse -slave "$1" -cache "$2" -cache-min "$3" | do_log
	rm /tmp/play
	rm /tmp/streamtitle
    done