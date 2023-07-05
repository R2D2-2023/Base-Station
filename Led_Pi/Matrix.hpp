#include "rpi-rgb-led-matrix/include/led-matrix.h"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>

#include <iostream>
#include <math.h>
#include <fstream>
#include <sstream>

class Matrix{
	private:
		int columns;
		int rows;
		rgb_matrix::Canvas * canvas;
		int offset_x;
		
	public:
		Matrix(rgb_matrix::Canvas *canvas, int columns = 0, int rows = 0, int offset_x = 0);
		
		void drawMatrix (std::vector<std::vector<int>> matrix);
		
		void setColumns(int column);
		void setRows(int row);
		void fillMatrix(int r, int g, int b);
		void clearMatrix();
};
