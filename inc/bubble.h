/**
 * @class Bubble
 *
 * Bubble represents a set of paths for 2 or more colors that share a common
 * starting and ending node. 
 *
 * @author Cole Lyman
 *
 * @date 2017/6/21
 *
 */

#ifndef BUBBLE_H
#define BUBBLE_H

#include <map>
#include <utility>
#include <set>
#include <memory>

#include "path.h"

using std::map;
using std::pair;
using std::set;
using std::shared_ptr;

class Bubble {
    
    public:
        Bubble();

        /// Checks if the path is present in the bubble
        bool pathExists(Path path) const;

        /// Checks if a bubble is valid, meaning it fits the following criteria:
        /// * there is more than one path
        /// * none of the paths are empty
        bool isValid(size_t kmerLen) const;

        /** 
         * Adds a path to paths, and appends the color to the corresponding path
         * if the path is already contained in colors.
         * @param path the path to add to paths
         * @param colors the colors that are associated with the path
         */
        void addPath(Path path, bit_vector colors);

        /// Returns the colors that are associated with path
        bit_vector getColors(Path path) const;

        /// Returns the paths of the bubble
        map<Path, bit_vector> getPaths() const;

    private:
        /// Return all of the paths in a vector
        vector<Path> pathsToVector() const;

        /// The paths that this bubble holds
        map<Path, bit_vector> paths;

};

#endif // BUBBLE_H
