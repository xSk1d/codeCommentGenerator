/*
 * Filename: comment_generator.cpp
 * Programmer: Lin Aung
 * Date: September 19, 2025
 * Class: 11210 CSCI 123 F
 * Class Time: 11:45 - 1:45
 * Instructor: Luciano Rodriguez
 * Project: C++ Comment Generator
 * Description: This program reads a C++ file and adds proper comments
 *              according to coding style guidelines. Fixed version with
 *              better validation and proper formatting.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;



/*
 * ExpandPath
 * This function expands ~ to home directory path if needed.
 * Assumes fileName contains a valid string.
 * Input: fileName [IN] - the file path that might contain ~
 * Return: string - returns expanded path with ~ replaced by home directory.
 *                  No side effects.
 */
string ExpandPath(string fileName)
{
    if (fileName.length() > 0 && fileName[0] == '~')
    {
        // Get home directory from environment
        const char* homeDir = getenv("HOME");
        if (homeDir != nullptr)
        {
            string homePath = homeDir;
            return homePath + fileName.substr(1); // Replace ~ with home path
        }
    }
    return fileName;
} // ExpandPath



/*
 * CheckIfFileExists
 * This function checks if a file exists by attempting to open it.
 * Handles ~ expansion and shows debugging info.
 * Input: fileName [IN] - the name or path of the file to check
 * Return: bool - returns true if file exists and can be opened,
 *                false otherwise. Side effect: shows expanded path.
 */
bool CheckIfFileExists(string fileName)
{
    string expandedPath = ExpandPath(fileName);
    cout << "Checking file: " << expandedPath << endl; // Debug info
    
    ifstream testFile(expandedPath);
    bool fileExists = testFile.is_open();
    testFile.close();
    return fileExists;
} // CheckIfFileExists



/*
 * GetValidFilePath
 * This function prompts user repeatedly until valid file path is entered.
 * Handles ~ expansion and provides helpful suggestions.
 * Input: None
 * Return: string - returns a valid expanded file path that exists on system.
 *                  Side effect: displays prompts and messages to user.
 */
string GetValidFilePath()
{
    string filePath;
    bool isFileFound = false;
    
    cout << "Tips: You can use:" << endl;
    cout << "  - Relative path: cashRegisterLoop.cpp" << endl;
    cout << "  - ~ for home: ~/HW02/cashRegisterLoop.cpp" << endl;
    cout << "  - Full path: /home/laung/HW02/cashRegisterLoop.cpp" << endl;
    cout << endl;
    
    while (!isFileFound)
    {
        cout << "Enter your C++ file name or full path: ";
        getline(cin, filePath);
        
        if (CheckIfFileExists(filePath))
        {
            isFileFound = true;
            cout << "File found!" << endl;
        }
        else
        {
            cout << "Error: File not found." << endl;
            cout << "Try checking if the file exists with: ls " << ExpandPath(filePath) << endl;
        }
    }
    
    return ExpandPath(filePath); // Return the expanded path
} // GetValidFilePath



/*
 * GetValidParameterMode
 * This function validates and gets proper parameter mode from user.
 * Keeps prompting until user enters IN, OUT, or IN/OUT.
 * Input: paramName [IN] - name of parameter to ask about
 * Return: string - returns valid parameter mode (IN, OUT, or IN/OUT).
 *                  Side effect: displays prompts to user.
 */
string GetValidParameterMode(string paramName)
{
    string mode;
    bool validMode = false;
    
    while (!validMode)
    {
        cout << "Is '" << paramName << "' [IN], [OUT], or [IN/OUT]? ";
        getline(cin, mode);
        
        if (mode == "IN" || mode == "OUT" || mode == "IN/OUT")
        {
            validMode = true;
        }
        else
        {
            cout << "Please enter exactly: IN, OUT, or IN/OUT" << endl;
        }
    }
    
    return mode;
} // GetValidParameterMode



/*
 * CreateFileHeader
 * This function writes the standard file header comment block to output.
 * Follows the exact format from coding guidelines sample.
 * Input: outputFile [IN/OUT] - file stream to write header to
 *        fileName [IN] - original filename to include in header
 *        date [IN] - current date string
 *        project [IN] - project name
 *        description [IN] - program description
 * Return: void - no return value. Side effect: writes formatted header
 *                block to the output file stream.
 */
void CreateFileHeader(ofstream& outputFile, string fileName, string date, 
                      string project, string description)
{
    outputFile << "// ============================================================================" << endl;
    outputFile << "// file: " << fileName << endl;
    outputFile << "// ============================================================================" << endl;
    outputFile << "// Programmer: Lin Aung" << endl;
    outputFile << "// Date: " << date << endl;
    outputFile << "// Class: CSCI 123 (\"Intro to Programming Using C++\")" << endl;
    outputFile << "// Time: MW 11:45am" << endl;
    outputFile << "// Instructor: Luciano Rodriguez" << endl;
    outputFile << "// Project: " << project << endl;
    outputFile << "//" << endl;
    outputFile << "// Description:" << endl;
    outputFile << "//      " << description << endl;
    outputFile << "//" << endl;
    outputFile << "// ============================================================================" << endl;
    outputFile << endl;
} // CreateFileHeader



/*
 * CreateFunctionHeader
 * This function creates a properly formatted function comment header.
 * Follows the exact format from the coding guidelines sample code.
 * Input: outputFile [IN/OUT] - file stream to write comment to
 *        functionName [IN] - name of the function
 *        description [IN] - what the function does
 *        parameters [IN] - parameter descriptions with modes
 *        returnDesc [IN] - description of return value
 * Return: void - no return value. Side effect: writes formatted function
 *                comment block to output file.
 */
void CreateFunctionHeader(ofstream& outputFile, string functionName, 
                          string description, string parameters, string returnDesc)
{
    outputFile << endl << endl;
    outputFile << "// ==== " << functionName << " ";
    
    // Add equal signs to reach approximately 80 characters
    int nameLength = functionName.length();
    int equalsNeeded = 80 - 9 - nameLength; // 9 for "// ==== " and " "
    for (int i = 0; i < equalsNeeded; i++)
    {
        outputFile << "=";
    }
    outputFile << endl;
    
    outputFile << "//" << endl;
    outputFile << "// " << description << endl;
    outputFile << "//" << endl;
    
    if (!parameters.empty())
    {
        outputFile << "// Input:" << endl;
        outputFile << "//      " << parameters << endl;
        outputFile << "//" << endl;
    }
    
    if (!returnDesc.empty())
    {
        outputFile << "// Output:" << endl;
        outputFile << "//      " << returnDesc << endl;
        outputFile << "//" << endl;
    }
    
    outputFile << "// ============================================================================" << endl;
    outputFile << endl;
} // CreateFunctionHeader



/*
 * IsLikelyFunctionStart
 * This function determines if a line looks like start of a function.
 * Uses better heuristics to avoid false positives.
 * Input: line [IN] - the code line to examine
 * Return: bool - returns true if line appears to be function definition,
 *                false otherwise. No side effects.
 */
bool IsLikelyFunctionStart(string line)
{
    // Must have parentheses and not end with semicolon
    if (line.find('(') == string::npos || line.find(')') == string::npos)
    {
        return false;
    }
    
    if (line.find(';') != string::npos)
    {
        return false;
    }
    
    // Exclude control structures
    if (line.find("if ") != string::npos ||
        line.find("while ") != string::npos ||
        line.find("for ") != string::npos ||
        line.find("switch ") != string::npos)
    {
        return false;
    }
    
    // Exclude I/O statements
    if (line.find("cout") != string::npos ||
        line.find("cin") != string::npos)
    {
        return false;
    }
    
    return true;
} // IsLikelyFunctionStart



/*
 * IsLikelyFunctionEnd
 * This function determines if a closing brace is likely end of function.
 * Uses better logic to avoid asking about every closing brace.
 * Input: line [IN] - the code line with closing brace
 *        previousLine [IN] - the line before this one for context
 * Return: bool - returns true if this appears to be function end,
 *                false otherwise. No side effects.
 */
bool IsLikelyFunctionEnd(string line, string previousLine)
{
    // Must contain closing brace but not opening brace
    if (line.find('}') == string::npos || line.find('{') != string::npos)
    {
        return false;
    }
    
    // Skip if previous line looks like control structure
    if (previousLine.find("if") != string::npos ||
        previousLine.find("else") != string::npos ||
        previousLine.find("for") != string::npos ||
        previousLine.find("while") != string::npos ||
        previousLine.find("{") != string::npos)
    {
        return false;
    }
    
    // Skip if this line has other code besides the brace
    string trimmed = line;
    while (!trimmed.empty() && (trimmed[0] == ' ' || trimmed[0] == '\t'))
    {
        trimmed = trimmed.substr(1);
    }
    
    // Only ask if it's mostly just a closing brace
    return (trimmed.length() <= 3 && trimmed[0] == '}');
} // IsLikelyFunctionEnd



/*
 * main
 * This function is the main program entry point that orchestrates the
 * comment generation process with improved validation and formatting.
 * Input: None
 * Return: int - returns 0 for successful completion, 1 for file errors.
 *               Side effects: creates output file, displays user interface.
 */
int main()
{
    cout << "// ============================================================================" << endl;
    cout << "// Enhanced C++ Comment Generator" << endl;
    cout << "// ============================================================================" << endl;
    cout << endl;
    
    // Get and validate input file path
    string inputFilePath = GetValidFilePath();
    
    // Get output filename
    string outputFileName;
    cout << "Enter output filename (or press Enter for default): ";
    getline(cin, outputFileName);
    
    if (outputFileName.empty())
    {
        outputFileName = "commented_" + inputFilePath;
    }
    
    // Get header information
    string currentDate;
    string projectName;
    string programDescription;
    
    cout << endl << "File header information:" << endl;
    cout << "Today's date (MM/DD/YYYY): ";
    getline(cin, currentDate);
    
    cout << "Project name: ";
    getline(cin, projectName);
    
    cout << "Program description: ";
    getline(cin, programDescription);
    
    // Open files
    ifstream inputFile(inputFilePath);
    if (!inputFile.is_open())
    {
        cout << "Error: Cannot open " << inputFilePath << endl;
        return 1;
    }
    
    ofstream outputFile(outputFileName);
    if (!outputFile.is_open())
    {
        cout << "Error: Cannot create " << outputFileName << endl;
        return 1;
    }
    
    // Create file header
    CreateFileHeader(outputFile, inputFilePath, currentDate, projectName, programDescription);
    
    // Process file line by line
    string currentLine;
    string previousLine = "";
    cout << endl << "Processing your code..." << endl << endl;
    
    while (getline(inputFile, currentLine))
    {
        // Skip existing comments
        if (currentLine.find("//") == 0 || currentLine.find("/*") == 0)
        {
            outputFile << currentLine << endl;
            previousLine = currentLine;
            continue;
        }
        
        // Detect function definitions with better logic
        if (IsLikelyFunctionStart(currentLine))
        {
            cout << "Found function: " << currentLine << endl;
            
            string answer;
            cout << "Add function comment? (y/n): ";
            getline(cin, answer);
            
            if (answer == "y" || answer == "Y")
            {
                string funcName;
                string funcDesc;
                string funcParams = "";
                string funcReturn;
                
                cout << "Enter function name: ";
                getline(cin, funcName);
                
                cout << "What does this function do? ";
                getline(cin, funcDesc);
                
                string hasParams;
                cout << "Does this function have parameters? (y/n): ";
                getline(cin, hasParams);
                
                if (hasParams == "y" || hasParams == "Y")
                {
                    string paramName;
                    string paramDesc;
                    
                    cout << "Enter parameter name: ";
                    getline(cin, paramName);
                    
                    cout << "What does '" << paramName << "' do? ";
                    getline(cin, paramDesc);
                    
                    string paramMode = GetValidParameterMode(paramName);
                    
                    funcParams = paramName + " [" + paramMode + "] -- " + paramDesc;
                }
                
                cout << "What does this function return? (or Enter for void): ";
                getline(cin, funcReturn);
                
                CreateFunctionHeader(outputFile, funcName, funcDesc, funcParams, funcReturn);
            }
            cout << endl;
        }
        
        // Detect variable declarations
        else if ((currentLine.find("int ") != string::npos ||
                  currentLine.find("string ") != string::npos ||
                  currentLine.find("double ") != string::npos ||
                  currentLine.find("float ") != string::npos ||
                  currentLine.find("char ") != string::npos ||
                  currentLine.find("bool ") != string::npos) &&
                 currentLine.find(';') != string::npos &&
                 currentLine.find('(') == string::npos)
        {
            cout << "Found variable: " << currentLine << endl;
            
            string answer;
            cout << "Add variable comment? (y/n): ";
            getline(cin, answer);
            
            if (answer == "y" || answer == "Y")
            {
                string varComment;
                cout << "What is this variable for? ";
                getline(cin, varComment);
                
                outputFile << "    // " << varComment << endl;
            }
            cout << endl;
        }
        
        // Write the original line
        outputFile << currentLine << endl;
        
        // Handle function end comments ONLY for likely function ends
        if (IsLikelyFunctionEnd(currentLine, previousLine))
        {
            string functionName;
            cout << "Found end of function: " << currentLine << endl;
            cout << "Enter function name for this closing brace (or Enter to skip): ";
            getline(cin, functionName);
            
            if (!functionName.empty())
            {
                outputFile.seekp(-2, ios::cur);
                outputFile << "  // end of \"" << functionName << "\"" << endl;
                outputFile << endl << endl;
            }
        }
        
        previousLine = currentLine;
    }
    
    // Close files
    inputFile.close();
    outputFile.close();
    
    cout << endl << "Done! Commented code saved as: " << outputFileName << endl;
    cout << "// ============================================================================" << endl;
    
    return 0;
} // main
