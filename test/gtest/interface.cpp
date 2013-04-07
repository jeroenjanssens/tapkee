#include <gtest/gtest.h>

#include <tapkee.hpp>
#include <tapkee_exceptions.hpp>

#include "callbacks.hpp"

TEST(Interface, NoDataPassThru) 
{
	std::vector<int> data;
	ASSERT_EQ(0,data.size());
	dummy_kernel_callback kcb;
	dummy_distance_callback dcb;
	dummy_feature_callback fcb;
	tapkee::ParametersMap params;
	params[tapkee::ReductionMethod] = tapkee::PassThru;
	params[tapkee::CurrentDimension] = static_cast<tapkee::IndexType>(1);

	tapkee::ReturnResult result;
	// should produce no error
	ASSERT_NO_THROW(result = tapkee::embed(data.begin(),data.end(),kcb,dcb,fcb,params));
	// that's normal
	ASSERT_EQ(1,result.first.cols());
	// that's normal
	ASSERT_EQ(0,result.first.rows());
	// that's normal
	ASSERT_EQ(NULL,result.second.implementation);
}

TEST(Interface, WrongParameterTypePassThru) 
{
	std::vector<int> data;
	ASSERT_EQ(0,data.size());
	dummy_kernel_callback kcb;
	dummy_distance_callback dcb;
	dummy_feature_callback fcb;
	tapkee::ParametersMap params;
	params[tapkee::ReductionMethod] = tapkee::PassThru;
	// here is the error (should be static_cast<tapkee::IndexType>(1))
	params[tapkee::CurrentDimension] = static_cast<tapkee::ScalarType>(1.0);

	tapkee::ReturnResult result;
	// fails with wrong parameter type as '1.0' is not of tapkee::IndexType
	ASSERT_THROW(result = tapkee::embed(data.begin(),data.end(),kcb,dcb,fcb,params), tapkee::wrong_parameter_type_error);
}

TEST(Interface, WrongParameterValuePassThru) 
{
	std::vector<int> data;
	ASSERT_EQ(0,data.size());
	dummy_kernel_callback kcb;
	dummy_distance_callback dcb;
	dummy_feature_callback fcb;
	tapkee::ParametersMap params;
	params[tapkee::ReductionMethod] = tapkee::PassThru;
	// here is the error (should be positive)
	params[tapkee::CurrentDimension] = static_cast<tapkee::IndexType>(-1);

	tapkee::ReturnResult result;
	// fails with wrong parameter type as '-1' is not a valid value.
	ASSERT_THROW(result = tapkee::embed(data.begin(),data.end(),kcb,dcb,fcb,params), tapkee::wrong_parameter_error);
}

bool always_cancel()
{
	return true;
}

TEST(Interface, CancellationPassThru)
{
	std::vector<int> data;
	ASSERT_EQ(0,data.size());
	dummy_kernel_callback kcb;
	dummy_distance_callback dcb;
	dummy_feature_callback fcb;
	tapkee::ParametersMap params;
	params[tapkee::ReductionMethod] = tapkee::PassThru;
	params[tapkee::CurrentDimension] = static_cast<tapkee::IndexType>(1);
	params[tapkee::CancelFunction] = static_cast<bool (*)()>(always_cancel);

	tapkee::ReturnResult result;
	// fails with wrong parameter type as '-1' is not a valid value.
	ASSERT_THROW(result = tapkee::embed(data.begin(),data.end(),kcb,dcb,fcb,params), tapkee::cancelled_exception);
}

TEST(Interface, NoReductionMethodSetFailPassThru)
{
	std::vector<int> data;
	ASSERT_EQ(0,data.size());
	dummy_kernel_callback kcb;
	dummy_distance_callback dcb;
	dummy_feature_callback fcb;
	tapkee::ParametersMap params;

	tapkee::ReturnResult result;
	// should fail with missed parameter
	ASSERT_THROW(result = tapkee::embed(data.begin(),data.end(),kcb,dcb,fcb,params), tapkee::missed_parameter_error);
}

TEST(Interface, NoCurrentDimensionSetFailPassThru) 
{
	std::vector<int> data;
	ASSERT_EQ(0,data.size());
	dummy_kernel_callback kcb;
	dummy_distance_callback dcb;
	dummy_feature_callback fcb;
	tapkee::ParametersMap params;
	params[tapkee::ReductionMethod] = tapkee::PassThru;
	// no current dimension set which is required

	tapkee::ReturnResult result;
	ASSERT_THROW(result = tapkee::embed(data.begin(),data.end(),kcb,dcb,fcb,params), tapkee::missed_parameter_error);
}

TEST(Interface, WrongMethodParameterType) 
{
	std::vector<int> data;
	ASSERT_EQ(0,data.size());
	dummy_kernel_callback kcb;
	dummy_distance_callback dcb;
	dummy_feature_callback fcb;
	tapkee::ParametersMap params;
	params[tapkee::ReductionMethod] = "diamonds are forever";
	params[tapkee::CurrentDimension] = static_cast<tapkee::IndexType>(1);

	tapkee::ReturnResult result;
	// fails with wrong parameter type as '1.0' is not of tapkee::IndexType
	ASSERT_THROW(result = tapkee::embed(data.begin(),data.end(),kcb,dcb,fcb,params), tapkee::wrong_parameter_type_error);
}

TEST(Interface, UnsupportedRandomizedForGeneralizedLE)
{
	std::vector<int> data;
	for (int i=0; i<10; i++) 
		data.push_back(i);
	
	dummy_kernel_callback kcb;
	float_distance_callback dcb;
	dummy_feature_callback fcb;
	tapkee::ParametersMap params;
	params[tapkee::ReductionMethod] = tapkee::LaplacianEigenmaps;
	params[tapkee::EigenEmbeddingMethod] = tapkee::Randomized;
	params[tapkee::NeighborsMethod] = tapkee::Brute;
	params[tapkee::GaussianKernelWidth] = static_cast<tapkee::ScalarType>(1.0);
	params[tapkee::TargetDimension] = static_cast<tapkee::IndexType>(1); 
	params[tapkee::NumberOfNeighbors] = static_cast<tapkee::IndexType>(5);

	tapkee::ReturnResult result;
	ASSERT_THROW(result = tapkee::embed(data.begin(),data.end(),kcb,dcb,fcb,params), tapkee::unsupported_method_error);
}

TEST(Interface, EigenDecompositionFailMDS)
{
	std::vector<int> data;
	for (int i=0; i<10; i++) 
		data.push_back(i);
	
	dummy_kernel_callback kcb;
	dummy_distance_callback dcb;
	dummy_feature_callback fcb;
	tapkee::ParametersMap params;
	params[tapkee::ReductionMethod] = tapkee::MultidimensionalScaling;
	params[tapkee::EigenEmbeddingMethod] = tapkee::Randomized;
	params[tapkee::TargetDimension] = static_cast<tapkee::IndexType>(3); 

	tapkee::ReturnResult result;
	ASSERT_THROW(result = tapkee::embed(data.begin(),data.end(),kcb,dcb,fcb,params), tapkee::eigendecomposition_error);
}

TEST(Interface, NotEnoughMemoryMDS)
{
	std::vector<int> data;
	for (int i=0; i<10000000; i++) 
		data.push_back(i);
	
	dummy_kernel_callback kcb;
	dummy_distance_callback dcb;
	dummy_feature_callback fcb;
	tapkee::ParametersMap params;
	params[tapkee::ReductionMethod] = tapkee::MultidimensionalScaling;
	params[tapkee::EigenEmbeddingMethod] = tapkee::Dense;
	params[tapkee::TargetDimension] = static_cast<tapkee::IndexType>(1); 

	tapkee::ReturnResult result;
	// tries to form 10000000 x 10000000 matrix (won't work on any machine in 2013)
	ASSERT_THROW(result = tapkee::embed(data.begin(),data.end(),kcb,dcb,fcb,params), tapkee::not_enough_memory_error);
}
