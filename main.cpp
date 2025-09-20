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
 *              smart brace tracking and proper I/O detection.
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
    
    // Exclude control structures (with space after keyword)
    if (line.find("if (") != string::npos ||
        line.find("while (") != string::npos ||
        line.find("for (") != string::npos ||
        line.find("switch (") != string::npos)
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
 * IsIOStatement
 * This function checks if a line contains input/output operations.
 * Looks for cout, cin, printf, scanf, etc.
 * Input: line [IN] - the code line to examine
 * Return: bool - returns true if line contains I/O operations,
 *                false otherwise. No side effects.
 */
bool IsIOStatement(string line)
{
    return (line.find("cout") != string::npos ||
            line.find("cin") != string::npos ||
            line.find("printf") != string::npos ||
            line.find("scanf") != string::npos ||
            line.find("getline") != string::npos);
} // IsIOStatement



/*
 * IsControlStatement
 * This function checks if a line contains control flow statements.
 * Looks for if, while, for, switch, etc.
 * Input: line [IN] - the code line to examine
 * Return: bool - returns true if line contains control statements,
 *                false otherwise. No side effects.
 */
bool IsControlStatement(string line)
{
    return (line.find("if (") != string::npos ||
            line.find("else if (") != string::npos ||
            line.find("else") != string::npos ||
            line.find("while (") != string::npos ||
            line.find("for (") != string::npos ||
            line.find("switch (") != string::npos ||
            line.find("do") != string::npos);
} // IsControlStatement



/*
 * main
 * This function is the main program entry point with smart brace tracking.
 * Tracks function depth to only ask about function-level closing braces.
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
    
    // Process file line by line with smart brace tracking
    string currentLine;
    int braceDepth = 0;
    bool inFunction = false;
    string lastFunctionName = "";
    
    cout << endl << "Processing your code..." << endl << endl;
    
    while (getline(inputFile, currentLine))
    {
        // Skip existing comments
        if (currentLine.find("//") == 0 || currentLine.find("/*") == 0)
        {
            outputFile << currentLine << endl;
            continue;
        }
        
        // Count braces to track depth
        int openBraces = 0;
        int closeBraces = 0;
        for (char c : currentLine)
        {
            if (c == '{') openBraces++;
            if (c == '}') closeBraces++;
        }
        
        // Detect function definitions
        if (IsLikelyFunctionStart(currentLine))
        {
            cout << "Found function: " << currentLine << endl;
            
            string answer;
            cout << "Add function comment? (y/n): ";
            getline(cin, answer);
            
            string funcName = "";
            if (answer == "y" || answer == "Y")
            {
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
            else
            {
                // Still need to get function name for end detection
                cout << "Enter function name for end detection (or Enter to skip): ";
                getline(cin, funcName);
            }
            
            // Remember function name for closing brace detection
            if (!funcName.empty())
            {
                lastFunctionName = funcName;
            }
            
            // Mark that we're entering a function
            inFunction = true;
            braceDepth = 0; // Will be incremented when we see the opening brace
            
            cout << endl;
        }
        
        // Detect I/O statements
        else if (IsIOStatement(currentLine))
        {
            cout << "Found I/O statement: " << currentLine << endl;
            
            string answer;
            cout << "Add comment for this I/O? (y/n): ";
            getline(cin, answer);
            
            if (answer == "y" || answer == "Y")
            {
                string comment;
                cout << "What does this I/O do? ";
                getline(cin, comment);
                
                outputFile << "    // " << comment << endl;
            }
            cout << endl;
        }
        
        // Detect control statements
        else if (IsControlStatement(currentLine))
        {
            cout << "Found control statement: " << currentLine << endl;
            
            string answer;
            cout << "Add comment for this control statement? (y/n): ";
            getline(cin, answer);
            
            if (answer == "y" || answer == "Y")
            {
                string comment;
                cout << "What does this control statement do? ";
                getline(cin, comment);
                
                outputFile << "    // " << comment << endl;
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
        
        // Update brace depth first
        braceDepth += openBraces - closeBraces;
        
        // Write the original line (but check if we need to add function end comment)
        if (inFunction && closeBraces > 0 && braceDepth == 0)
        {
            // This is the end of a function - check if user wants end comment
            if (!lastFunctionName.empty())
            {
                cout << "End of function '" << lastFunctionName << "' detected: " << currentLine << endl;
                string answer;
                cout << "Add function end comment? (y/n): ";
                getline(cin, answer);
                
                if (answer == "y" || answer == "Y")
                {
                    // Add comment to the same line as the closing brace
                    outputFile << currentLine << "  // end of \"" << lastFunctionName << "\"" << endl;
                    outputFile << endl << endl;
                }
                else
                {
                    outputFile << currentLine << endl;
                }
                cout << endl;
            }
            else
            {
                outputFile << currentLine << endl;
            }
            inFunction = false;
            lastFunctionName = "";
        }
        else
        {
            // Normal line - just write it
            outputFile << currentLine << endl;
        }
        
        // If we just entered a function and saw an opening brace, set depth properly
        if (inFunction && openBraces > 0 && braceDepth == openBraces)
        {
            // This handles the case where { is on the same line as function declaration
            // braceDepth is already set correctly by the += operation above
        }
    }
    
    // Close files
    inputFile.close();
    outputFile.close();
    
    cout << endl << "Done! Commented code saved as: " << outputFileName << endl;
    cout << "// ============================================================================" << endl;
    
    return 0;
} // main
