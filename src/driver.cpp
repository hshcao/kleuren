/*
 * Implementation of driver.h
 */

#include <iostream>
#include <memory>

#include "driver.h"

extern "C" {
    #include <bft.h>
}

using std::cout;
using std::endl;

Driver::Driver(Args args_p) {
    this->args = args_p;

    char* bftFilePath = (char*) malloc(args.getBFTFilePath().length() + 1);
    strcpy(bftFilePath, args.getBFTFilePath().c_str());
    graph = new Graph(bftFilePath);

    if(args.getN() == 0) {
        args.setN(graph->getNumColors());
    }

    // open the kmer file
    kmerFile = new ifstream();
    kmerFile->open(args.getKmerFilePath());

    kmerBank = new KmerBank(kmerFile);

    bubbleStats = new BubbleStats();

    bubbleBuilder = new BubbleBuilder(graph, bubbleStats);

    // create the output bubble file
    if(!args.getBubbleFilePath().empty()) {
        bubbleFile = new ofstream();
        bubbleFile->open(args.getBubbleFilePath());
    }

    bubbleManager = BubbleManager(bubbleFile, graph);
}

Driver::~Driver() {
    kmerFile->close();
    delete kmerFile;
    delete kmerBank;
    delete graph;
    delete bubbleBuilder;
    delete bubbleStats;
    if(!args.getBubbleFilePath().empty()) {
        bubbleFile->close();
        delete bubbleFile;
    }
}

void Driver::run() {
    string kmer = kmerBank->getNextKmer();

    // iterate over the kmers
    while(kmer != "") {
        bubbleStats->incNumKmers();
        char* strKmer = const_cast<char*>(kmer.c_str());
        // find a start kmer for a bubble
        if(graph->isBFTKmer(strKmer)) {
            BFT_kmer* bftKmer = graph->getBFTKmer(strKmer);

            // build the bubble
            Bubble bubble = bubbleBuilder->build(bftKmer, args.getN(), args.getMaxDepth());
            if(bubble.getPaths().empty()) { // no bubble was found, try next kmer
                bubbleStats->incNumNoPathsFound();
                kmer = kmerBank->getNextKmer();
                continue;
            }
            if(!bubble.isValid(kmer.length(), args.getN())) { // the bubble is not valid, try next kmer
                kmer = kmerBank->getNextKmer();
                continue;
            }
            else {
                bubbleStats->incNumBubblesFound();
            }
            // write the bubble if there is a file to write to
            if(!args.getBubbleFilePath().empty()) {
                bubbleManager.writeBubble(bubble);
            }
        }
        // get the next kmer
        kmer = kmerBank->getNextKmer();
    }
    cout << bubbleStats->toString() << endl;
}

