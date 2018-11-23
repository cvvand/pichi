
#ifndef PICHI_DIAGRAMS_H
#define PICHI_DIAGRAMS_H

#include "pichi/graph.h"
#include "pichi/tensor.h"

namespace pichi {


/*
 * Diagrams are known contraction patterns, where for performance reasons it
 * is beneficial to contract the tensors in a certain way. A diagram is
 * implemented as a graph.
 * There are currently 10 diagrams implemented (isomorphic diagrams are
 * treated equally):
 *
 *  ------------------------------------------------------------------------
 * |Diagram number  |  String representation       |  Hadron interpretation |
 * |----------------|------------------------------|------------------------|
 * |    0           |          A_aa                | Meson internal loop    |
 * |    1           |       A_ab B_ab              | Meson-meson            |
 * |    2           |     A_ab B_bc C_ac           | Meson triangle         |
 * |    3           |   A_ab B_bc C_cd D_ad        | Meson box              |
 * |    4           |      A_abc B_abc             | Baryon-baryon          |
 * |    5           |    A_abc B_abd C_cd          | BBM triangle           |
 * |    6           |  A_abc B_abd C_ce D_de       | BBM box                |
 * |    7           |  A_abc B_ade C_bd D_ce       | BBM double triangle    |
 * |    8           |  A_abc B_abd C_def D_cef     | Baryon box             |
 * |    9           |     A_ab B_cd C_abcd         | Tetraquark - MM        |
 *  ------------------------------------------------------------------------
 */


/*
 * Identify a diagram in a graph. Assuming that the input graph is isomorphic
 * to a known diagram, this function finds out which diagram it is.
 */
int identifyDiagram(const Graph& graph);

/*
 * Extract a part of a graph, such that if the extracted part was evaluated
 * it would reduce the diagram into another, lower level diagram. An example
 * could be evaluating one of the contractions in diagram 3 in order to end
 * up in diagram 2.
 * Unknown diagrams are not reduced systematically but by a greedy (and
 * highly suboptimal) approach.
 * The input graph is not modified. The suggested extracted subgraph is
 * returned.
 */
Graph extract(const Graph& graph, int diagram);

}


#endif //PICHI_DIAGRAMS_H
