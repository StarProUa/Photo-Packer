# Project
The project is written in C and has a small codebase. This program is designed for file packing and unpacking operations, and it is currently available exclusively for Windows. However, I would greatly appreciate a cross-platform fork of this project.

# Binary File Structure
The binary file format used by this program consists of the following elements:

1. **Separator (1 byte)**: A single byte used as a separator between records.
2. **File Name (256 bytes)**: The name of the recorded file, stored in 256 bytes.
3. **Data Size (8 bytes)**: The size of the recorded file's data, stored in 8 bytes.
4. **Data (n bytes)**: The actual data of the recorded file. The number of bytes is determined by the value stored in the "Data Size" field.

# Command-Line Parameters
The program supports the following command-line parameters:

1. `--pack` or `--unpack` (assigned to the `action` variable): This parameter determines whether the program should execute a packing operation (`--pack`) or an unpacking operation (`--unpack`). It does not accept any additional parameters.
2. `--name` (assigned to the `name` variable): This option requires one parameter and specifies a name to be stored in the `name` variable. This option should not be used when performing an unpacking operation.
3. `--erase` (assigned to the `erase` variable): This parameter indicates that the program should perform a file deletion operation. It does not accept any additional parameters.
4. `--dir` (assigned to the `dir` variable): This parameter requires one parameter and specifies the directory where the operation should be executed. The result is stored in the `dir` variable.
5. `--filter` (assigned to the `filter` variable): This parameter requires one parameter and specifies a filter or condition for the operation. The result is stored in the `filter` variable.

Please note the following regarding command-line usage:

- When using the `--dir` parameter, you must provide a full path to the directory (e.g., `D:/Folder1/Folder2/`).
- You can also simply drag and drop files onto the executable `Quick-Packer.exe` to perform packing and unpacking operations.
- If you run the program without specifying any parameters, no action will be taken.
- If you provide the full path to the file(s), the `--dir` parameter should not be used.
- You can also use parameter abbreviations, such as `-p` for `--pack`, and so on.

Here are examples of how to use the command-line parameters:


```Quick-Packer --pack --name SpritePack --dir C:/path/to/file/ --filter png```  
```Quick-Packer -p C:/path/to/files/file.txt```  
```Quick-Packer -u -d C:/path/to/file/ -e -f SpritePack1```  
```Quick-Packer C:/path/to/files/file1.txt C:/path/to/files/file2.txt C:/path/to/files/file3.txt```  
```Quick-Packer -u```

**Tip** - it can be used in conjunction with various libraries, for example, to upload photos using [stb-image](https://github.com/nothings/stb/tree/master), or to upload audio files using [PortAudio](https://github.com/PortAudio/portaudio).
