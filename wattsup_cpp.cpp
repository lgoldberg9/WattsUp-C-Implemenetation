#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h> /* Text animation lib */
#include <sys/utsname.h> /* Sys information lib */
#include <ctime.h>
#include <unistd.h>

class WattsUp: public object
{
  
private:
  WattsUp(self, port, interval){
    if (args.sim)
      self.s = open(port, 'r');
    else
      {
        self.s = serial.Serial(port, 115200 );
      }
    self.logfile = None;
    self.interval = interval;
    // initialize lists for keeping data
    self.t = [];
    self.power = [];
    self.potential = [];
    self.current = [];
  }
  
public:
  void mode(self, runmode){
    if (args.sim)
      return; // can't set run mode while in simulation
    ofstream myfile;
    myfile.open(self.s);
    myfile << "#L,W,3,%s,,%d;" , runmode, self.interval; 
    if (runmode == INTERNAL_MODE)
      myfile <<"#O,W,1,%d" , FULLHANDLING ;
    myfile.close();
  }

  void fetch(self){
    if (args.sim)
      return;
    ifstream myfile;
    string sLine = "";
    myfile.open(self.s);
    int counter;
    char* word;
     string fields[100];
    while(!myfile.eof())
      {
        string current_line = getline(myfile, sLine);
        if (current_line[0] == '#' && current_line[1] == 'd')
          {
            counter=0;
            word = strtok(line, ',');
            while(word != NULL){
              fields[counter] = word;
              ++counter;
              word = strtok(NULL, ',');
            }
          }
        float W = std::stof(fields[3])/10;
        float V = std::stof(fields[4])/10;
        float A = std::stof(fields[5])/1000;
        
        printf("time TBP, %lf ,%lf, %lf \n", W,V,A );
      }
  }

  void log(self, logfile= NULL)
  {
    printf("Logging...\n");
    if (!args.sim)
      self.mode(EXTERNAL_MODE);
    if (logfile){
      self.logfile = logfile;
      ofstream o;
      o.open(self.logfile)
        }
    if (args.raw){
      rawfile = 
    
        
    
  
  


