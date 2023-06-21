#include "rpi-rgb-led-matrix/include/led-matrix.h"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>

#include <iostream>
#include <math.h>
#include <fstream>
#include <sstream>


using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}


std::vector<std::vector<int>> matrix1;
std::vector<std::vector<int>> matrix2;
std::vector<std::vector<int>> matrix3;



//ToDo:
//of het geupdate moet worden. (kijken wanneer bestand voor het laatst is aangepast?)
static void readFile(int n){
  std::string bestands_naam = "Paneel" + std::to_string(n) + ".txt";
  std::ifstream paneel(bestands_naam);
  if(!paneel.is_open()){
    std::cerr << "could not open file" << std::endl;
    return;
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



static void DrawBatteryAnimation (Canvas *canvas){
  while(true){
    for(int i=0; i<48; i++){
      for(int j=0; j<32; j++){  
        canvas->SetPixel(j, i, 100, 0, 0);
        if(interrupt_received){
          return;
        }
        usleep(5000);
      }
    }
  }
}


static void DrawMatrix1 (Canvas *canvas){
int pixelcount = 0;
  for(int i=0; i<32; i++){
    for(int j=0; j<16; j++){
      canvas->SetPixel(i, j, matrix1[pixelcount][0], matrix1[pixelcount][1], matrix1[pixelcount][2]);
      pixelcount++;
    }
  }
}

static void DrawMatrix2 (Canvas *canvas){
int pixelcount = 0;
  for(int i=0; i<32; i++){
    for(int j=0; j<16; j++){
      canvas->SetPixel(i, j+16, matrix2[pixelcount][0], matrix2[pixelcount][1], matrix2[pixelcount][2]);
      pixelcount++;
    }
  }
}

static void DrawMatrix3 (Canvas *canvas){
int pixelcount = 0;
  for(int i=0; i<32; i++){
    for(int j=0; j<16; j++){
      canvas->SetPixel(i, j+32, matrix3[pixelcount][0], matrix3[pixelcount][1], matrix3[pixelcount][2]);
      pixelcount++;
    }
  }
}



int main(int argc, char *argv[]) {
  rgb_matrix::RGBMatrix::Options my_defaults;
  my_defaults.hardware_mapping = "regular";  // or e.g. "adafruit-hat"
  my_defaults.rows = 16;
  my_defaults.cols = 32;
  my_defaults.chain_length = 1;
  my_defaults.parallel = 3;
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

  
  
  readFile(1);
  readFile(2);
  readFile(3);
  DrawMatrix1(canvas);
  DrawMatrix2(canvas);
  DrawMatrix3(canvas);
  sleep(1000);

  // Animation finished. Shut down the RGB matrix.
  canvas->Clear();
  delete canvas;

  return 0;
}
