#include "VMManager.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

using namespace std;


// ==========================================================
// CONSTRUCTOR
// ==========================================================

VMManager::VMManager()
{
    // Initialize PM to 0
    for (int i = 0; i < PM_SIZE; i++)
    {
        PM[i] = 0;
    }

    // Initialize disk to 0
    for (int i = 0; i < DISK_BLOCKS; i++)
    {
        for (int j = 0; j < FRAME_SIZE; j++)
        {
            D[i][j] = 0;
        }
    }

    // Initialize frame usage
    for (int i = 0; i < NUM_FRAMES; i++)
    {
        frameUsed[i] = false;
    }

    // Segment table occupies frames 0 and 1
    frameUsed[0] = true;
    frameUsed[1] = true;
}


// ==========================================================
// INITIALIZATION
// ==========================================================

void VMManager::initialize(string initFile)
{
    ifstream file(initFile);

    string line;

    // ------------------------------------------------------
    // Read Segment Table line
    // ------------------------------------------------------

    getline(file, line);

    stringstream ss1(line);

    int s, z, f;

    while (ss1 >> s >> z >> f)
    {
        // Store segment size
        PM[2 * s] = z;

        // Store PT location
        PM[2 * s + 1] = f;

        // Mark PT frame occupied if resident
        if (f > 0)
        {
            frameUsed[f] = true;
        }
    }


    // ------------------------------------------------------
    // Read Page Table entries
    // ------------------------------------------------------

    getline(file, line);

    stringstream ss2(line);

    int p;

    while (ss2 >> s >> p >> f)
    {
        int ptLocation = PM[2 * s + 1];

        // --------------------------------------------------
        // PT is resident in PM
        // --------------------------------------------------

        if (ptLocation > 0)
        {
            int ptBase = ptLocation * FRAME_SIZE;

            PM[ptBase + p] = f;
        }

        // --------------------------------------------------
        // PT is stored on disk
        // --------------------------------------------------

        else if (ptLocation < 0)
        {
            int diskBlock = abs(ptLocation);

            D[diskBlock][p] = f;
        }

        // --------------------------------------------------
        // Mark resident page frames occupied
        // --------------------------------------------------

        if (f > 0)
        {
            frameUsed[f] = true;
        }
    }

    file.close();
}


// ==========================================================
// ADDRESS EXTRACTION HELPERS
// ==========================================================

int VMManager::extractSegment(int VA)
{
    return VA >> 18;
}


int VMManager::extractPage(int VA)
{
    return (VA >> 9) & OFFSET_MASK;
}


int VMManager::extractOffset(int VA)
{
    return VA & OFFSET_MASK;
}


int VMManager::extractPW(int VA)
{
    return VA & PW_MASK;
}


// ==========================================================
// FREE FRAME MANAGEMENT
// ==========================================================

int VMManager::allocateFreeFrame()
{
    for (int i = 0; i < NUM_FRAMES; i++)
    {
        if (!frameUsed[i])
        {
            frameUsed[i] = true;
            return i;
        }
    }

    // Spec guarantees free frames exist
    return -1;
}


// ==========================================================
// DISK OPERATIONS
// ==========================================================

void VMManager::readBlock(int diskBlock, int frameNumber)
{
    int start = frameNumber * FRAME_SIZE;

    for (int i = 0; i < FRAME_SIZE; i++)
    {
        PM[start + i] = D[diskBlock][i];
    }
}


// ==========================================================
// DEMAND PAGING HELPERS
// ==========================================================

void VMManager::loadPageTable(int segment)
{
    int entry = PM[2 * segment + 1];

    int diskBlock = abs(entry);

    int frame = allocateFreeFrame();

    readBlock(diskBlock, frame);

    // Update ST entry
    PM[2 * segment + 1] = frame;
}


void VMManager::loadPage(int segment, int page)
{
    int ptFrame = PM[2 * segment + 1];

    int ptBase = ptFrame * FRAME_SIZE;

    int entry = PM[ptBase + page];

    int diskBlock = abs(entry);

    int frame = allocateFreeFrame();

    readBlock(diskBlock, frame);

    // Update PT entry
    PM[ptBase + page] = frame;
}


// ==========================================================
// MAIN TRANSLATION FUNCTION
// ==========================================================

int VMManager::translateAddress(int VA)
{
    // ------------------------------------------------------
    // Extract address components
    // ------------------------------------------------------

    int s = extractSegment(VA);

    int p = extractPage(VA);

    int w = extractOffset(VA);

    int pw = extractPW(VA);


    // ------------------------------------------------------
    // Validate segment boundary
    // ------------------------------------------------------

    if (pw >= PM[2 * s])
    {
        return -1;
    }


    // ------------------------------------------------------
    // Handle PT page fault
    // ------------------------------------------------------

    if (PM[2 * s + 1] < 0)
    {
        loadPageTable(s);
    }


    // ------------------------------------------------------
    // Locate PT
    // ------------------------------------------------------

    int ptFrame = PM[2 * s + 1];

    int ptBase = ptFrame * FRAME_SIZE;


    // ------------------------------------------------------
    // Handle regular page fault
    // ------------------------------------------------------

    if (PM[ptBase + p] < 0)
    {
        loadPage(s, p);
    }


    // ------------------------------------------------------
    // Compute physical address
    // ------------------------------------------------------

    int frame = PM[ptBase + p];

    int PA = frame * FRAME_SIZE + w;

    return PA;
}


// ==========================================================
// OPTIONAL DEBUGGING FUNCTIONS
// ==========================================================

void VMManager::printPM()
{
    for (int i = 0; i < 200; i++)
    {
        cout << "PM[" << i << "] = " << PM[i] << endl;
    }
}


void VMManager::printFrameUsage()
{
    for (int i = 0; i < NUM_FRAMES; i++)
    {
        cout << "Frame " << i << ": ";

        if (frameUsed[i])
        {
            cout << "USED";
        }
        else
        {
            cout << "FREE";
        }

        cout << endl;
    }
}