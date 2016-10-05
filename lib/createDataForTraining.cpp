/**
 * 1/3
 * This file is not part of the main program.
 *
 * We will iterate in the `puzzles folder` and for each puzzle we will extract all the cells holding a number
 * We will normalize the cell (25x25) and save these images for future use.
 * Future use ?
 * All these images will be used to train the multi layer perceptron artificial neural network
 *
 * Run this program to start classifying the images
 * The result will be a structure like that:
 * data
    ├── 1
    ├── 2
    ├── 3
    ├── 4
    ├── 5
    ├── 6
    ├── 7
    ├── 8
    └── 9
 *
 * where every recognized number (eg 9) will be save in his folder (9)
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * UNRECOGNIZE NUMBER WILL BE SAVE IN THE DATA FOLDER WAITING FOR AN MANUAL ACTION
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * after manual action (the data folder has only folder no more images)
 * run the randomizeDataForTraining.sh script
 * */

#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include "debug.h"
#include "extractPuzzle.h"
#include "cellHandling.h"

#include <iostream>

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#define BOOST_FILESYSTEM_VERSION 3

//  As an example program, we don't want to use any deprecated features
#ifndef BOOST_FILESYSTEM_NO_DEPRECATED
#  define BOOST_FILESYSTEM_NO_DEPRECATED
#endif
#ifndef BOOST_SYSTEM_NO_DEPRECATED
#  define BOOST_SYSTEM_NO_DEPRECATED
#endif

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"
#include <iostream>

namespace fs = boost::filesystem;
using namespace cv;
using namespace std;

std::string remove_extension(const std::string &filename) {
    size_t lastdot = filename.find_last_of(".");
    if (lastdot == std::string::npos) return filename;
    return filename.substr(0, lastdot);
}

std::string uuid_first_part(const std::string &uuid) {
    size_t first = uuid.find_first_of("-");
    if (first == std::string::npos) return uuid;
    return uuid.substr(0, first);
}

char *identifyText(Mat input) {
    tesseract::TessBaseAPI ocr;
    ocr.Init("./tessdata", "eng", tesseract::OEM_TESSERACT_ONLY);
    ocr.SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
    ocr.SetImage(input.data, input.cols, input.rows, 1,
                 input.step);
    ocr.SetVariable("tessedit_char_whitelist", "0123456789");

    char *text = ocr.GetUTF8Text();
//    cout << "Text:" << endl;
//    cout << text << endl;
//    cout << "Confidence: " << ocr.MeanTextConf() << endl << endl;
// Get the text

    return text;
}

/**
 * pre training
 *
 * ├── data
│*   ├── 1
│*   ├── 2
│*   ├── 3
│*   ├── 4
│*   ├── 5
│*   ├── 6
│*   ├── 7
│*   ├── 8
│*   └── 9
 *
 * create file image
 * An input representing a 4 will be saved in 4 if recognize if not in the parent folder
 *
 * */
void createPreTrainingData(Mat input, string fileNumber, int cellIndex) {

    string number = identifyText(input);

    std::stringstream trimmer;
    trimmer << number;
    number.clear();
    trimmer >> number;

    cout << "number is:" << number << endl;

    std::stringstream ss;
    if (number == "") {
        ss << "./data/" << fileNumber << "-" << cellIndex << ".jpg";
    } else {
        ss << "./data/" << number << "/" << fileNumber << "-" << cellIndex << ".jpg";
    }

    std::string filename = ss.str();
    cout << filename << endl;

    imwrite(filename, input);
}

void create_structure() {
    fs::path data("data");
    fs::path data1("data/1");
    fs::path data2("data/2");
    fs::path data3("data/3");
    fs::path data4("data/4");
    fs::path data5("data/5");
    fs::path data6("data/6");
    fs::path data7("data/7");
    fs::path data8("data/8");
    fs::path data9("data/9");

    if (!fs::exists(data)) {
        boost::filesystem::create_directories(data);
        boost::filesystem::create_directories(data1);
        boost::filesystem::create_directories(data2);
        boost::filesystem::create_directories(data3);
        boost::filesystem::create_directories(data4);
        boost::filesystem::create_directories(data5);
        boost::filesystem::create_directories(data6);
        boost::filesystem::create_directories(data7);
        boost::filesystem::create_directories(data8);
        boost::filesystem::create_directories(data9);
    } else {
        if (!fs::exists(data1))
            boost::filesystem::create_directories(data1);
        if (!fs::exists(data2))
            boost::filesystem::create_directories(data2);
        if (!fs::exists(data3))
            boost::filesystem::create_directories(data3);
        if (!fs::exists(data4))
            boost::filesystem::create_directories(data4);
        if (!fs::exists(data5))
            boost::filesystem::create_directories(data5);
        if (!fs::exists(data6))
            boost::filesystem::create_directories(data6);
        if (!fs::exists(data7))
            boost::filesystem::create_directories(data7);
        if (!fs::exists(data8))
            boost::filesystem::create_directories(data8);
        if (!fs::exists(data9))
            boost::filesystem::create_directories(data9);
    }


}

int main(int argc, char *argv[]) {

    create_structure();
//    fs::path p(fs::current_path());
    fs::path p("../puzzles");

    if (argc > 1)
        p = fs::system_complete(argv[1]);
    else
        std::cout << "\nusage:   simple_ls [path]" << std::endl;

    unsigned long file_count = 0;
    unsigned long dir_count = 0;
    unsigned long other_count = 0;
    unsigned long err_count = 0;

    if (!fs::exists(p)) {
        std::cout << "\nNot found: " << p << std::endl;
        return 1;
    }

    if (fs::is_directory(p)) {
        std::cout << "\nIn directory: " << p << "\n\n";
        fs::directory_iterator end_iter;
        for (fs::directory_iterator dir_itr(p);
             dir_itr != end_iter;
             ++dir_itr) {
            try {
                if (fs::is_directory(dir_itr->status())) {
                    ++dir_count;
                    std::cout << dir_itr->path().filename() << " [directory]\n";
                } else if (fs::is_regular_file(dir_itr->status())) {
                    ++file_count;
                    string fullName = dir_itr->path().filename().string();
                    string name = remove_extension(fullName);


                    stringstream ss(stringstream::in | stringstream::out);
                    ss << p.string() << "/" << fullName;

                    string fullPath = ss.str();
//                    std::cout << fullPath << "\n";

                    Mat raw = imread(fullPath, CV_LOAD_IMAGE_GRAYSCALE);


                    Mat preprocessed = preprocess(raw.clone());
                    vector<Point> biggestApprox = findBigestApprox(preprocessed);

                    Mat sudoku = extractPuzzle(raw, biggestApprox);


                    for (unsigned i = 0; i < 81; i++) {
                        Mat cell = getCell(sudoku, i), cell_no_noise, cell_no_light, final_cell;

                        // remove noise
                        medianBlur(cell, cell_no_noise, 1);
                        // remove background/light
                        cell_no_light = removeLight(cell_no_noise, calculateLightPattern(cell), 2);
                        // binarize image
                        adaptiveThreshold(cell_no_light, final_cell, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 3,
                                          1);

                        Mat roi = extractRoiFromCell(final_cell);

                        if (roi.empty()) {

                        } else {
                            string s = identifyText(roi);
//                            cout << s << endl;
//                            showImage(roi);
                            createPreTrainingData(roi, name, i);
                        }
                    }
                } else {
                    ++other_count;
                    std::cout << dir_itr->path().filename() << " [other]\n";
                }

            }
            catch (const std::exception &ex) {
                ++err_count;
                std::cout << dir_itr->path().filename() << " " << ex.what() << std::endl;
            }
        }
        std::cout << "\n" << file_count << " files\n"
                  << dir_count << " directories\n"
                  << other_count << " others\n"
                  << err_count << " errors\n";
    } else // must be a file
    {
        std::cout << "\nFound: " << p << "\n";
    }
    return 0;
}