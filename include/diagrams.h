
#ifndef PICHI_DIAGRAMS_H
#define PICHI_DIAGRAMS_H

#include "contraction.h"

namespace pichi {

/* ************************************************************************
 *
 * This file declares the optimal way of computing diagrams that are
 * important for hadron spectroscopy. For processes including 1-1, 2-1 and
 * 2-2 scattering with mesons and baryons, there are only 8 topologically
 * distinct diagrams we need to compute. The functionality declared in this
 * file makes it easy to compute these diagrams in the most optimal way.
 *
 * A diagram in this context is a set of tensors and a corresponding set of
 * index strings. For example, we could imagine the diagram
 *
 * 1 : "abc"
 * 2 : "abd"
 * 3 : "cd"
 *
 * where the numbers correspond to a tensor and the strings tell us how the
 * indices should be contracted. Of course, indices can be permuted or
 * relabelled without meaningfully changing the diagram, i.e. the diagram
 *
 * 1 : "gcb"
 * 2 : "bd"
 * 3 : "gdc"
 *
 * is topologically the same diagram as the one before, but will of course
 * yield a different result. This is all handled by the evaluating functions.
 *
 * The highest level function in this collection is the "compute" function,
 * which takes as input a diagrammatic string expression and converts it to a
 * (set of) diagram(s) that are computed. For example, consider a list of
 * 100 tensors computed from earlier data analysis. We can then do the following
 *
 * compute("34ab72acd12bcd52ee", tensors);
 *
 * where "tensors" is the list of tensors. That call will compute the
 * contraction of the tensors with index 34 (rank 2), 72 and 12 (both rank 3)
 * according to the rules of diagram 5 and the diagram 0 contraction of the
 * rank 2 tensor with index 52. The two resulting numbers are added together
 * and delivered as a result.
 *
 * This file defines the struct DiagramNode, which just contains a pointer to
 * a tensor and a string. A diagram is then a list of DiagramNodes,
 * essentially yielding a graph.
 *
 * For an overview of the diagrams, see the documentation --> benchmark
 * document.
 *
 *
 * ***********************************************************************/


/*
 * Computes a diagrammatic contraction expression. Takes as input a
 * contraction expression which can contain an arbitrary number of
 * disconnected diagrams. The other argument is a list of the tensors that
 * are used for the contractions.
 * A contraction string could be
 * "0ab1bc2cd4ad"
 * which is a single connected diagram (diagram 3) consisting of four rank 2
 * tensors with index 0, 1, 2 and 4 in the input collection.
 * An example of disconnected diagrams could be
 * "0abc1abc2dd3ee"
 * which consists of one diagram 4 expression ("0abc1abc") and two diagram 0
 * expressions ("2dd" and "3ee").
 * The return value is the result of all of the connected diagrams added
 * together.
 */
cdouble compute(std::string, std::vector<Tensor>& tensors);

/*
 * A container for a tensor and its indices.
 */
struct DiagramNode {
  Tensor* t;
  std::string idx;
};

/*
 * Computes the diagram A_aa
 */
cdouble diagram0(std::vector<DiagramNode>& nodes);


/*
 * Computes diagrams topologically equivalent to A_ab B_ab. In this case
 * there are only two possibilities:
 *   A_ab B_ab,  or
 *   A_ab B_ba
 *
 * but NOT
 *   A_aa B_bb (Disconnected diagrams)
 */
cdouble diagram1(std::vector<DiagramNode>& nodes);


/*
 * Computes diagrams topologically equivalent to A_ab B_bc C_ac.
 * Examples:
 *   A_ab B_bc C_ac
 *   A_ab B_cb C_ac
 *   A_ab B_ca C_bc
 *
 *   but NOT
 *   A_ab B_ba C_cc (Disconnected diagrams)
 */
cdouble diagram2(std::vector<DiagramNode>& nodes);


/*
 * Computes diagrams topologically equivalent to A_ab B_bc C_cd D_ad.
 * Examples:
 *   A_ab B_bc C_cd D_ad
 *   A_ab B_ac C_bd D_dc
 *   A_ab B_cd C_bc D_ad
 *
 *   but NOT
 *   A_ab B_bc C_ca D_dd (Disconnected diagrams)
 */
cdouble diagram3(std::vector<DiagramNode>& nodes);


/*
 * Computes diagrams topologically equivalent to A_abc B_abc.
 * Examples:
 *   A_abc B_abc
 *   A_acb B_bac
 *
 *   but NOT
 *   A_aab B_bcc (A and B must be connected by 3 indices)
 */
cdouble diagram4(std::vector<DiagramNode>& nodes);

/*
 * Computes diagrams topologically equivalent to A_abc B_abd C_cd.
 * Examples:
 *   A_abc B_abd C_cd
 *   A_ab B_cdb C_cad
 *
 *   but NOT
 *   A_abc B_abc C_cc (Disconnected diagrams)
 *   A_abc B_add C_bc (A and B must share two indices, and C must share one
 *                     index with both A and B)
 */
cdouble diagram5(std::vector<DiagramNode>& nodes);

/*
 * Computes diagrams topologically equivalent to A_abc B_abd C_ce D_de.
 * Examples:
 *   A_abc B_abd C_ce D_de
 *   A_abc B_dba C_de D_ec
 *
 *
 *   but NOT
 *   A_abc B_abc C_de D_ed (Disconnected diagrams)
 *   A_abc B_ade C_bd D_ce (A and B must share two indices, they must each
 *                          only be connected to one of C and D, and C and D
 *                          must share an index)
 */
cdouble diagram6(std::vector<DiagramNode>& nodes);

/*
 * Computes diagrams topologically equivalent to A_abc B_ade C_bd D_ce.
 * Examples:
 *   A_abc B_ade C_bd D_ce
 *   A_abc B_bd C_ade D_ec
 *
 *
 *   but NOT
 *   A_abc B_abc C_de D_ed (Disconnected diagrams)
 *   A_abc B_abd C_ce D_de (A and B must share one index, they must both
 *                          be connected to both C and D, and C and D
 *                          may not have shared indices)
 */
cdouble diagram7(std::vector<DiagramNode>& nodes);

/*
 * Computes diagrams topologically equivalent to A_abc B_abd C_def D_cef.
 * Examples:
 *   A_abc B_abd C_def D_cef
 *   A_abc B_acd C_fed D_fec
 *
 *
 *   but NOT
 *   A_abc B_abc C_def D_edf (Disconnected diagrams)
 *   A_aab B_bcd C_cee D_dff (A must be connected to both B and e.g. D, and B
 *                            cannot be connected to all other tensors, only 2).
 */
cdouble diagram8(std::vector<DiagramNode>& nodes);



}


#endif //PICHI_DIAGRAMS_H
