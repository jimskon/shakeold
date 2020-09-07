
#include <iostream>
#include <fstream>
#include <string>

// Stuff for Ajax
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

//Stuff for pipes                                                               
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "fifo.h"

#define logging

using namespace std;
using namespace cgicc;

// Pipes for communication                                                      
string receive_pipe = "SSreply";
string send_pipe = "SSrequest";

// Logfile
#ifdef logging
const char* LogFile =  "/tmp/skonsslookup.log";
ofstream logFile;
string timestamp() {
  time_t _tm =time(NULL );
  struct tm * curtime = localtime ( &_tm );
  string t = asctime(curtime);
  t.erase(t.end() - 1);
  return t;
}
#define log(message) logFile << timestamp() << ": " << message << endl
#else
#define log(message)
#endif

int main() {
  Cgicc cgi;
  form_iterator sstring = cgi.getElement("sstring");
  string searchString = **sstring;
  int length = searchString.length();

  Fifo recfifo(receive_pipe);
  Fifo sendfifo(send_pipe);

#ifdef logging
  logFile.open(LogFile,ios::out);
#endif


  sendfifo.openwrite();
  log("Open Request Pipe");

  // Call server to get results                                                 
  sendfifo.send(searchString);
  log("Request: "+searchString);
  
  // prepare the output
  cout << "Content-Type: text/plain\n\n" << endl;

  recfifo.openread();
  log("open reply fifo");

  // output the response to the web page
  string results = "";
  int times = 0; // Counter for header lines
  while (results != "$end") {
    results = recfifo.recv();
    log("Reply: "+results);
    if (results != "$end") {
      cout << results << endl;
      if (times++ > 2) {
	cout << "<br>";
      }
    }
  }
  recfifo.fifoclose();
  log("close reply fifo");
  sendfifo.fifoclose();
  log("close request fifo");
  return 0;
}
