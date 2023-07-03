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
#include <chrono>

#include <wiringSerial.h>

#include "Matrix.hpp"

//ToDo:
//iets met de map functie klopt niet
//soms kunnen de files niet gevonden worden

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}

//simple map function to get variable in 1 range to other range.
long map(float x, float in_min, float in_max, float out_min, float out_max){
	return(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//get the voltage of the robot fromt he database.
std::string readFromDB(){
	system("/usr/bin/python /home/pi/basedstation/dataBase.py");
	usleep(200000);	//wait for python script to be done.
	std::string bestands_naam = "voltage.txt";
	std::ifstream voltage(bestands_naam);
	if(!voltage.is_open()){
		return "could not open file";
	}
	std::string line;
	std::getline(voltage, line);
	return line;
}


std::vector<std::vector<int>> makeBatteryIcon(int battery_stage){
	//calculate rgb values based of battery_stage
	int red = round(map(battery_stage, 1, 5, 255, 0));
	int green = round(map(battery_stage, 1, 5, 0, 255));
	int blue = 0;
	
	if(battery_stage < 1){
		red = 255;
		green = 0;
	}
	else if(battery_stage > 5){
		red = 0;
		green = 255;
	}
	
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

std::vector<std::vector<int>> readFile(int n){
	std::string bestands_naam = "./led_panelen/Paneel" + std::to_string(n) + ".txt";
	std::ifstream paneel(bestands_naam);
	if(!paneel.is_open()){
		throw (n);
	}
	std::vector<std::vector<int>> tmp_matrix;
	std::vector<int> tmp_pixel;
	std::string line;
	while(getline(paneel, line)){
		tmp_pixel.clear();
		std::stringstream ss(line);
		std::string token;
		while(std::getline(ss, token, ',')){
			tmp_pixel.push_back(std::stoi(token));
		}
		tmp_matrix.push_back(tmp_pixel);
	}
	return tmp_matrix;
}

int main(int argc, char *argv[]) {
	//remember to also start download_panelinfo.sh along side this program to get the updated pixelarts.
	//Initialize the library for the led matrices.
	rgb_matrix::RGBMatrix::Options my_defaults;
	my_defaults.hardware_mapping = "regular";
	my_defaults.rows = 16;
	my_defaults.cols = 32;
	my_defaults.chain_length = 4;
	my_defaults.parallel = 1;
	my_defaults.show_refresh_rate = false;
	my_defaults.limit_refresh_rate_hz = 100;
	rgb_matrix::RuntimeOptions runtime_defaults;
	runtime_defaults.gpio_slowdown = 4;
	my_defaults.disable_hardware_pulsing = 1;
	my_defaults.scan_mode = 0;

	rgb_matrix::Canvas *canvas = rgb_matrix::RGBMatrix::CreateFromFlags(&argc, &argv, &my_defaults);
	if (canvas == NULL)
		return 1;
	signal(SIGTERM, InterruptHandler);
	signal(SIGINT, InterruptHandler);
  
	
	//construct classes so we can easily draw to the matrices.
	Matrix matrix_top(canvas, 16, 32);
	Matrix matrix_side_right(canvas, 16, 32, 32);
	Matrix matrix_side_middle(canvas, 16, 32, 64);
	Matrix matrix_side_left(canvas, 16, 32, 96);
	
	int battery_stage = map(std::stof(readFromDB()), 10.8, 12.6, 1, 5);
	auto start_time = std::chrono::steady_clock::now() - std::chrono::seconds(60);
	
	while(true){
		if(interrupt_received){
			break;
		}
		
		auto current_time = std::chrono::steady_clock::now();
		auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
		if(elapsed_time >= 60){
			start_time = current_time;
			
			try{
				matrix_side_right.drawMatrix(readFile(1));
				matrix_side_middle.drawMatrix(readFile(2));		
				matrix_side_left.drawMatrix(readFile(3));
			}
			catch(int n){
				std::cerr << "could not open file" << n << std::endl;
			}
			
			
			battery_stage = map(std::stof(readFromDB()), 10.8, 12.6, 1, 5);
			
			if(battery_stage > 0 && battery_stage < 6){
				matrix_top.drawMatrix(makeBatteryIcon(battery_stage));
			}
		}
		if(battery_stage < 1 || battery_stage > 5){
			matrix_top.drawMatrix(makeBatteryIcon(battery_stage));
			usleep(200000);
			matrix_top.clearMatrix();
			usleep(200000);
		}
	}	
	//Animation finished. Shut down the RGB matrix.
	canvas->Clear();
	delete canvas;
}
