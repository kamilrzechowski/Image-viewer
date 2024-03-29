// ImageViewer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/photo.hpp>
#include <fstream>
#include <iostream>
#include <experimental/filesystem>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;
namespace fs = std::experimental::filesystem;

void help(char** argv)
{
	cout << "\nThis program gets you started reading a sequence of images using cv::VideoCapture.\n"
		<< "Image sequences are a common way to distribute video data sets for computer vision.\n"
		<< "Usage: " << argv[0] << " <path to the first image in the sequence>\n"
		<< "example: " << argv[0] << " right%%02d.jpg\n"
		<< "q,Q,esc -- quit\n"
		<< "\tThis is a starter sample, to get you up and going in a copy paste fashion\n"
		<< endl;
}

std::vector<std::string> get_filenames(std::experimental::filesystem::path path)
{
	namespace stdfs = std::experimental::filesystem;

	std::vector<std::string> filenames;

	// http://en.cppreference.com/w/cpp/experimental/fs/directory_iterator
	const stdfs::directory_iterator end{};

	for (stdfs::directory_iterator iter{ path }; iter != end; ++iter)
	{
		// http://en.cppreference.com/w/cpp/experimental/fs/is_regular_file 
		if (stdfs::is_regular_file(*iter)) // comment out if all names (names of directories tc.) are required
			filenames.push_back(iter->path().string());
	}

	return filenames;
}

vector<int> readXML(string path) {
	ifstream myReadFile;
	myReadFile.open(path);
	char output[100];
	vector<int> coordinates;

	if (myReadFile.is_open()) {
		while (!myReadFile.eof()) {


			myReadFile >> output;
			cout << output;


		}

		cout << endl;
		int length = strlen(output);
		//cout << "length: " << length;
		char przecinek = 'a';
		int position = length, prevPosition = length;
		for (int i = 0; i < 4; i++) {
			while (przecinek != ',') {
				position--;
				przecinek = output[position];
			}
			przecinek = 'a';

			char subbuff[10];
			position++;
			memcpy(subbuff, &output[position], prevPosition - position);
			subbuff[prevPosition - position] = '\0';
			position--;
			prevPosition = position;

			int liczba = atoi(subbuff);
			coordinates.push_back(liczba);
		}
	}
	myReadFile.close();

	return coordinates;
}

int main()
{
	string path = "C:/KamilVisualStudio/python_TensorFlow/python_tf_generate_record/test_img";// "C:/KamilVisualStudio/python_TensorFlow/img_piff/";
	string ext = ".jpg";
	int position = 0;
	//for (auto & p : fs::directory_iterator(path)) {
	//const fs::directory_iterator end{};
	vector<fs::path> files;

	fs::path apk_path(path);
	fs::recursive_directory_iterator end;

	for (fs::recursive_directory_iterator i(apk_path); i != end; ++i)
	{
		const fs::path cp = (*i);
		files.push_back(cp);
	}

	for (std::vector<fs::path>::iterator p = files.begin(); p != files.end(); ++p) {
		if (p->extension() == ext) {
			//std::cout << p << std::endl;
	
			Mat img = imread(p->string());
			string myPath = p->string();
			myPath.replace(myPath.find(ext), sizeof(".txt") - 1, ".txt");

			vector<int> coordinates = readXML(myPath);
			rectangle(img, Point(coordinates[3], coordinates[2]), Point(coordinates[1], coordinates[0]), Scalar(255, 0, 0));

			imshow("myImg", img);
			//waitKey(0);
			char key = (char)waitKey(30000);
			if (key == 'd') {
				remove(*p);
				const char *cstr = myPath.c_str();
				remove(cstr);
			}
			else if (key == 'b') {
				if (position >= 3) {
					p = p - 3;
					position -= 3;
				}
			}
		}
		else if (p->extension() == ".xmla") {
			ifstream myReadFile;
			myReadFile.open(p->string());
			char output[100];
			if (myReadFile.is_open()) {
				while (!myReadFile.eof()) {


					myReadFile >> output;
					cout << output;


				}
				cout << endl;
				int length = strlen(output);
				//cout << "length: " << length;
				vector<int> coordinates;
				char przecinek = 'a';
				int position = length, prevPosition = length;
				for (int i = 0; i < 4; i++) {
					while (przecinek != ',') {
						position--;
						przecinek = output[position];
					}
					przecinek = 'a';

					char subbuff[10];
					position++;
					memcpy(subbuff, &output[position], prevPosition - position);
					subbuff[prevPosition - position] = '\0';
					position--;
					prevPosition = position;

					int liczba = atoi(subbuff);
					coordinates.push_back(liczba);
				}
			}
			myReadFile.close();
		}
		position++;
	}

    return 0;
}

