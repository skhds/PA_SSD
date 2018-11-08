#!/bin/sh
trap "/bin/rm -f /home/skhds/PAPER_MASTER/SSD_PA/v0.2/software_cache/.clean_up.sh" 0
sleep 5
exec $COWAREHOME/common/bin/process_cleanup 17668 > /dev/null 2>&1
