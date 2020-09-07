# Simple build for the AjaxDemoclientserver to lookup Shakespeare

USER= skon

all:	indexdemo sslookupclient sslookupserver  PutCGI PutHTML testclient

textindex.o:	textindex.cpp textindex.h english_stem.h narrow_wide_string.h string_util.h utilities.h meta.h stemming.h	
		g++ -c textindex.cpp

indexdemo.o:    indexdemo.cpp textindex.h
		g++ -c indexdemo.cpp 

indexdemo:	textindex.o indexdemo.o
		g++ -o indexdemo indexdemo.o textindex.o

testclient.o: 	testclient.cpp fifo.h
		g++ -c testclient.cpp

testclient:	testclient.o fifo.o
		g++ -o 	testclient testclient.o fifo.o

sslookupclient.o: 	sslookupclient.cpp fifo.h
		g++ -c sslookupclient.cpp

sslookupserver.o:	sslookupserver.cpp fifo.h
		g++ -c sslookupserver.cpp 

sslookupclient:	sslookupclient.o fifo.o
		g++ -o 	sslookupclient sslookupclient.o fifo.o -L/usr/local/lib -lcgicc

sslookupserver: sslookupserver.o textindex.o fifo.o
		g++ -o sslookupserver sslookupserver.o textindex.o fifo.o

fifo.o:		fifo.cpp fifo.h
		g++ -c fifo.cpp

PutCGI: sslookupclient
		chmod 757 sslookupclient
		cp sslookupclient /usr/lib/cgi-bin

PutHTML: sslookup.html
		cp sslookup.html  /var/www/html/class/softdev/skon/shake/
		cp layout.css /var/www/html/class/softdev/skon/shake/
		echo "Current contents of your HTML directory: "
		ls -l  /var/www/html/class/softdev/skon/shake/

clean:
	rm *.o *# *~ sslookupclient sslookupserver testclient indexdemo


