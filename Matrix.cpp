#include "Matrix.hpp"

Matrix::Matrix(rgb_matrix::Canvas *canvas, int columns, int rows, int offset_x):
	columns(columns),
	rows(rows),
	canvas(canvas),
	offset_x(offset_x)
{};

void Matrix::drawMatrix (std::vector<std::vector<int>> matrix){
	int pixelcount = 0;
	
	for(int x=0; x<rows; x++){
		for(int y=0; y<columns; y++){
			canvas->SetPixel(x+offset_x, y, matrix[pixelcount][0], matrix[pixelcount][1], matrix[pixelcount][2]);
			pixelcount++;
		}
	}
};

void Matrix::setColumns(int columns){
	columns = columns;
};

void Matrix::setRows(int row){
	row = row;
};

void Matrix::fillMatrix(int r, int g, int b){
	for(int x=0; x<rows; x++){
		for(int y=0; y<columns; y++){
			canvas->SetPixel(x+offset_x, y, r, g, b);
		}
	}
};

void Matrix::clearMatrix(){
	for(int x=0; x<rows; x++){
		for(int y=0; y<columns; y++){
			canvas->SetPixel(x+offset_x, y, 0, 0, 0);
		}
	}
};
