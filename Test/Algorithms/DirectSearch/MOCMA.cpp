/**
 *
 *  \brief Unit tests for class shark::(detail::)MOCMA.
 *
 *  \author T.Voss, T. Glasmachers, O.Krause
 *  \date 2010-2011
 *
 *  \par Copyright (c) 1998-2011:
 *      Institut f&uuml;r Neuroinformatik<BR>
 *      Ruhr-Universit&auml;t Bochum<BR>
 *      D-44780 Bochum, Germany<BR>
 *      Phone: +49-234-32-25558<BR>
 *      Fax:   +49-234-32-14209<BR>
 *      eMail: Shark-admin@neuroinformatik.ruhr-uni-bochum.de<BR>
 *      www:   http://www.neuroinformatik.ruhr-uni-bochum.de<BR>
 *      <BR>
 *
 *
 *  <BR><HR>
 *  This file is part of Shark. This library is free software;
 *  you can redistribute it and/or modify it under the terms of the
 *  GNU General Public License as published by the Free Software
 *  Foundation; either version 3, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this library; if not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <iostream>
#include <boost/numeric/ublas/io.hpp>
#define BOOST_TEST_MODULE DirectSearch_CMA
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <shark/Algorithms/DirectSearch/MOCMA.h>
#include <shark/Algorithms/DirectSearch/InterruptibleAlgorithmRunner.h>

#include <shark/ObjectiveFunctions/AbstractMultiObjectiveFunction.h>
#include <shark/ObjectiveFunctions/Benchmarks/Benchmarks.h>

// #include <shark/Core/Probe.h>


/*
BOOST_AUTO_TEST_CASE( ProbeFramework ) {
	shark::ProbeProvider< shark::detail::MOCMA<> > probeProvider;
	shark::ProbeProvider< shark::detail::MOCMA< shark::AdditiveEpsilonIndicator > > probeProvider1;

	std::vector<
		boost::shared_ptr<
			shark::Probe
		>
	> probes( 30 );

	for( unsigned int i = 0; i < 15; i++ ) {
		probes[ i ] = probeProvider.registerProbe( ( boost::format( "Probe_%1%" ) % i ).str(), "Description" );
	}

	for( unsigned int i = 15; i < 30; i++ ) {
		probes[ i ] = probeProvider1.registerProbe( ( boost::format( "Probe_%1%" ) % i ).str(), "Description" );
	}

	std::list< 
		boost::shared_ptr< shark::Probe > 
	> mocmaProbesForInstance = shark::ProbeManager::instance().find( boost::regex( "/" + probeProvider.context() + "/.*" ) );

	for( std::list< 
		boost::shared_ptr< shark::Probe > 
	>::iterator it = mocmaProbesForInstance.begin(); it != mocmaProbesForInstance.end(); ++it )
		std::clog << (*it)->name() << std::endl;

	shark::ProbeManager::instance().print( std::cout );
}*/


BOOST_AUTO_TEST_CASE( ApproximatedHypMOCMA_Serialization ) {

	shark::mocma::Chromosome chromosome1, chromosome2;
	BOOST_CHECK(			chromosome1.m_mutationDistribution.covarianceMatrix().size1() == 0 );
	BOOST_CHECK(			chromosome1.m_mutationDistribution.covarianceMatrix().size2() == 0 );
	BOOST_CHECK(			chromosome1.m_mutationDistribution.eigenValues().size() == 0 );
	BOOST_CHECK(			chromosome1.m_mutationDistribution.eigenVectors().size1() == 0 );
	BOOST_CHECK(			chromosome1.m_mutationDistribution.eigenVectors().size2() == 0 );
	BOOST_CHECK(			chromosome1.m_evolutionPath.size() == 0 );
	BOOST_CHECK(			chromosome1.m_lastStep.size() == 0 );
	BOOST_CHECK(			chromosome1.m_lambda == 0 );
	BOOST_CHECK_EQUAL(		chromosome1.m_noSuccessfulOffspring, 0. );
	BOOST_CHECK_EQUAL(		chromosome1.m_stepSize, 0. );
	BOOST_CHECK_EQUAL(		chromosome1.m_stepSizeDampingFactor, 0. );
	BOOST_CHECK_EQUAL(		chromosome1.m_stepSizeLearningRate, 0. );
	BOOST_CHECK_EQUAL(		chromosome1.m_successProbability, 0. );
	BOOST_CHECK_EQUAL(		chromosome1.m_targetSuccessProbability, 0. );
	BOOST_CHECK_EQUAL(		chromosome1.m_evolutionPathLearningRate, 0. );
	BOOST_CHECK_EQUAL(		chromosome1.m_covarianceMatrixLearningRate, 0. );
	BOOST_CHECK_EQUAL(		chromosome1.m_needsCovarianceUpdate, false );
	BOOST_CHECK(			chromosome1.mep_parent == NULL );

	chromosome1.m_mutationDistribution.resize( 10 );
	chromosome1.m_evolutionPath = boost::numeric::ublas::zero_vector<double>( 10 );
	chromosome1.m_lastStep = boost::numeric::ublas::zero_vector<double>( 10 );
	chromosome1.m_lambda = 5;
	chromosome1.m_noSuccessfulOffspring = 5.;
	chromosome1.m_stepSize = 5.;
	chromosome1.m_stepSizeDampingFactor = 5.;
	chromosome1.m_stepSizeLearningRate = 5.;
	chromosome1.m_successProbability = 5.;
	chromosome1.m_targetSuccessProbability = 5.;
	chromosome1.m_evolutionPathLearningRate = 5.;
	chromosome1.m_covarianceMatrixLearningRate = 5.;
	chromosome1.m_needsCovarianceUpdate = true;
	
	{
		std::stringstream ss;
		boost::archive::text_oarchive oa( ss );
		BOOST_CHECK_NO_THROW( (oa << chromosome1) );

		boost::archive::text_iarchive ia( ss );
		BOOST_CHECK_NO_THROW( (ia >> chromosome2) );

		BOOST_CHECK( chromosome1 == chromosome2 );
	}

	shark::mocma::Initializer initializer1, initializer2;
	BOOST_CHECK_EQUAL( initializer1.m_searchSpaceDimension, 0 );
	BOOST_CHECK_EQUAL( initializer1.m_noObjectives, 0 );
	BOOST_CHECK_CLOSE( initializer1.m_initialSigma, 0., 1E-10 );
	BOOST_CHECK_EQUAL( initializer1.m_useNewUpdate, false );
	BOOST_CHECK_EQUAL( initializer1.m_constrainedFitnessFunction, false );

	BOOST_CHECK_THROW( initializer1( chromosome1 ), shark::Exception );
	initializer1.m_searchSpaceDimension = 5;
	BOOST_CHECK_THROW( initializer1( chromosome1 ), shark::Exception );
	initializer1.m_noObjectives = 5;
	BOOST_CHECK_NO_THROW( initializer1( chromosome1 ) );
	initializer1.m_initialSigma = 5;
	initializer1.m_useNewUpdate = true;
	initializer1.m_constrainedFitnessFunction = true;
	{
		std::stringstream ss;
		boost::archive::text_oarchive oa( ss );
		BOOST_CHECK_NO_THROW( (oa << initializer1) );

		boost::archive::text_iarchive ia( ss );
		BOOST_CHECK_NO_THROW( (ia >> initializer2) );

		BOOST_CHECK( initializer1 == initializer2 );
	}

	shark::PropertyTree node;
	shark::detail::MOCMA<> mocma;
	BOOST_CHECK_NO_THROW( mocma.init() );
	BOOST_CHECK_NO_THROW( shark::OptimizerTraits< shark::detail::MOCMA<> >::defaultConfig( node ) );
	BOOST_CHECK_NO_THROW( mocma.configure( node ) );

	//~ shark::AbstractMultiObjectiveFunction< shark::VectorSpace< double > > function;
	//~ BOOST_CHECK_THROW( mocma.init( function ), shark::Exception );
	shark::DTLZ1 dtlz1;
	dtlz1.setNoObjectives( 3 );
	dtlz1.setNumberOfVariables( 10 );
	BOOST_CHECK_NO_THROW( mocma.init( dtlz1 ) );
	BOOST_CHECK_NO_THROW( mocma.step( dtlz1 ) );
	
	{
		std::stringstream ss;
		boost::archive::text_oarchive oa( ss );

		BOOST_CHECK_NO_THROW( (oa << mocma) );

		shark::detail::MOCMA<> mocma2;

		boost::archive::text_iarchive ia( ss );
		BOOST_CHECK_NO_THROW( (ia >> mocma2) );

		shark::Rng::seed( 1 );
		shark::FastRng::seed( 1 );
		shark::detail::MOCMA<>::SolutionSetType set1 = mocma.step( dtlz1 );
		shark::Rng::seed( 1 );
		shark::FastRng::seed( 1 );
		shark::detail::MOCMA<>::SolutionSetType set2 = mocma2.step( dtlz1 );

		for( unsigned int i = 0; i < set1.size(); i++ ) {
			BOOST_CHECK_SMALL( boost::numeric::ublas::norm_2( set1.at( i ).value - set2.at( i ).value ), 1E-20 );
			BOOST_CHECK_SMALL( boost::numeric::ublas::norm_2( set1.at( i ).point- set2.at( i ).point), 1E-20 );
		}

	}


}

BOOST_AUTO_TEST_CASE( ExactHypMOCMA_Serialization ) {

	shark::mocma::Chromosome chromosome1, chromosome2;
	BOOST_CHECK(			chromosome1.m_mutationDistribution.covarianceMatrix().size1() == 0 );
	BOOST_CHECK(			chromosome1.m_mutationDistribution.covarianceMatrix().size2() == 0 );
	BOOST_CHECK(			chromosome1.m_mutationDistribution.eigenValues().size() == 0 );
	BOOST_CHECK(			chromosome1.m_mutationDistribution.eigenVectors().size1() == 0 );
	BOOST_CHECK(			chromosome1.m_mutationDistribution.eigenVectors().size2() == 0 );
	BOOST_CHECK(			chromosome1.m_evolutionPath.size() == 0 );
	BOOST_CHECK(			chromosome1.m_lastStep.size() == 0 );
	BOOST_CHECK(			chromosome1.m_lambda == 0 );
	BOOST_CHECK_EQUAL(		chromosome1.m_noSuccessfulOffspring, 0. );
	BOOST_CHECK_EQUAL(		chromosome1.m_stepSize, 0. );
	BOOST_CHECK_EQUAL(		chromosome1.m_stepSizeDampingFactor, 0. );
	BOOST_CHECK_EQUAL(		chromosome1.m_stepSizeLearningRate, 0. );
	BOOST_CHECK_EQUAL(		chromosome1.m_successProbability, 0. );
	BOOST_CHECK_EQUAL(		chromosome1.m_targetSuccessProbability, 0. );
	BOOST_CHECK_EQUAL(		chromosome1.m_evolutionPathLearningRate, 0. );
	BOOST_CHECK_EQUAL(		chromosome1.m_covarianceMatrixLearningRate, 0. );
	BOOST_CHECK_EQUAL(		chromosome1.m_needsCovarianceUpdate, false );
	BOOST_CHECK(			chromosome1.mep_parent == NULL );

	chromosome1.m_mutationDistribution.resize( 10 );
	chromosome1.m_evolutionPath = boost::numeric::ublas::zero_vector<double>( 10 );
	chromosome1.m_lastStep = boost::numeric::ublas::zero_vector<double>( 10 );
	chromosome1.m_lambda = 5;
	chromosome1.m_noSuccessfulOffspring = 5.;
	chromosome1.m_stepSize = 5.;
	chromosome1.m_stepSizeDampingFactor = 5.;
	chromosome1.m_stepSizeLearningRate = 5.;
	chromosome1.m_successProbability = 5.;
	chromosome1.m_targetSuccessProbability = 5.;
	chromosome1.m_evolutionPathLearningRate = 5.;
	chromosome1.m_covarianceMatrixLearningRate = 5.;
	chromosome1.m_needsCovarianceUpdate = true;

	{
		std::stringstream ss;
		boost::archive::text_oarchive oa( ss );
		BOOST_CHECK_NO_THROW( (oa << chromosome1) );

		boost::archive::text_iarchive ia( ss );
		BOOST_CHECK_NO_THROW( (ia >> chromosome2) );

		BOOST_CHECK( chromosome1 == chromosome2 );
	}

	shark::mocma::Initializer initializer1, initializer2;
	BOOST_CHECK_EQUAL( initializer1.m_searchSpaceDimension, 0 );
	BOOST_CHECK_EQUAL( initializer1.m_noObjectives, 0 );
	BOOST_CHECK_CLOSE( initializer1.m_initialSigma, 0., 1E-10 );
	BOOST_CHECK_EQUAL( initializer1.m_useNewUpdate, false );
	BOOST_CHECK_EQUAL( initializer1.m_constrainedFitnessFunction, false );

	BOOST_CHECK_THROW( initializer1( chromosome1 ), shark::Exception );
	initializer1.m_searchSpaceDimension = 5;
	BOOST_CHECK_THROW( initializer1( chromosome1 ), shark::Exception );
	initializer1.m_noObjectives = 5;
	BOOST_CHECK_NO_THROW( initializer1( chromosome1 ) );
	initializer1.m_initialSigma = 5;
	initializer1.m_useNewUpdate = true;
	initializer1.m_constrainedFitnessFunction = true;
	{
		std::stringstream ss;
		boost::archive::text_oarchive oa( ss );
		BOOST_CHECK_NO_THROW( (oa << initializer1) );

		boost::archive::text_iarchive ia( ss );
		BOOST_CHECK_NO_THROW( (ia >> initializer2) );

		BOOST_CHECK( initializer1 == initializer2 );
	}

	shark::PropertyTree node;
	shark::detail::MOCMA<> mocma;
	BOOST_CHECK_NO_THROW( mocma.init() );
	BOOST_CHECK_NO_THROW( shark::OptimizerTraits< shark::detail::MOCMA<> >::defaultConfig( node ) );
	BOOST_CHECK_NO_THROW( mocma.configure( node ) );

	//~ shark::AbstractMultiObjectiveFunction< shark::VectorSpace< double > > function;
	//~ BOOST_CHECK_THROW( mocma.init( function ), shark::Exception );
	mocma.m_useApproximatedHypervolume = false;
	shark::DTLZ1 dtlz1;
	dtlz1.setNoObjectives( 3 );
	dtlz1.setNumberOfVariables( 10 );
	BOOST_CHECK_NO_THROW( mocma.init( dtlz1 ) );
	BOOST_CHECK_NO_THROW( mocma.step( dtlz1 ) );

	{
		std::stringstream ss;
		boost::archive::text_oarchive oa( ss );

		BOOST_CHECK_NO_THROW( (oa << mocma) );

		shark::detail::MOCMA<> mocma2;

		boost::archive::text_iarchive ia( ss );
		BOOST_CHECK_NO_THROW( (ia >> mocma2) );

		shark::Rng::seed( 1 );
		shark::FastRng::seed( 1 );
		shark::detail::MOCMA<>::SolutionSetType set1 = mocma.step( dtlz1 );
		shark::Rng::seed( 1 );
		shark::FastRng::seed( 1 );
		shark::detail::MOCMA<>::SolutionSetType set2 = mocma2.step( dtlz1 );

		for( unsigned int i = 0; i < set1.size(); i++ ) {
			BOOST_CHECK_SMALL( boost::numeric::ublas::norm_2( set1.at( i ).value - set2.at( i ).value ), 1E-20 );
			BOOST_CHECK_SMALL( boost::numeric::ublas::norm_2( set1.at( i ).point - set2.at( i ).point ), 1E-20 );
		}

	}


}

BOOST_AUTO_TEST_CASE( AdditiveEpsMOCMA_Serialization ) {

	shark::mocma::Chromosome chromosome1, chromosome2;
	BOOST_CHECK(			chromosome1.m_mutationDistribution.covarianceMatrix().size1() == 0 );
	BOOST_CHECK(			chromosome1.m_mutationDistribution.covarianceMatrix().size2() == 0 );
	BOOST_CHECK(			chromosome1.m_mutationDistribution.eigenValues().size() == 0 );
	BOOST_CHECK(			chromosome1.m_mutationDistribution.eigenVectors().size1() == 0 );
	BOOST_CHECK(			chromosome1.m_mutationDistribution.eigenVectors().size2() == 0 );
	BOOST_CHECK(			chromosome1.m_evolutionPath.size() == 0 );
	BOOST_CHECK(			chromosome1.m_lastStep.size() == 0 );
	BOOST_CHECK(			chromosome1.m_lambda == 0 );
	BOOST_CHECK_EQUAL(		chromosome1.m_noSuccessfulOffspring, 0. );
	BOOST_CHECK_EQUAL(		chromosome1.m_stepSize, 0. );
	BOOST_CHECK_EQUAL(		chromosome1.m_stepSizeDampingFactor, 0. );
	BOOST_CHECK_EQUAL(		chromosome1.m_stepSizeLearningRate, 0. );
	BOOST_CHECK_EQUAL(		chromosome1.m_successProbability, 0. );
	BOOST_CHECK_EQUAL(		chromosome1.m_targetSuccessProbability, 0. );
	BOOST_CHECK_EQUAL(		chromosome1.m_evolutionPathLearningRate, 0. );
	BOOST_CHECK_EQUAL(		chromosome1.m_covarianceMatrixLearningRate, 0. );
	BOOST_CHECK_EQUAL(		chromosome1.m_needsCovarianceUpdate, false );
	BOOST_CHECK(			chromosome1.mep_parent == NULL );

	chromosome1.m_mutationDistribution.resize( 10 );
	chromosome1.m_evolutionPath = boost::numeric::ublas::zero_vector<double>( 10 );
	chromosome1.m_lastStep = boost::numeric::ublas::zero_vector<double>( 10 );
	chromosome1.m_lambda = 5;
	chromosome1.m_noSuccessfulOffspring = 5.;
	chromosome1.m_stepSize = 5.;
	chromosome1.m_stepSizeDampingFactor = 5.;
	chromosome1.m_stepSizeLearningRate = 5.;
	chromosome1.m_successProbability = 5.;
	chromosome1.m_targetSuccessProbability = 5.;
	chromosome1.m_evolutionPathLearningRate = 5.;
	chromosome1.m_covarianceMatrixLearningRate = 5.;
	chromosome1.m_needsCovarianceUpdate = true;

	{
		std::stringstream ss;
		boost::archive::text_oarchive oa( ss );
		BOOST_CHECK_NO_THROW( (oa << chromosome1) );

		boost::archive::text_iarchive ia( ss );
		BOOST_CHECK_NO_THROW( (ia >> chromosome2) );

		BOOST_CHECK( chromosome1 == chromosome2 );
	}

	shark::mocma::Initializer initializer1, initializer2;
	BOOST_CHECK_EQUAL( initializer1.m_searchSpaceDimension, 0 );
	BOOST_CHECK_EQUAL( initializer1.m_noObjectives, 0 );
	BOOST_CHECK_CLOSE( initializer1.m_initialSigma, 0., 1E-10 );
	BOOST_CHECK_EQUAL( initializer1.m_useNewUpdate, false );
	BOOST_CHECK_EQUAL( initializer1.m_constrainedFitnessFunction, false );

	BOOST_CHECK_THROW( initializer1( chromosome1 ), shark::Exception );
	initializer1.m_searchSpaceDimension = 5;
	BOOST_CHECK_THROW( initializer1( chromosome1 ), shark::Exception );
	initializer1.m_noObjectives = 5;
	BOOST_CHECK_NO_THROW( initializer1( chromosome1 ) );
	initializer1.m_initialSigma = 5;
	initializer1.m_useNewUpdate = true;
	initializer1.m_constrainedFitnessFunction = true;
	{
		std::stringstream ss;
		boost::archive::text_oarchive oa( ss );
		BOOST_CHECK_NO_THROW( (oa << initializer1) );

		boost::archive::text_iarchive ia( ss );
		BOOST_CHECK_NO_THROW( (ia >> initializer2) );

		BOOST_CHECK( initializer1 == initializer2 );
	}

	shark::PropertyTree node;
	shark::detail::MOCMA< shark::AdditiveEpsilonIndicator > mocma;
	BOOST_CHECK_NO_THROW( mocma.init() );
	BOOST_CHECK_NO_THROW( shark::OptimizerTraits< shark::detail::MOCMA<shark::AdditiveEpsilonIndicator> >::defaultConfig( node ) );
	BOOST_CHECK_NO_THROW( mocma.configure( node ) );

	//~ shark::AbstractMultiObjectiveFunction< shark::VectorSpace< double > > function;
	//~ BOOST_CHECK_THROW( mocma.init( function ), shark::Exception );
	mocma.m_useApproximatedHypervolume = false;
	shark::DTLZ1 dtlz1;
	dtlz1.setNoObjectives( 3 );
	dtlz1.setNumberOfVariables( 10 );
	BOOST_CHECK_NO_THROW( mocma.init( dtlz1 ) );
	BOOST_CHECK_NO_THROW( mocma.step( dtlz1 ) );

	{
		std::stringstream ss;
		boost::archive::text_oarchive oa( ss );

		BOOST_CHECK_NO_THROW( (oa << mocma) );

		shark::detail::MOCMA<shark::AdditiveEpsilonIndicator> mocma2;

		boost::archive::text_iarchive ia( ss );
		BOOST_CHECK_NO_THROW( (ia >> mocma2) );

		shark::Rng::seed( 1 );
		shark::FastRng::seed( 1 );
		shark::detail::MOCMA<shark::AdditiveEpsilonIndicator>::SolutionSetType set1 = mocma.step( dtlz1 );
		shark::Rng::seed( 1 );
		shark::FastRng::seed( 1 );
		shark::detail::MOCMA<shark::AdditiveEpsilonIndicator>::SolutionSetType set2 = mocma2.step( dtlz1 );

		for( unsigned int i = 0; i < set1.size(); i++ ) {
			BOOST_CHECK_SMALL( boost::numeric::ublas::norm_2( set1.at( i ).value - set2.at( i ).value ), 1E-20 );
			BOOST_CHECK_SMALL( boost::numeric::ublas::norm_2( set1.at( i ).point - set2.at( i ).point ), 1E-20 );
		}

	}

}
#ifdef NDEBUG
BOOST_AUTO_TEST_CASE( MOCMA_Performance ) {

	boost::shared_ptr< 
		shark::MOCMA
	> mocma( new shark::MOCMA() );
	
	shark::moo::InterruptibleAlgorithmRunner<
		shark::MOCMA,
		shark::DTLZ7
	> runner1 (
	mocma,
	boost::shared_ptr<
	shark::DTLZ7
	>( new shark::DTLZ7() )
	);
	

	mocma->m_useApproximatedHypervolume = true;
	runner1.run( 
		1,
		100,
		10,
		2,
		10000,
		1000
	);

	shark::moo::InterruptibleAlgorithmRunner<
		shark::EpsilonMOCMA,
		shark::DTLZ7
	> runner2 (
		boost::shared_ptr< 
			shark::EpsilonMOCMA
		>( new shark::EpsilonMOCMA() ),
		boost::shared_ptr<
			shark::DTLZ7
		>( new shark::DTLZ7() )
	);

	runner2.run( 
		1,
		100,
		10,
		2,
		10000,
		1000
	);

}
#endif