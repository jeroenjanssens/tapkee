/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Copyright (c) 2012, Sergey Lisitsyn, Fernando J. Iglesias García
 *
 * This code uses Any type developed by C. Diggins under Boost license, version 1.0.
 * http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef TAPKEE_DEFINES_H_
#define TAPKEE_DEFINES_H_

#include "utils/any.hpp"
#include <map>
#include <vector>
#include <utility>

#ifndef TAPKEE_DEBUG
	#define EIGEN_NO_DEBUG
#endif
//#define EIGEN_MATRIXBASE_PLUGIN "utils/matrix.hpp"
//#undef EIGEN_MATRIXBASE_PLUGIN
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Sparse>
#include <eigen3/Eigen/SparseCholesky>
//#include <eigen3/Eigen/SuperLUSupport>

// Customizable types
#ifdef TAPKEE_CUSTOM_TYPES
	#include TAPKEE_CUSTOM_TYPES
#else
	typedef double DefaultScalarType;
	typedef Eigen::Matrix<DefaultScalarType,Eigen::Dynamic,1> DenseVector;
	typedef Eigen::Matrix<DefaultScalarType,Eigen::Dynamic,Eigen::Dynamic> DenseMatrix;
	typedef DenseMatrix DenseSymmetricMatrix;
	typedef Eigen::SparseMatrix<DefaultScalarType> SparseWeightMatrix;
	typedef Eigen::SelfAdjointEigenSolver<DenseMatrix> DefaultDenseSelfAdjointEigenSolver;
	typedef Eigen::SimplicialLDLT<SparseWeightMatrix> DefaultSparseSolver;
#endif

#ifdef TAPKEE_CUSTOM_PROPERTIES
	#include TAPKEE_CUSTOM_PROPERTIES
#else
	#define COVERTREE_BASE 1.3
#endif

/** Parameters that are used by the library */
enum TAPKEE_PARAMETERS
{
	/* TAPKEE_METHOD */	REDUCTION_METHOD,
	/* unsigned int */ NUMBER_OF_NEIGHBORS,
	/* unsigned int */ TARGET_DIMENSION,
	/* unsigned int */ CURRENT_DIMENSION,
	/* TAPKEE_EIGEN_EMBEDDING_METHOD */ EIGEN_EMBEDDING_METHOD,
	/* TAPKEE_NEIGHBORS_METHOD */ NEIGHBORS_METHOD,
	/* unsigned int */ DIFFUSION_MAP_TIMESTEPS,
	/* DefaultScalarType */ GAUSSIAN_KERNEL_WIDTH,
	/* unsigned int */ MAX_ITERATION,
	/* bool */ SPE_GLOBAL_STRATEGY,
	/* DefaultScalarType */ SPE_TOLERANCE,
	/* unsigned int */ SPE_NUM_UPDATES,
	/* DefaultScalarType */ LANDMARK_RATIO,
	/* DefaultScalarType */ EIGENSHIFT
};
/** Parameters map type */
typedef std::map<TAPKEE_PARAMETERS, any> ParametersMap;

/** Dimension reduction method */
enum TAPKEE_METHOD
{
	/** Locally Linear Embedding / Kernel Locally Linear Embedding
	 * Requires Mercer kernel callback (linear kernel makes the algorithm a 
	 * standard Locally Linear Embedding) and number of neighbors 
	 * @enum TAPKEE_PARAMETERS::NUMBER_OF_NEIGHBORS. 
	 *
	 * References:
	 *
	 * @cite Saul, L. K., Ave, P., Park, F., & Roweis, S. T. (2001).
	 *      An Introduction to Locally Linear Embedding. 290(5500), 2323-2326.
	 * @cite Decoste, D. (2001). Visualizing Mercer Kernel Feature Spaces 
	 *      Via Kernelized Locally-Linear Embeddings.
	 *      The 8th International Conference on Neural Information Processing ICONIP2001
	 * @cite Zhao, D. (2006). Formulating LLE using alignment technique.
	 *      Pattern Recognition, 39(11), 2233-2235.
	 *
	 */
	KERNEL_LOCALLY_LINEAR_EMBEDDING,
	/**
	 *
	 *
	 *
	 */
	NEIGHBORHOOD_PRESERVING_EMBEDDING,
	/** Local Tangent Space Alignment / Kernel Local Tangent Space Alignment
	 * Requires Mercer kernel callback (linear kernel makes the algorithm a 
	 * standard Local Tangent Space Alignment) and number of neighbors 
	 * @enum TAPKEE_PARAMETERS::NUMBER_OF_NEIGHBORS. 
	 * 
	 * References:
	 *
	 * Zhang, Z., & Zha, H. (2002). Principal Manifolds
	 * and Nonlinear Dimension Reduction via Local Tangent Space Alignment.
	 * Journal of Shanghai University English Edition, 8(4), 406-424. SIAM.
	 *
	 */
	KERNEL_LOCAL_TANGENT_SPACE_ALIGNMENT,
	/**
	 *
	 *
	 *
	 */
	LINEAR_LOCAL_TANGENT_SPACE_ALIGNMENT,
	/**
	 *
	 *
	 *
	 */
	HESSIAN_LOCALLY_LINEAR_EMBEDDING,
	/**
	 *
	 *
	 *
	 */
	LAPLACIAN_EIGENMAPS,
	/**
	 *
	 *
	 *
	 */
	LOCALITY_PRESERVING_PROJECTIONS,
	/**
	 *
	 *
	 *
	 */
	DIFFUSION_MAP,
	/**
	 *
	 *
	 *
	 */
	ISOMAP,
	/**
	 *
	 *
	 *
	 */
	LANDMARK_ISOMAP,
	/**
	 *
	 *
	 *
	 */
	MULTIDIMENSIONAL_SCALING,
	/**
	 *
	 *
	 *
	 */
	LANDMARK_MULTIDIMENSIONAL_SCALING,
	/**
	 *
	 *
	 *
	 */
	STOCHASTIC_PROXIMITY_EMBEDDING,
	/**
	 *
	 *
	 *
	 */
	MAXIMUM_VARIANCE_UNFOLDING,
	/**
	 *
	 *
	 *
	 */
	KERNEL_PCA,
	/**
	 *
	 *
	 *
	 */
	PCA
};

/** Neighbors computation method */
enum TAPKEE_NEIGHBORS_METHOD
{
	/** Brute force method with approx. O(N*N*log k) time complexity.
	 * Recommended to be used only in debug purposes.
	 */
	BRUTE_FORCE,
	/** Covertree-based method with approx. O(log N) time complexity.
	 * Recommended to be used as a default method.
	 */
	COVER_TREE,
};

/** Eigendecomposition-based embedding method */
enum TAPKEE_EIGEN_EMBEDDING_METHOD
{
	/** ARPACK-based method (requires the ARPACK library
	 * binaries to be available around). 
	 * Recommended to be used as a default method.
	 * Supports both generalized and standard eigenproblems. */
	ARPACK_XSXUPD,
	/** Randomized method (implementation taken from the redsvd lib). 
	 * Supports only standard eigenproblems. */
	RANDOMIZED_INVERSE,
	/** Eigen library dense method (useful for debugging)
	 */
	EIGEN_DENSE_SELFADJOINT_SOLVER
};

/** Internal types */
typedef Eigen::Triplet<DefaultScalarType> SparseTriplet;
typedef std::vector<SparseTriplet> SparseTriplets;
typedef std::vector<unsigned int> LocalNeighbors;
typedef std::vector<LocalNeighbors> Neighbors;
typedef std::pair<DenseMatrix,DenseVector> EmbeddingResult;
typedef std::pair<DenseMatrix,DenseVector> ProjectionResult;
typedef Eigen::DiagonalMatrix<DefaultScalarType,Eigen::Dynamic> DenseDiagonalMatrix;
typedef std::vector<unsigned int> Landmarks;

#include "callbacks/traits.hpp"

#endif
