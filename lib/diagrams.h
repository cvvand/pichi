
#ifndef PICHI_DIAGRAMS_H
#define PICHI_DIAGRAMS_H

#include "pichi/graph.h"
#include "pichi/tensor.h"

namespace pichi {



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
 * The input graph is not modified. The suggested extracted subgraph is
 * returned.
 */
Graph extract(const Graph& graph, int diagram);

}


#endif //PICHI_DIAGRAMS_H
