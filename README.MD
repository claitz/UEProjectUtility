# UEProjectUtility
UEProjectUtility is a C++ based utility tool specifically designed for Unreal Engine projects. The tool offers a couple of primary functions which are essential during the development lifecycle of an Unreal Engine game.

## Features
1. **Project Renaming**: This tool can assist in renaming Unreal Engine projects, changing references within files to ensure a smooth and error-free transition.

2. **Project Cleaning**: This tool also offers the functionality to clean Unreal Engine projects by deleting generated and temporary files that can be safely removed.

## Screenshots

![UEPU Screenshot (gif)](https://media.giphy.com/media/hhvmxUp8zRKqeuvvuD/giphy.gif)

## How to Use
Once launched, the tool will display a menu with the available options for the user to choose from. The options are:

1. **Rename project**: Allows you to input an existing project's name and the new name you want to give to the project. The tool then proceeds to rename the project and change references within files.

2. **Clean project**: Lets you input the name of an existing project. The tool then cleans the project by deleting generated and temporary files.

3. **Exit**: Terminate the application.

Once an option is chosen, the user is then asked to input the necessary information depending on the chosen operation. The tool then handles the rest.

## Where to Get
To get a working version of this tool, please visit the Releases page.  
Simply download the provided binary for your platform, and you're good to go!

## Building from Source
Binaries from the interwebs not your thing? No problem!  
You probably got this, but just in case, you will need to have CMake, any C++17 compatible compiler, and Termcolor installed on your machine.

### Step by Step

1. Clone this repository to your local machine.
2. Open a terminal and navigate to the cloned repository.
3. Create a new directory called `build` and navigate to it.
4. Run `cmake ..` to generate the build files.
5. Run `make` to build the project.
6. The built binaries will be located in the `bin` directory.
7. You can now run the tool by executing the binary file.
8. Enjoy!

## Support and Contributions
We welcome any community contributions!  
If you find a bug, have feature requests, or want to contribute to the code, please feel free to open an issue or submit a pull request.

## Acknowledgements
This tool uses the following libraries:
- [Termcolor](https://github.com/ikalnytskyi/termcolor)