#include "/home/pi/rpi-rgb-led-matrix/include/led-matrix.h"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>
#include <mqtt/async_client.h>

#include <iostream>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <ctime>
#include <chrono>


#include "MQTTClient.h"
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <thread>
#include "mqtt/async_client.h"

#include "Matrix.hpp"


const std::string SERVER_ADDRESS("tcp://145.89.224.225:1883");
const std::string TOPIC("BasedStation");

const int       QOS = 1;
const int       N_RETRY_ATTEMPTS = 5;
bool charging = false;

/////////////////////////////////////////////////////////////////////////////

// Callbacks for the success or failures of requested actions.
// This could be used to initiate further action, but here we just log the
// results to the console.
class action_listener : public virtual mqtt::iaction_listener
{
        std::string name_;

        void on_failure(const mqtt::token& tok) override {
                std::cout << name_ << " failure";
                if (tok.get_message_id() != 0)
                        std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
                std::cout << std::endl;
        }

        void on_success(const mqtt::token& tok) override {
                std::cout << name_ << " success";
                if (tok.get_message_id() != 0)
                        std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
                auto top = tok.get_topics();
                if (top && !top->empty())
                        std::cout << "\ttoken topic: '" << (*top)[0] << "', ..." << std::endl;
                std::cout << std::endl;
        }

public:
        action_listener(const std::string& name) : name_(name) {}
};

/////////////////////////////////////////////////////////////////////////////

/**
 * Local callback & listener class for use with the client connection.
 * This is primarily intended to receive messages, but it will also monitor
 * the connection to the broker. If the connection is lost, it will attempt
 * to restore the connection and re-subscribe to the topic.
 */
class callback : public virtual mqtt::callback,
                                        public virtual mqtt::iaction_listener

{
        // Counter for the number of connection retries
        int nretry_;
        // The MQTT client
        mqtt::async_client& cli_;
        // An action listener to display the result of actions.
        action_listener subListener_;

        // This deomonstrates manually reconnecting to the broker by calling
        // connect() again. This is a possibility for an application that keeps
        // a copy of it's original connect_options, or if the app wants to
        // reconnect with different options.
        // Another way this can be done manually, if using the same options, is
        // to just call the async_client::reconnect() method.
        void reconnect() {
                std::this_thread::sleep_for(std::chrono::milliseconds(2500));
                try {
                        cli_.connect(nullptr, *this);
                }
                catch (const mqtt::exception& exc) {
                        std::cerr << "Error: " << exc.what() << std::endl;
                        exit(1);
                }
        }

        // Re-connection failure
        void on_failure(const mqtt::token& tok) override {
                std::cout << "Connection attempt failed" << std::endl;
                if (++nretry_ > N_RETRY_ATTEMPTS)
                        exit(1);
                reconnect();
        }

        // (Re)connection success
        // Either this or connected() can be used for callbacks.
        void on_success(const mqtt::token& tok) override {}

        // (Re)connection success
        void connected(const std::string& cause) override {
                std::cout << "\nConnection success" << std::endl;
                std::cout << "\nSubscribing to topic '" << TOPIC << "'\n"
                        << " using QoS" << QOS << "\n"
                        << "\nPress Q<Enter> to quit\n" << std::endl;

                cli_.subscribe(TOPIC, QOS, nullptr, subListener_);
        }

        // Callback for when the connection is lost.
        // This will initiate the attempt to manually reconnect.
        void connection_lost(const std::string& cause) override {
                std::cout << "\nConnection lost" << std::endl;
                if (!cause.empty())
                        std::cout << "\tcause: " << cause << std::endl;

                std::cout << "Reconnecting..." << std::endl;
                nretry_ = 0;
                reconnect();
        }
		
        // Callback for when a message arrives.
        void message_arrived(mqtt::const_message_ptr msg) override {
                std::cout << "Message arrived" << std::endl;
                std::cout << "\ttopic: '" << msg->get_topic() << "'" << std::endl;
                std::cout << "\tpayload: '" << msg->to_string() << "'\n" << std::endl;
                std::cout << "test 3" << std::endl;
        std::string status = msg->to_string();
        if(status == "charging"){
            charging = true;
        }
        else if(status == "done"){
            charging = false;
        }
        }

        void delivery_complete(mqtt::delivery_token_ptr token) override {}

public:
        callback(mqtt::async_client& cli)
                                : nretry_(0), cli_(cli), subListener_("Subscription") {}
};

/////////////////////////////////////////////////////////////////////////////


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
	system("/usr/bin/python3.9 /home/pi/Base-Station/dataBase.py");
	usleep(200000); //wait for python script to be done.
	std::string bestands_naam = "/home/pi/Base-Station/voltage.txt";
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
    std::string bestands_naam = "/home/pi/Base-Station/led_panelen/paneel" + std::to_string(n) + ".txt";
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
    std::cout << "map test result is: " << round(map(5, 1, 5, 255, 0)) << std::endl;

    //remember to also start download_panelinfo.sh along side this program to get the updated pixelarts.
    //Initialize the library for the led matrices.


    rgb_matrix::RGBMatrix::Options my_defaults;
    my_defaults.hardware_mapping = "regular";
    my_defaults.rows = 16;
    my_defaults.cols = 32;
    my_defaults.chain_length = 4;
    my_defaults.parallel = 1;
    my_defaults.show_refresh_rate = true;
    my_defaults.limit_refresh_rate_hz = 100;
    my_defaults.brightness=50;
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

    //system("./download_panelinfo.sh"); //ditwerkt dus niet
    int battery_stage = map(std::stof(readFromDB()), 10.8, 12.6, 1, 5);
    auto start_time = std::chrono::steady_clock::now() - std::chrono::seconds(60);
    std::string status = "not_charging";

    mqtt::async_client cli(SERVER_ADDRESS, "");

    // Install the callback(s) before connecting.
    callback cb(cli);
    cli.set_callback(cb);

    // Start the connection.
    // When completed, the callback will subscribe to topic.
    try {
        std::cout << "Connecting to the MQTT server..." << std::flush;
        cli.connect(nullptr, cb);
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "\nERROR: Unable to connect to MQTT server: '"
        << SERVER_ADDRESS << "'" << exc << std::endl;
        return 1;
    }
	while(true){
        if(interrupt_received){
                break;
        }

        if(charging == true){
            for(int i=1; i<6; i++){
                matrix_top.drawMatrix(makeBatteryIcon(i));
                usleep(200000);
            }
        }
        else if(battery_stage < 1 || battery_stage > 5){
            matrix_top.drawMatrix(makeBatteryIcon(battery_stage));
            usleep(200000);
            matrix_top.clearMatrix();
            usleep(200000);
        }
        else if(battery_stage > 0 && battery_stage < 6){
            matrix_top.drawMatrix(makeBatteryIcon(battery_stage));
        }

        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
        if(elapsed_time >= 30){
            std::cout << "elke 30 seconde" << std::endl;
            start_time = current_time;

            try{
                matrix_side_right.drawMatrix(readFile(1));
                matrix_side_middle.drawMatrix(readFile(2));
                matrix_side_left.drawMatrix(readFile(3));
            }
            catch(int n){
                std::cerr << "could not open file" << n << std::endl;
            }
            std::cout << "test2" << std::endl;
            battery_stage = map(std::stof(readFromDB()), 10.8, 12.6, 1, 5);
        }
    }
    //Animation finished. Shut down the RGB matrix.
    canvas->Clear();
    // Disconnect
    try {
        std::cout << "\nDisconnecting from the MQTT server..." << std::flush;
        cli.disconnect()->wait();
        std::cout << "OK" << std::endl;
    }
    catch (const mqtt::exception& exc) {
        std::cerr << exc << std::endl;
         return 1;
    }
    delete canvas;
}