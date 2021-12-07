//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include "Image.h"

// Constructor and Desctructors
MyImage::MyImage() {
	Data = NULL;
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
}

MyImage::~MyImage() {
	if (Data)
		delete[] Data;
}

// Copy constructor
MyImage::MyImage(MyImage* otherImage) {
	Height = otherImage->Height;
	Width = otherImage->Width;
	Data = new char[Width * Height * 4];
	strcpy(otherImage->ImagePath, ImagePath);

	for (int i = 0; i < (Height * Width * 4); i++) {
		Data[i] = otherImage->Data[i];
	}
}

// = operator overload
MyImage& MyImage::operator= (const MyImage& otherImage) {
	Height = otherImage.Height;
	Width = otherImage.Width;
	Data = new char[Width * Height * 4];
	strcpy(ImagePath, (char*)otherImage.ImagePath);

	for (int i = 0; i < (Height * Width * 4); i++) {
		Data[i] = otherImage.Data[i];
	}

	return *this;
}

// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage() {

	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0) {
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}

	// Create a valid output file pointer
	FILE* IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if (IN_FILE == NULL) {
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char* Rbuf = new char[Height * Width];
	char* Gbuf = new char[Height * Width];
	char* Bbuf = new char[Height * Width];

	for (i = 0; i < Width * Height; i++) {
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width * Height; i++) {
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width * Height; i++) {
		Bbuf[i] = fgetc(IN_FILE);
	}

	// Allocate Data structure and copy
	Data = new char[Width * Height * 4];
	for (i = 0; i < Height * Width; i++) {
		Data[4 * i] = Bbuf[i];
		Data[4 * i + 1] = Gbuf[i];
		Data[4 * i + 2] = Rbuf[i];
		Data[4 * i + 3] = 255;
	}

	// Clean up and return
	delete[] Rbuf;
	delete[] Gbuf;
	delete[] Bbuf;
	fclose(IN_FILE);

	return true;

}

// MyImage functions defined here
bool MyImage::WriteImage() {
	// Verify ImagePath
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0) {
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}

	// Create a valid output file pointer
	FILE* OUT_FILE;
	OUT_FILE = fopen(ImagePath, "wb");
	if (OUT_FILE == NULL) {
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char* Rbuf = new char[Height * Width];
	char* Gbuf = new char[Height * Width];
	char* Bbuf = new char[Height * Width];

	for (i = 0; i < Height * Width; i++) {
		Bbuf[i] = Data[3 * i];
		Gbuf[i] = Data[3 * i + 1];
		Rbuf[i] = Data[3 * i + 2];
	}


	// Write data to file
	for (i = 0; i < Width * Height; i++) {
		fputc(Rbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width * Height; i++) {
		fputc(Gbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width * Height; i++) {
		fputc(Bbuf[i], OUT_FILE);
	}

	// Clean up and return
	delete[] Rbuf;
	delete[] Gbuf;
	delete[] Bbuf;
	fclose(OUT_FILE);

	return true;

}

bool MyImage::TargetArea(int x1, int y1, int x2, int y2, int b, int g, int r) {
	if (x1 > x2)
		std::swap(x1, x2);
	if (y1 > y2)
		std::swap(y1, y2);
	for (int i = x1; i <= x2; i++) {
		for (int j = -2; j < 3; j++) {
			if (y1 + j >= 0 && y1 + j < Height) {
				Data[4 * ((y1 + j) * Width + i)] = b;
				Data[4 * ((y1 + j) * Width + i) + 1] = g;
				Data[4 * ((y1 + j) * Width + i) + 2] = r;
			}
			if (y2 + j >= 0 && y2 + j < Height) {
				Data[4 * ((y2 + j) * Width + i)] = b;
				Data[4 * ((y2 + j) * Width + i) + 1] = g;
				Data[4 * ((y2 + j) * Width + i) + 2] = r;
			}
		}
	}

	for (int i = y1; i <= y2; i++) {
		for (int j = -2; j < 3; j++) {
			if (x1 + j >= 0 && x1 + j < Width) {
				Data[4 * (i * Width + x1 + j)] = b;
				Data[4 * (i * Width + x1 + j) + 1] = g;
				Data[4 * (i * Width + x1 + j) + 2] = r;
			}
			if (x2 + j >= 0 && x2 + j < Width) {
				Data[4 * (i * Width + x2 + j)] = b;
				Data[4 * (i * Width + x2 + j) + 1] = g;
				Data[4 * (i * Width + x2 + j) + 2] = r;
			}
		}
	}

	return false;
}

// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::Modify() {
	// sample operation
	for (int i = 0; i < Width * Height; i++) {

	}

	return false;
}