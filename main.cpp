#include "rpi-rgb-led-matrix/include/led-matrix.h"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>

#include <iostream>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <ctime>

#include <wiringSerial.h>

#include "Matrix.hpp"
#include "TopMatrix.hpp"
#include "SideMatrix.hpp"


using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

std::vector<std::vector<int>> matrix1;
std::vector<std::vector<int>> matrix2;
std::vector<std::vector<int>> matrix3;

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}

long map(long x, long in_min, long in_max, long out_min, long out_max){
	return(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

std::string readFromDB(){
	system("/usr/bin/python /home/pi/basedstation/dataBase.py");
	sleep(1);
	std::string bestands_naam = "voltage.txt";
	std::ifstream voltage(bestands_naam);
	if(!voltage.is_open()){
		return "could not open file";
	}
	std::string line;
	std::getline(voltage, line);
	return line;
}


std::vector<std::vector<int>> makeBatteryIcon(Canvas *canvas, int battery_stage){
		
	int red = round(map(battery_stage, 1, 5, 255, 0));
	int green = round(map(battery_stage, 1, 5, 0, 255));
	int blue = 0;
	std::vector<std::vector<int>> matrix = {
{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {255,255,255}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, 
{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}
};

switch(1){
	case 1:
		for(int y=24; y<28; y++){
			for(int x=4; x<12; x++){
				matrix[y*16+x][0] = red;
				matrix[y*16+x][1] = green;
				matrix[y*16+x][2] = blue;
			}
		}
		if(battery_stage == 1){break;}
	case 2:
		for(int y=19; y<23; y++){
			for(int x=4; x<12; x++){
				matrix[y*16+x][0] = red;
				matrix[y*16+x][1] = green;
				matrix[y*16+x][2] = blue;
			}
		}
		if(battery_stage == 2){break;}
	case 3:
		for(int y=14; y<18; y++){
			for(int x=4; x<12; x++){
				matrix[y*16+x][0] = red;
				matrix[y*16+x][1] = green;
				matrix[y*16+x][2] = blue;
			}
		}
		if(battery_stage == 3){break;}
	case 4:
		for(int y=9; y<13; y++){
			for(int x=4; x<12; x++){
				matrix[y*16+x][0] = red;
				matrix[y*16+x][1] = green;
				matrix[y*16+x][2] = blue;
			}
		}
		if(battery_stage == 4){break;}
	case 5:
		for(int y=4; y<=5; y++){
			for(int x=6; x<=9; x++){
				matrix[y*16+x][0] = red;
				matrix[y*16+x][1] = green;
				matrix[y*16+x][2] = blue;
			}
		}
		if(battery_stage == 5){break;}
}
return matrix;
}

static void readFile(int n){
  std::string bestands_naam = "./led_panelen/Paneel" + std::to_string(n) + ".txt";
  std::ifstream paneel(bestands_naam);
  if(!paneel.is_open()){
    std::cerr << "could not open file" << std::endl;
    return;
  }
      
    if(n == 1){
      matrix1.clear();
    }
    else if(n ==2){
      matrix2.clear();
    }
    else if(n ==3){
      matrix3.clear();
    }

  std::vector<int> matrix;
  std::string line;
  while(getline(paneel, line)){
    matrix.clear();
    std::stringstream ss(line);
    std::string token;
    while(std::getline(ss, token, ',')){
      matrix.push_back(std::stoi(token));
    }
    if(n == 1){
      matrix1.push_back(matrix);
    }
    else if(n ==2){
      matrix2.push_back(matrix);
    }
    else if(n ==3){
      matrix3.push_back(matrix);
    }
  }
}

int main(int argc, char *argv[]) {
	/*
	while(true){
	serialOpen("/dev/ttyUSB0", 115200);
	serialPuts(4, "k");
	serialClose(4);
	
	std::cout << "serial shit done" << std::endl;
	}
	*/
  rgb_matrix::RGBMatrix::Options my_defaults;
  my_defaults.hardware_mapping = "regular";  // or e.g. "adafruit-hat"
  my_defaults.rows = 16;
  my_defaults.cols = 32;
  my_defaults.chain_length = 4;

  my_defaults.parallel = 1;
  my_defaults.show_refresh_rate = true;
  my_defaults.limit_refresh_rate_hz = 100;
  
  rgb_matrix::RuntimeOptions runtime_defaults;
  runtime_defaults.gpio_slowdown = 4;
  my_defaults.disable_hardware_pulsing = 1;
  my_defaults.scan_mode = 0;

  Canvas *canvas = rgb_matrix::RGBMatrix::CreateFromFlags(&argc, &argv, &my_defaults);
  if (canvas == NULL)
    return 1;
  signal(SIGTERM, InterruptHandler);
  signal(SIGINT, InterruptHandler);
  
  
  
  if(interrupt_received){
	  return 0;
  }
	
  //HIER CLASSES AANROEPEN
	Matrix matrix_top(canvas, 16, 32);
	Matrix matrix_side_right(canvas, 16, 32, 32);
	Matrix matrix_side_middle(canvas, 16, 32, 64);
	Matrix matrix_side_left(canvas, 16, 32, 96);
	
	readFile(1);
    readFile(2);
    readFile(3);
    sleep(1);
    matrix_side_right.drawMatrix(matrix1);
	matrix_side_middle.drawMatrix(matrix2);
	matrix_side_left.drawMatrix(matrix3);
	
	int voltage = std::stoi(readFromDB());
	int battery_stage = map(voltage, 10.8, 12.6, 1, 5);
	std::vector<std::vector<int>> matrix = {};
	while(true){
	if(interrupt_received){
	  return 0;
  }
	for(int i=1; i<=5; i++){
		matrix.clear();
		matrix = makeBatteryIcon(canvas, i);
		matrix_top.drawMatrix(matrix);
		sleep(1);
		
	}}

	sleep(1000);
	

 //Animation finished. Shut down the RGB matrix.
  canvas->Clear();
  delete canvas;
}
