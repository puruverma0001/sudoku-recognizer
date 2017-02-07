# Sudoku-recognizer

## Requirement
* [CMake](https://cmake.org/)  
* [OpenCV 3.1.0](http://opencv.org/downloads.html) (This [link](http://docs.opencv.org/3.1.0/df/d65/tutorial_table_of_content_introduction.html) might be useful  )

## Useful information
Note that all the commands have to be invoked in the `build` folder. The first time you have to create it.
```
mkdir build && cd build
```


## Recognizer
![hhh.gif](https://bitbucket.org/repo/njp6xM/images/3411734402-hhh.gif)


### Tests
Once you have installed all the requirements, execute the tests to see if everything is OK
```
cmake .. && ctest -V
```

### Parse one of the provided puzzle located in `assets/puzzles`
```
cmake .. && make && src/recognize ./../assets/puzzles/s1.jpg
```

### Tools that can help you during the development process
#### Show all extracted puzzles located in `assets/puzzles`
```
cmake .. && make && src/showExtracted
```

![eee.gif](https://bitbucket.org/repo/njp6xM/images/4118982265-eee.gif)

#### Show all extracted cells for `assets/puzzles/s33.jpg`
```
cmake .. && make && src/showExtracted --puzzleNumber 33 --showCell
```

![fff.gif](https://bitbucket.org/repo/njp6xM/images/1253348601-fff.gif)

#### Other actions

##### will show all extracted puzzles  
```
cmake .. && make && src/showExtracted  
```

##### will show all extracted cells  
```
cmake .. && make && src/showExtracted --showCell  
```

##### will show the extracted puzzle: `assets/puzzles/s33.jpg`  
```
cmake .. && make && src/showExtracted --puzzleNumber 33  
```

##### will show the cell index: 10 of `assets/puzzles/s33.jpg`  
```
## useful in situation where only cell number 40 is wrong
cmake .. && make && src/showExtracted --puzzleNumber 33 --showCell --cellNumber 10  
```

##### will show the cell index: 10 of `assets/puzzles/s33.jpg` plus the process of extraction  
```
cmake .. && make && src/showExtracted --puzzleNumber 33 --showCell --cellNumber 10 --debug  
```

### Create the `assets/raw-features.yml` yourself
```
cmake .. && make && src/prepareData # create training data from s0 --> s35
```

* The knn network can be tested using this:
```
cmake .. && make && src/testKnn
```
> You can iterate over each picture by setting debug=true in the `testKnn` fn  

## The whole process

```
cmake .. && make && src/mouline ./../assets/puzzles/s0.jpg
```

![iii.gif](https://bitbucket.org/repo/njp6xM/images/1534426116-iii.gif)

All the hard work was to recognize the numbers that's why this part comes after the `Recognizer` even if obviously this is what people want to do. 
A [mouline](https://github.com/BenNG/sudoku-recognizer/blob/master/src/lib/sudoku.cpp#L1743) function has been created that gather all other functions.
So the process is:
 * parse the original image that holds the sudoku puzzle and extract it
 * parse the extracted puzzle and use a computer vision algorithm to grab the initial state.
 * send the initial state to a server that solve the sudoku
 * write the solution on the extracted puzzle
 * merge the extracted puzzle on the original picture


## Auto questions
### Why do we need a server to solve the sudoku you might ask ? 
This is because at the very begining all I wanted is learning this [language](http://elixir-lang.org/) so I started this [project](https://github.com/BenNG/sudoku-solver) and now I use it :)

## Tips
* There are 2 extraction process
  * the extraction of the puzzle
  * the extraction of the grid within the puzzle (somethimes it is the same see s6.jpg)
* If you want to play with `HOGDescriptor` add set(OpenCV_LIBS opencv_core opencv_objdetect) (this is not used in the project anymore)
* The function `prepareData` creates `assets/raw-features.yml` and getKnn uses it
* boost library
 * check is the /FindBoost.cmake the supported version (the lib is not used anymore because it is not possible to use it on android)
* If you ask for a cell and you have an error chances are that your cell is a 1 and it is too tiny go to extractNumber and play with (and do not forget to run prepareData && test after)
  * area
  * boundingArea
  * width_threshold
  * height_threshold
* Before the portage on android, I used a system for resolving the path of file that looked for the root of the project and join the path given but in a apk application
  there are no path so I had to rewrite all and use relative paths
* adding curl to the project
  * dl the source code [here](https://github.com/curl/curl/releases)
  * in the source code there is a `CMakeLists.txt` so:
    * cmake .
    * make
    * sudo make install
  * [then](https://github.com/BenNG/sudoku-recognizer/blob/2f30e4dc54620b646df8a97a8ec5651a171c3e56/src/CMakeLists.txt#L1)
  * [and](https://github.com/BenNG/sudoku-recognizer/blob/2f30e4dc54620b646df8a97a8ec5651a171c3e56/src/CMakeLists.txt#L31)
* When you do a project using computer vision think about the resolution you will use for:
  * the camera
  * the image being processed by the algorithm   


## Thank you for answering my questions
* [Berak](http://answers.opencv.org/users/2130/berak/)
* [Baptiste Wicht](https://github.com/wichtounet/)
