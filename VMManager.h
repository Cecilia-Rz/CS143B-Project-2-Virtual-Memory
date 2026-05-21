#ifndef VMMANAGER_H
#define VMMANAGER_H

// ==========================================================
// VMManager.h
// Virtual Memory Manager Interface
// ==========================================================

#include <string>

using namespace std;


// ==========================================================
// VMManager Class
// ==========================================================

class VMManager
{
private:

    // ======================================================
    // CONSTANTS
    // ======================================================

    // ------------------------------------------------------
    // Physical memory specifications
    // ------------------------------------------------------

    static const int PM_SIZE = 524288;
    static const int NUM_FRAMES = 1024;
    static const int FRAME_SIZE = 512;


    // ------------------------------------------------------
    // Disk specifications
    // ------------------------------------------------------

    static const int DISK_BLOCKS = 1024;


    // ------------------------------------------------------
    // Address structure specifications
    // ------------------------------------------------------

    static const int ADDRESS_BITS = 9;

    static const int OFFSET_MASK = 0x1FF;
    static const int PW_MASK = 0x3FFFF;


    // ======================================================
    // MEMORY STRUCTURES
    // ======================================================

    // Physical Memory
    int PM[PM_SIZE];

    // Paging Disk
    int D[DISK_BLOCKS][FRAME_SIZE];


    // ======================================================
    // FREE FRAME MANAGEMENT
    // ======================================================

    // Tracks occupied/free frames
    bool frameUsed[NUM_FRAMES];


    // ======================================================
    // ADDRESS EXTRACTION HELPERS
    // ======================================================

    int extractSegment(int VA);

    int extractPage(int VA);

    int extractOffset(int VA);

    int extractPW(int VA);


    // ======================================================
    // DEMAND PAGING HELPERS
    // ======================================================

    int allocateFreeFrame();

    void readBlock(int diskBlock, int frameNumber);

    void loadPageTable(int segment);

    void loadPage(int segment, int page);


public:

    // ======================================================
    // CONSTRUCTOR
    // ======================================================

    VMManager();


    // ======================================================
    // INITIALIZATION
    // ======================================================

    void initialize(string initFile);


    // ======================================================
    // ADDRESS TRANSLATION
    // ======================================================

    int translateAddress(int VA);


    // ======================================================
    // OPTIONAL DEBUGGING FUNCTIONS
    // ======================================================

    void printPM();

    void printFrameUsage();
};


#endif