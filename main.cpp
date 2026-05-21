#include <iostream>
#include <fstream>
#include <string>

#include "VMManager.h"

using namespace std;

// ==========================================================
// main.cpp
// Virtual Memory Manager Driver
// ==========================================================

int main(int argc, char* argv[])
{
    // ------------------------------------------------------
    // Step 1:
    // Verify correct number of command-line arguments
    // ------------------------------------------------------

    if (argc != 4)
    {
        cout << "Usage: ./vm init.txt input.txt output.txt" << endl;
        return 1;
    }


    // ------------------------------------------------------
    // Step 2:
    // Store file names from command-line arguments
    // ------------------------------------------------------

    string initFile = argv[1];
    string inputFile = argv[2];
    string outputFile = argv[3];


    // ------------------------------------------------------
    // Step 3:
    // Create VMManager object
    // ------------------------------------------------------

    VMManager vm;


    // ------------------------------------------------------
    // Step 4:
    // Initialize VM system using initialization file
    // ------------------------------------------------------

    vm.initialize(initFile);


    // ------------------------------------------------------
    // Step 5:
    // Open VA input file
    // ------------------------------------------------------

    ifstream input(inputFile);

    if (!input.is_open())
    {
        cout << "Error opening input file." << endl;
        return 1;
    }


    // ------------------------------------------------------
    // Step 6:
    // Open output file
    // ------------------------------------------------------

    ofstream output(outputFile);

    if (!output.is_open())
    {
        cout << "Error opening output file." << endl;
        return 1;
    }


    // ------------------------------------------------------
    // Step 7:
    // Read and translate virtual addresses
    // ------------------------------------------------------

    int VA;
    bool firstOutput = true;

    while (input >> VA)
    {
        // Translate VA into PA
        int PA = vm.translateAddress(VA);

        // Prevent leading/trailing spacing issues
        if (!firstOutput)
        {
            output << " ";
        }

        output << PA;

        firstOutput = false;
    }


    // ------------------------------------------------------
    // Step 8:
    // Close files
    // ------------------------------------------------------

    input.close();
    output.close();


    // ------------------------------------------------------
    // Step 9:
    // Return success
    // ------------------------------------------------------

    return 0;
}