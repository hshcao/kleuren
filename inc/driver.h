/**
 * @class Driver
 *
 * This class is where the algorithms are started, and general logic
 * is performed to connect the various algorithms together.
 *
 * @author Cole Lyman
 *
 * @date 2017/6/22
 *
 */

#ifndef DRIVER_H
#define DRIVER_H

#include <set>
#include <fstream>

#include "args.h"
#include "kmer_bank.h"
#include "bubble_builder.h"
#include "bubble_manager.h"
#include "bubble_stats.h"
#include "graph.h"

using std::set;
using std::ifstream;
using std::ofstream;

class Driver {

    public:
        Driver(Args args);

        ~Driver();

        /// Starts the whole algorithm given the arguments
        void run();

    private:
        /// The pointer to the file handle that contains the kmers
        ifstream* kmerFile;

        /// The pointer to the file handle that will output the bubbles
        ofstream* bubbleFile;

        /// The KmerBank provides a super set of kmers for each color
        KmerBank* kmerBank;

        /// The Graph underlying structure to represent the Colored de Bruijn graph
        Graph* graph;

        /// Builds bubbles!
        BubbleBuilder* bubbleBuilder;

        /// Manages bubbles
        BubbleManager bubbleManager;

        /// Hold statistics about the bubbles and kleuren in general
        BubbleStats* bubbleStats;

        /// The arguments for the current run
        Args args;

};

#endif // DRIVER_H
