#include "ArrayList.h"
#include "Utilities.h"
//#include "WattsUp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/utsname.h> /* Sys information lib */
#include <unistd.h>
#include <iostream>
#include <curses.h> /* for ncurses */
#include <fstream> 

//To replace std:: in front of every function
using namespace std;


/*
meter_log logs all input from the WattsUp Device onto a given logfile (as determined by the argument,  outfile_t logfile)
It also displays the information it is login onscreen for the uses, using ncurses.
Right now it has basic functionality, and does not account for all the flags that were present in the python file.
*/
void meter_log(outfile_t logfile)
{
	printf("Logging...\n");

  //Opening logfile
	std::ofstream writefile;
	writefile.open (logfile.outfile_path);

	//Opening serial port to reeive data from WattsUp
	std::ifstream readfile;
	readfile.open("/dev/ttyUSB0");

	//Get current line
	string current_line;
	getline (readfile, current_line);
	char* line = new char[current_line.length() + 1];
	strcpy(line, current_line.c_str());
	int n=0;

  //Initialize screen and ncurses
	WINDOW* mainwin = initscr();
	noecho();
	timeout(0);
	refresh();

	//Misc. Variables
	unsigned int microseconds = 1000000;
	char* fields[100];
	int counter=0;
	char* word;

  //graph drawing would go here if necessary

	/*Infinite while loop that 
	      sleeps for microseconds (see above variable)
	      Checks to see if string gotten from Wattsup has data (happens if line starts with '#d')
	      delimits string gotten from WattsUp by commas and parses data
	      logs this data and prints it to user
	      gets next string

      This goes on until the user quits by pressing the 'q' key
	*/
      while (true)
      {
      	usleep(microseconds);

      	if (line[0] == '#' && line[1] == 'd')
      	{
      		//Delimit by ',' using strtok
      		counter=0;
      		word = strtok(line, ",");
      		while(word != NULL){
      			fields[counter] = word;
      			++counter;
      			word = strtok(NULL, ",");
      		}

      		//if there are appropriate number of strings in the delimitations, get data
      		if (counter>5)
      		{
      			/*
      			float W; //power
      			sscanf(fields[3], "%f", &W);
      			W = W/10.0;
      			float V; //voltage
      			sscanf(fields[4], "%f", &V);
      			W = V/10.0;
      			float A; //amps
      			sscanf(fields[5], "%f", &A);
      			W = A/1000.0;
      			*/
#include "ArrayList.h"

      			double W, V, A;
      			W = atof(fields[3]) / 10.0;
      			V = atof(fields[4]) / 10.0;
      			A = atof(fields[5]) / 1000.0;

      			//Print data onto screen
      			mvprintw(10, 10, "Logging to file ");
      			mvprintw(11, 10, "Time : ", n);
      			mvprintw(12, 10, "Power %lf", W);
      			mvprintw(13, 10, "Voltage %lf", V);
      			if (A<1000)
      				mvprintw(14, 10, "milli Amps %lf", (A*1000));
      			else
      				mvprintw(14, 10, "milli Amps %lf", A);
      			mvprintw(15, 10, "Press q to Quit.");

      			//If user presses 'q', quit while loop
      			int key = getch();
      			if (key == 'q'){
      				break;
      			}

      			//Write data to log file
      			writefile << "\t W = "<< W;
      			writefile << "\t V = "<< V;
      			writefile << "\t A = "<< A;
      			writefile << "\n";

      			//Increase counter to know which second we are at
      			n++;

      			refresh();
      		}
      	}
      	//Get next line
      	getline (readfile, current_line);
      	strcpy(line, current_line.c_str());
      }//while loop
      endwin(); //close ncurses
  }//meter_log

//Initialize all options of the array list, initially to be false
  void initialize_options(flags *options) {
  	options->verbose = false;
  	options->debug = false;
  	options->simulation_mode = false;
  	options->internal_mode = false;
  	options->fetch = false;
  	options->graphics_mode = false;
  	options->logging = false;
  	options->raw = false;
  	options->outfile.outfile_b = false;
  	options->outfile.outfile_path = (char*)"log.out";
  	options->interval.active_interval = false;
  	options->interval.interval_d = 1;
  	options->port.port_b = false;
  } //initialize_options

  void option_descr(void) {
  	fprintf(stdout, "usage: wattsup [-h] [-v] [-d] [-m] [-i] [-f] [-g] [-l] [-r] ");
  	fprintf(stdout, "[-o OUTFILE]\n\t\t[-s INTERVAL] [-p PORT]\n\n");
  	fprintf(stdout, "Get data from Watts Up power meter.\n\n");
  	fprintf(stdout, "optional arguments:\n");
  	fprintf(stdout, "  -h,\t\t show this help message and exit\n");
  	fprintf(stdout, "  -v,\t\t verbose\n");
  	fprintf(stdout, "  -d,\t\t debugging output\n");
  	fprintf(stdout, "  -m,\t\t simulate logging by reading serial data from disk ");
  	fprintf(stdout, " with\n\t\t\t delay of sample interval between lines\n");
  	fprintf(stdout, "  -i,\t\t Set meter to internal logging mode\n");
  	fprintf(stdout, "  -f,\t\t Fetch data stored on the meter (NOT YET WORKING!)\n");
  	fprintf(stdout, "  -g,\t\t Graphical output: plot the data in real time\n");
  	fprintf(stdout, "  -l,\t\t log data in real time\n");
  	fprintf(stdout, "  -r,\t\t output raw file\n");
  	fprintf(stdout, "  -o OUTFILE,\t Output file\n");
  	fprintf(stdout, "  -s INTERVAL,\t Sample interval\n");
  	fprintf(stdout, "  -p PORT,\t USB serial port\n");
  }//option_descr

//Sets appropriate options as per flags provided by user
  void parse_inputs(flags *options, int argc, char **argv) {

  	for (int index = 1; index < argc; index++) {
  		switch (argv[index][1]) {
  			case 'v':
  			options->verbose = true;
  			break;
  			case 'd':
  			options->debug = true;
  			break;
  			case 'm':
  			options->simulation_mode = true;
  			break;
  			case 'i':
  			options->internal_mode = true;
  			break;
  			case 'f':
  			options->fetch = true;
  			break;
  			case 'g':
  			options->graphics_mode = true;
  			break;
  			case 'l':
  			options->logging = true;
  			break;
  			case 'r':
  			options->raw = true;
  			break;
  			case 'o':
  			options->outfile.outfile_b = true;
  			if (index++ < argc) {
  				options->outfile.outfile_path = (char*) malloc(sizeof(argv[index]));
  				options->outfile.outfile_path = argv[index];
  			} else {
  				fprintf(stderr, "Not enough inputs provided.\n");
  			}
  			break;
  			case 's':
  			options->interval.active_interval = true;
  			if (index++ < argc) {
  				options->interval.interval_d = atoi(argv[index]);
  			} else {
  				fprintf(stderr, "Not enough inputs provided.\n");
  			}
  			break;
  			case 'p':
  			options->port.port_b = true;
  			if (index++ < argc) {
  				options->port.portDest = argv[index];
  			} else {
  				fprintf(stderr, "Not enough inputs provided.\n");
  			}
  			break;
  			case 'h':
  			option_descr();
  			exit(EXIT_SUCCESS);
  			default:
  			fprintf(stdout, "Incorrect option provided!\n");
  			exit(EXIT_FAILURE);
  		}//switch
  	}//for loop
  }//parse_inputs

  int main(int argc, char **argv) {

  	flags *options;
  	struct utsname* buf;

  	if ((options = (flags*) malloc(sizeof(flags))) == NULL) {
  		fprintf(stderr, "Failed to allocate memory for flags.\n");
  		return EXIT_FAILURE;
  	}

  	initialize_options(options);
  	parse_inputs(options, argc, argv);

  	if (!options->port.port_b) {
  		options->port.port_b = true;
  		if ((buf = (struct utsname*) malloc(sizeof(struct utsname))) == NULL) {
  			fprintf(stderr, "Failed to allocate memory for UNAME.\n");
  			return EXIT_FAILURE;
  		}

  		uname(buf);
  		char *system = (char*) buf->sysname;

  		options->port.portDest = (char*) malloc(sizeof(char) * MAX_STRING_LEN);
    if (strcmp(system, "Darwin") == 0) { /* Max OS X */
  		strcpy(options->port.portDest, "/dev/tty.usb-serial-A1000wT3");
  	} else if (strcmp(system, "Linux") == 0) {
  		strcpy(options->port.portDest, "/dev/ttyUSB0");
  	}
  	free(buf);
  }
  if (access(options->port.portDest, F_OK) == -1) {
  	if (!options->simulation_mode) {
  		fprintf(stdout, "\nSerial port %s does not exist.\n", options->port.portDest);
  		fprintf(stdout, "Please make sure FDTI drivers are installed\n");
  		fprintf(stdout, " (http://www.ftdichip.com/Drivers/VCP.htm)\n");
  		fprintf(stdout, "Default ports are /dev/ttyUSB0 for Linux\n");
  		fprintf(stdout, " and /dev/tty.usbserial-A1000wT3 for Mac OS X\n");
  	} else {
  		fprintf(stdout, "\nFile %s does not exist.", options->port.portDest);
  	}
  }

  if (options->logging)
  	meter_log(options->outfile);



//Logans Code
  /*
  WattsUp *meter = initialize_wattsup(options->simulation_mode,
  	options->port.portDest,
  	options->interval.interval_d);
  if (options->logging) {
  	logging(meter, options->simulation_mode, options->raw,
  		options->outfile.outfile_path);
  }
  /*
  if (options->fetch) {
  	fetch(meter, options->simulation_mode);
  }
  if (options->internal_mode) {
  	mode(meter, options->simulation_mode, INTERNAL_MODE);
  }
*/


  return 0;
}//main