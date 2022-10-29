To run this example :

~~~~~
sudo chmod 666 /dev/dummydriver             // give access to dummydriver
echo “This is a test ” > /dev/dummydriver   // write to dummy driver

sudo tail -f /var/log/syslog
head -n 1 /dev/dummydriver
~~~~~

