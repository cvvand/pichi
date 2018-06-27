
#ifndef PICHI_DIAGRAMS_H
#define PICHI_DIAGRAMS_H

#include "graph.h"
#include "tensor.h"

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

/*
 * Compute a completely contracted, known diagram, represented by a graph.
 * The graph nodes and the position of the corresponding tensor in the input
 * tensor array must be equal.
 * EXAMPLE: Compute the diagram 0_ab 1_acd 2_cdb
 * The input tensor array must contain three tensors, the first of which is
 * rank two and the two next being rank 3. The graph must correspond to the
 * diagram (constructed for example by Graph("0ab1acd2cdb")).
 * If the diagram is not connected the result will be the sum of the
 * connected parts.
 */
cdouble compute(const Graph&, std::vector<Tensor>&);

}


#endif //PICHI_DIAGRAMS_H
