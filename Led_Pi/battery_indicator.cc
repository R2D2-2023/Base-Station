#include "rpi-rgb-led-matrix/include/led-matrix.h"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>

#include <iostream>
#include <math.h>
#include <fstream>
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <ctime>


using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
  //canvas->Fill(0,0,0);
}


std::vector<std::vector<int>> matrix1;
std::vector<std::vector<int>> matrix2;
std::vector<std::vector<int>> matrix3;

std::vector<std::vector<int>> full_charge = {
{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
{0,0,0},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{255,255,255},{0,0,0},
{0,0,0},{255,255,255},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{255,255,255},{0,0,0},
{0,0,0},{0,0,0},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{255,255,255},{0,0,0},{0,0,0},
{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}
};



//ToDo:
//of het geupdate moet worden. (kijken wanneer bestand voor het laatst is aangepast?)
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


static void DrawMatrix1 (Canvas *canvas){
int pixelcount = 0;
  for(int i=0; i<32; i++){
    for(int j=15; j>=0; j--){
      canvas->SetPixel(i+64, j, matrix1[pixelcount][0], matrix1[pixelcount][1], matrix1[pixelcount][2]);
      pixelcount++;
    }
  }
}

static void DrawMatrix2 (Canvas *canvas){
int pixelcount = 0;
  for(int i=0; i<32; i++){
    for(int j=15; j>=0; j--){
      canvas->SetPixel(i+32, j, matrix2[pixelcount][0], matrix2[pixelcount][1], matrix2[pixelcount][2]);
      pixelcount++;
    }
  }
}

static void DrawMatrix3 (Canvas *canvas){
int pixelcount = 0;
  for(int i=0; i<32; i++){
    for(int j=15; j>=0; j--){
      canvas->SetPixel(i, j, matrix3[pixelcount][0], matrix3[pixelcount][1], matrix3[pixelcount][2]);
      pixelcount++;
    }
  }
}
int offset_x = 0;
static void drawMatrix (Canvas *canvas, std::vector<std::vector<int>> matrix){
	int pixelcount = 0;
	
	for(int x=0; x<32; x++){
		for(int y=0; y<15; y++){
			canvas->SetPixel(x+offset_x, y, matrix[pixelcount][0], matrix[pixelcount][1], matrix[pixelcount][2]);
			pixelcount++;
		}
	}
}



int main(int argc, char *argv[]) {
  rgb_matrix::RGBMatrix::Options my_defaults;
  my_defaults.hardware_mapping = "regular";  // or e.g. "adafruit-hat"
  my_defaults.rows = 16;
  my_defaults.cols = 32;
  my_defaults.chain_length = 4;

  my_defaults.parallel = 1;
  my_defaults.show_refresh_rate = true;
  my_defaults.limit_refresh_rate_hz = 100;
  
  rgb_matrix::RuntimeOptions runtime_defaults;
  runtime_defaults.gpio_slowdown = 0;
  my_defaults.disable_hardware_pulsing = 1;
  my_defaults.scan_mode = 0;

  Canvas *canvas = rgb_matrix::RGBMatrix::CreateFromFlags(&argc, &argv, &my_defaults);
  if (canvas == NULL)
    return 1;
  signal(SIGTERM, InterruptHandler);
  signal(SIGINT, InterruptHandler);
  
  //canvas->Fill(255,255,255);
  //canvas->SetPixel(0,0,255,255,255);
  //sleep(10);
  
  //for(int x=32; x<128; x++){
  //  for(int y=47; y>=16; y--){
   //  canvas->SetPixel(x,y,255,255,0);
   //  usleep(1000);
   //  std::cout << x << " " << y << std::endl;
   // }
  //}

//drawMatrix(canvas, matrix_top);
//sleep(100);


for(int i=0; i<500; i++){
  for(int j=0; j<500; j++){
    canvas->SetPixel(i,j,255,0,255);
  }
}
sleep(1000);
/*
  system("./led_panelen/download_panelinfo.sh &");
  sleep(10);
  
  struct stat buf;
  stat("./led_panelen/Paneel3.txt", &buf);
  auto last_modTime = buf.st_mtime;
  std::time_t current_time = std::time(nullptr);
  auto diff = current_time - last_modTime;
  std::cout << diff<<std::endl;
  
  while (true){
    last_modTime = buf.st_mtime;
    current_time = std::time(nullptr);
    diff = current_time - last_modTime;
    if (diff > 5){
      readFile(1);
      readFile(2);
      readFile(3);
      sleep(1);
      DrawMatrix1(canvas);
      DrawMatrix2(canvas);
      DrawMatrix3(canvas);
      sleep(10);
    }
   
    for (int i = 0; i < 16; i++){
      canvas->SetPixel(i,0,255,255,255);
      sleep(1);
    sleep(10);
    
    }
    //if (interrupt_received){
    //break;}
    
  //}
  }
  */
 //Animation finished. Shut down the RGB matrix.
  canvas->Clear();
  delete canvas;
}