#!/bin/bash
sync(){
echo "Sync XRT on $1[$2] ..." >&2
rsync -e "$2" --delete --exclude-from='excludes.txt' --compress-level=9 -v -r -a -P -L -i --stats ../* $1:~/Gitroot/xrt/ >&2
#ssh $1 $REMOTE_SCRIPT start >&2
}

$(sync petalinux@zeus.arh.pub.ro "ssh -p 50022") #pynq1
#$(sync petalinux@zeus.arh.pub.ro "ssh -p 60022") #pynq2
#$(sync mstoian@athena.arh.pub.ro "ssh -p    22") #athena

