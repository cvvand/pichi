
#ifndef PICHI_STRING_UTILS_H
#define PICHI_STRING_UTILS_H

#include <vector>
#include <string>

namespace pichi {

/*
 * Matches two strings:
 * Finds the subset of characters in the strings which occurs in both. The
 * result is a list of pairs of integers giving the position of the matches.
 *
 * Example:
 *    "abcd"+"aebf" -> [(0,0),(1,2)] ,
 * since "a" is in position 0 and 0, and "b" is in position 1 and 2.
 *
 * The result is empty if there are no matches.
 */
std::vector<std::pair<int,int>> matchChars(std::string s1, std::string s2);

/*
 * Combines two strings, cutting away the duplicates:
 * This function basically finds the symmetric difference between the two
 * strings. The result is this difference, starting with string 1, in order.
 *
 * Example:
 *    "abcd"+"aebf" -> "cdef"
 * since "a" and "b" occur in both strings, and the rest is in order.
 */
std::string residualString(std::string s1, std::string s2);


/*
 * Split a contraction string into nodes, one for each tensor. For example, the
 * string
 * "12ab77acd103cdb21ee"
 * would be divided into
 * "12ab", "77acd", "103cdb", "21ee"
 */
std::vector<std::string> splitToNodes(const std::string&);

/*
 * Splits a contraction string into connected components. For example, the
 * string
 * "12ab77acd103cdb21ee"
 * is split into two connected parts:
 * "12ab77acd103cdb"
 * "21ee"
 */
std::vector<std::string> splitToConnected(const std::string&);

}

#endif //PICHI_STRING_UTILS_H
