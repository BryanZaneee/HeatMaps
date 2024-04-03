# Gainesville HeatMap

Project that creates a heat map of Gainesville that displays car crash locations at the University of Florida and the surronding areas. This implementation allows the user to select between a B-Tree or HashMap data strucutre to compare the time it takes for each structure to generate the map.

## CMake

This project utilizes CMake as its build system, simplifying the build process across different platforms. CMake automates the detection and configuration of various dependencies and compiler options, ensuring that the project can be built with minimal setup!

### Getting it running!

Before building the project, you need to ensure that all dependencies are installed:

- **CMake**: Install the latest version of CMake (at least 3.14). You can download it from [CMake's official website](https://cmake.org/download/).
- **Compiler**: Ensure you have a C++14 compliant compiler installed. For macOS, you can use Clang that comes with Xcode. On Windows, you could use MinGW or Visual Studio.
- **Matplot++**: The project's CMake configuration is set to fetch and build this library automatically. If for any reason you need to install it manually, follow the instructions from the [Matplot++ GitHub repository](https://github.com/alandefreitas/matplotplusplus).

Once those dependencies are wrapped up, and you have cloned the repo, we can move on to buiding the heatmap program.


1. Navigate to the project's root directory in the terminal.
2. Navigate to the build directory, then run "cmake ..", This will generate the build files in the "build" directory.
3. Run the terminal command "make" for the makefile in the "build" directory that was generated to compile the code.
4. The executable is now within the "build" directory!
5. Run the terminal command "./HeatMap" in the build directory to begin the application and generate your heatmap.


## Usage

After building the project, run the executable generated in the `build` directory. The application prompts the user to choose between using a B-Tree or HashMap. Based on the selection, the program processes the data and generates a heat map visualization.
