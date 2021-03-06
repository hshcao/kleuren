/*
 * Implementation of graph.h.
 */

#include "graph.h"

Graph::Graph(char* bftFileName) {
    bft = load_BFT(bftFileName);
    free(bftFileName);
    //set_neighbors_traversal(bft);
}

Graph::~Graph() {
    free_cdbg(bft);
}

bool Graph::isBFTKmer(char* kmer) const {
    uint32_t* colorSet = query_sequence(bft, kmer, 0.001, false);
    if(colorSet == NULL) {
        return false;
    }
    uint32_t numColors = colorSet[0];
    free(colorSet);
    if(numColors) {
        return true;
    }
    return false;
}

BFT_kmer* Graph::getBFTKmer(char* kmer) const {
    if(isBFTKmer(kmer)) {
        return get_kmer(kmer, bft);
    }
    return NULL;
}

bool Graph::isValidBFTKmer(BFT_kmer* bftKmer) const {
    if(bftKmer != NULL) {
        if(is_kmer_in_cdbg(bftKmer)) {
            return true;
        }
    }
    return false;
}

uint32_t Graph::getNumColors(BFT_kmer* bftKmer) const {
    uint32_t numColors = 0;
    BFT_annotation* bftAnno = get_annotation(bftKmer);
    uint32_t* temp = get_list_id_genomes(bftAnno, bft);
    numColors = temp[0];
    free_BFT_annotation(bftAnno);
    free(temp);

    return numColors;
}

uint32_t Graph::getNumColors() const {
    return bft->nb_genomes;
}

uint32_t* Graph::getColors(BFT_kmer* bftKmer) const {
    BFT_annotation* bftAnno = get_annotation(bftKmer);

    uint32_t* colors = get_list_id_genomes(bftAnno, bft);
    free_BFT_annotation(bftAnno);
    return colors;
}

char* Graph::getColorFilePath(uint32_t colorId) const {
    return bft->filenames[colorId];
}

BFT_kmer* Graph::getSuffixNeighbors(BFT_kmer* bftKmer) const {
    if(is_kmer_in_cdbg(bftKmer)) {
        return get_successors(bftKmer, bft);
    }
    return NULL;
}

bool Graph::hasSuffixNeighbors(BFT_kmer* bftKmer) const {
    BFT_kmer* neighbors = getSuffixNeighbors(bftKmer);
    if(neighbors == NULL) {
        return false;
    }
    bool hasNeighbors = !(checkIfEmpty(neighbors));
    free_BFT_kmer(neighbors, 4);
    return hasNeighbors;
}

BFT_kmer* Graph::getPrefixNeighbors(BFT_kmer* bftKmer) const {
    if(is_kmer_in_cdbg(bftKmer)) {
        return get_predecessors(bftKmer, bft);
    }
    return NULL;
}

bool Graph::hasPrefixNeighbors(BFT_kmer* bftKmer) const {
    BFT_kmer* neighbors = getPrefixNeighbors(bftKmer);
    if(neighbors == NULL) {
        return false;
    }
    bool hasNeighbors = !(checkIfEmpty(neighbors));
    free_BFT_kmer(neighbors, 4);
    return hasNeighbors;
}

bool Graph::checkIfEmpty(BFT_kmer* bftKmers) const {
    for(uint32_t i = 0; i < 4; i++) {
        if(isValidBFTKmer(bftKmers + i)) {
            return false;
        }
    }
    return true;
}
