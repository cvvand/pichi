
#ifndef PICHI_DIAGRAMS_H
#define PICHI_DIAGRAMS_H

#include "contraction.h"

namespace pichi {

/* ************************************************************************
 *
 * This file declares the contraction diagrams that are important for hadron
 * spectroscopy. Including only processes involving a maximum of four hadrons
 * in total and only diagrams with mesons and baryons, there are 8
 * topologically distinct diagrams that we might want to compute. Other
 * disconnected diagrams also exist, but those will be sums of diagrams
 * declared here or diagrams that have no basis in quantum field theory.
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
 * This file defines the struct DiagramNode, which just contains a pointer to
 * a tensor and a string. A diagram is then a list of DiagramNodes,
 * essentially yielding a graph.
 *
 * All diagrams are computed in the most efficient way with respect to runtime.
 *
 * For an overview of the diagrams, see the documentation --> benchmark
 * document.
 *
 *
 * ***********************************************************************/


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
