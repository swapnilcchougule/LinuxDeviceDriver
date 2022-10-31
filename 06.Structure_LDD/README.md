To run this example :

~~~~~
Video : https://www.youtube.com/watch?v=pIUTaMKq0Xc&list=PLbzoR-pLrL6q8QMGJ4dFnqejkHDm76kJV&index=2&ab_channel=TheLinuxFoundatio
Presentation link :  https://elinux.org/ELC_2021_Presentations

cat /proc/devices       	     // list the major number and driver
sudo mknod -m 666 /dev/led c 510 0  // create a char device node  
ls -l /dev/led               	     // list the led driver
echo 1 >/dev/led 		     // write 0 led driver file 
echo 0 >/dev/led 	             // write 0 led driver file 

cat /proc/iomem


ls /sys/class/leds/         // list the leds
~~~~~

