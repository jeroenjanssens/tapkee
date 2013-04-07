/* This software is distributed under BSD 3-clause license (see LICENSE file).
 *
 * Copyright (c) 2012-2013 Sergey Lisitsyn, Fernando Iglesias 
 */

#ifndef TAPKEE_APP_UTIL_H_
#define TAPKEE_APP_UTIL_H_

#include <istream>
#include <fstream>
#include <ostream>
#include <iterator>

using namespace std;

inline bool is_wrong_char(char c) {
	if (!(isdigit(c) || isspace(c) || c == '.' || c == '-' || c == '+' || c == 'e'))
	{
		return true;
	}
	return false;
} 

// TODO this absolutely unexceptionally definitive should be improved later
tapkee::DenseMatrix read_data(ifstream& ifs)
{
	string str;
	vector< vector<tapkee::ScalarType> > input_data;
	while (!ifs.eof())
	{
		getline(ifs,str);

		if (find_if(str.begin(), str.end(), is_wrong_char) != str.end())
			throw std::runtime_error("Input file contains some junk, please check it");

		if (str.size())
		{
			stringstream strstr(str);
			istream_iterator<tapkee::ScalarType> it(strstr);
			istream_iterator<tapkee::ScalarType> end;
			vector<tapkee::ScalarType> row(it, end);
			input_data.push_back(row);
		}
	}

	tapkee::DenseMatrix fm(input_data.size(),input_data[0].size());
	for (int i=0; i<fm.rows(); i++)
	{
		if (static_cast<tapkee::DenseMatrix::Index>(input_data[i].size()) != fm.cols()) 
		{
			stringstream ss;
			ss << "Wrong data at line " << i;
			throw std::runtime_error(ss.str());
		}
		for (int j=0; j<fm.cols(); j++)
			fm(i,j) = input_data[i][j];
	}
	return fm;
}

bool method_needs_kernel(tapkee::MethodId method) 
{
	switch (method)
	{
#define IF_NEEDS_KERNEL(X) case X : return tapkee::MethodTraits<X>::needs_kernel(); break;
		IF_NEEDS_KERNEL(tapkee::KernelLocalTangentSpaceAlignment);
		IF_NEEDS_KERNEL(tapkee::KernelLocallyLinearEmbedding);
		IF_NEEDS_KERNEL(tapkee::HessianLocallyLinearEmbedding);
		IF_NEEDS_KERNEL(tapkee::MultidimensionalScaling);
		IF_NEEDS_KERNEL(tapkee::LandmarkMultidimensionalScaling);
		IF_NEEDS_KERNEL(tapkee::Isomap);
		IF_NEEDS_KERNEL(tapkee::LandmarkIsomap);
		IF_NEEDS_KERNEL(tapkee::DiffusionMap);
		IF_NEEDS_KERNEL(tapkee::KernelPCA);
		IF_NEEDS_KERNEL(tapkee::PCA);
		IF_NEEDS_KERNEL(tapkee::RandomProjection);
		IF_NEEDS_KERNEL(tapkee::LaplacianEigenmaps);
		IF_NEEDS_KERNEL(tapkee::LocalityPreservingProjections);
		IF_NEEDS_KERNEL(tapkee::NeighborhoodPreservingEmbedding);
		IF_NEEDS_KERNEL(tapkee::LinearLocalTangentSpaceAlignment);
		IF_NEEDS_KERNEL(tapkee::StochasticProximityEmbedding);
		IF_NEEDS_KERNEL(tapkee::PassThru);
		IF_NEEDS_KERNEL(tapkee::FactorAnalysis);
		IF_NEEDS_KERNEL(tapkee::tDistributedStochasticNeighborEmbedding);
#undef IF_NEEDS_KERNEL
	}
	return false;
}

bool method_needs_distance(tapkee::MethodId method)
{
	switch (method)
	{
#define IF_NEEDS_DISTANCE(X) case X : return tapkee::MethodTraits<X>::needs_distance(); break;
		IF_NEEDS_DISTANCE(tapkee::KernelLocalTangentSpaceAlignment);
		IF_NEEDS_DISTANCE(tapkee::KernelLocallyLinearEmbedding);
		IF_NEEDS_DISTANCE(tapkee::HessianLocallyLinearEmbedding);
		IF_NEEDS_DISTANCE(tapkee::MultidimensionalScaling);
		IF_NEEDS_DISTANCE(tapkee::LandmarkMultidimensionalScaling);
		IF_NEEDS_DISTANCE(tapkee::Isomap);
		IF_NEEDS_DISTANCE(tapkee::LandmarkIsomap);
		IF_NEEDS_DISTANCE(tapkee::DiffusionMap);
		IF_NEEDS_DISTANCE(tapkee::KernelPCA);
		IF_NEEDS_DISTANCE(tapkee::PCA);
		IF_NEEDS_DISTANCE(tapkee::RandomProjection);
		IF_NEEDS_DISTANCE(tapkee::LaplacianEigenmaps);
		IF_NEEDS_DISTANCE(tapkee::LocalityPreservingProjections);
		IF_NEEDS_DISTANCE(tapkee::NeighborhoodPreservingEmbedding);
		IF_NEEDS_DISTANCE(tapkee::LinearLocalTangentSpaceAlignment);
		IF_NEEDS_DISTANCE(tapkee::StochasticProximityEmbedding);
		IF_NEEDS_DISTANCE(tapkee::PassThru);
		IF_NEEDS_DISTANCE(tapkee::FactorAnalysis);
		IF_NEEDS_DISTANCE(tapkee::tDistributedStochasticNeighborEmbedding);
#undef IF_NEEDS_DISTANCE
	}
	return false;
}

tapkee::MethodId parse_reduction_method(const char* str)
{
	if (!strcmp(str,"local_tangent_space_alignment") || !strcmp(str,"ltsa"))
		return tapkee::KernelLocalTangentSpaceAlignment;
	if (!strcmp(str,"locally_linear_embedding") || !strcmp(str,"lle"))
		return tapkee::KernelLocallyLinearEmbedding;
	if (!strcmp(str,"hessian_locally_linear_embedding") || !strcmp(str,"hlle"))
		return tapkee::HessianLocallyLinearEmbedding;
	if (!strcmp(str,"multidimensional_scaling") || !strcmp(str,"mds"))
		return tapkee::MultidimensionalScaling;
	if (!strcmp(str,"landmark_multidimensional_scaling") || !strcmp(str,"l-mds"))
		return tapkee::LandmarkMultidimensionalScaling;
	if (!strcmp(str,"isomap"))
		return tapkee::Isomap;
	if (!strcmp(str,"landmark_isomap") || !strcmp(str,"l-isomap"))
		return tapkee::LandmarkIsomap;
	if (!strcmp(str,"diffusion_map") || !strcmp(str,"dm"))
		return tapkee::DiffusionMap;
	if (!strcmp(str,"kernel_pca") || !strcmp(str,"kpca"))
		return tapkee::KernelPCA;
	if (!strcmp(str,"pca"))
		return tapkee::PCA;
	if (!strcmp(str,"random_projection") || !strcmp(str,"ra"))
		return tapkee::RandomProjection;
	if (!strcmp(str,"laplacian_eigenmaps") || !strcmp(str,"la"))
		return tapkee::LaplacianEigenmaps;
	if (!strcmp(str,"locality_preserving_projections") || !strcmp(str,"lpp"))
		return tapkee::LocalityPreservingProjections;
	if (!strcmp(str,"neighborhood_preserving_embedding") || !strcmp(str,"npe"))
		return tapkee::NeighborhoodPreservingEmbedding;
	if (!strcmp(str,"linear_local_tangent_space_alignment") || !strcmp(str,"lltsa"))
		return tapkee::LinearLocalTangentSpaceAlignment;
	if (!strcmp(str,"stochastic_proximity_embedding") || !strcmp(str,"spe"))
		return tapkee::StochasticProximityEmbedding;
	if (!strcmp(str,"passthru"))
		return tapkee::PassThru;
	if (!strcmp(str,"factor_analysis") || !strcmp(str,"fa"))
		return tapkee::FactorAnalysis;
	if (!strcmp(str,"t-stochastic_neighbor_embedding") || !strcmp(str,"t-sne"))
		return tapkee::tDistributedStochasticNeighborEmbedding;
	
	throw std::exception();
	return tapkee::PassThru;
}

tapkee::NeighborsMethodId parse_neighbors_method(const char* str)
{
	if (!strcmp(str,"brute"))
		return tapkee::Brute;
#ifdef TAPKEE_USE_LGPL_COVERTREE
	if (!strcmp(str,"covertree"))
		return tapkee::CoverTree;
#endif

	throw std::exception();
	return tapkee::Brute;
}

tapkee::EigenEmbeddingMethodId parse_eigen_method(const char* str)
{
#ifdef TAPKEE_WITH_ARPACK
	if (!strcmp(str,"arpack"))
		return tapkee::Arpack;
#endif
	if (!strcmp(str,"randomized"))
		return tapkee::Randomized;
	if (!strcmp(str,"dense"))
		return tapkee::Dense;
	
	throw std::exception();
	return tapkee::Dense;
}

template <class PairwiseCallback>
tapkee::DenseMatrix matrix_from_callback(const tapkee::IndexType N, PairwiseCallback callback)
{
	tapkee::DenseMatrix result(N,N);
	tapkee::IndexType i,j;
#pragma omp parallel for shared(callback,result) private(j) default(none)
	for (i=0; i<N; ++i)
	{
		for (j=i; j<N; j++)
		{
			tapkee::ScalarType res = callback(i,j);
			result(i,j) = res;
			result(j,i) = res;
		}
	}
	return result;
}

#endif
