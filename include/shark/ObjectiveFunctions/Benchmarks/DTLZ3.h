//===========================================================================
/*!
*  \brief Objective function DTLZ3.
*
*  \author T.Voss, T. Glasmachers, O.Krause
*  \date 2010-2011
*
*  \par Copyright (c) 1998-2007:
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
//===========================================================================
#ifndef SHARK_OBJECTIVEFUNCTIONS_BENCHMARK_DTLZ3_H
#define SHARK_OBJECTIVEFUNCTIONS_BENCHMARK_DTLZ3_H

#include <shark/ObjectiveFunctions/AbstractObjectiveFunction.h>
#include <shark/ObjectiveFunctions/BoxConstraintHandler.h>

namespace shark {
/**
* \brief Implements the benchmark function DTLZ3.
*
* See: http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.18.7531&rep=rep1&type=pdf
* The benchmark function exposes the following features:
*	- Scalable w.r.t. the searchspace and w.r.t. the objective space.
*/
struct DTLZ3 : public MultiObjectiveFunction
{
	DTLZ3(std::size_t numVariables = 0) : m_objectives(2), m_handler(SearchPointType(numVariables,0),SearchPointType(numVariables,1) ){
		announceConstraintHandler(&m_handler);
		m_name="DTLZ3";
	}
	
	std::size_t numberOfObjectives()const{
		return m_objectives;
	}
	bool hasScalableObjectives()const{
		return true;
	}
	void setNumberOfObjectives( std::size_t numberOfObjectives ){
		m_objectives = numberOfObjectives;
	}
	
	std::size_t numberOfVariables()const{
		return m_handler.dimensions();
	}
	
	bool hasScalableDimensionality()const{
		return true;
	}

	/// \brief Adjusts the number of variables if the function is scalable.
	/// \param [in] numberOfVariables The new dimension.
	void setNumberOfVariables( std::size_t numberOfVariables ){
		m_handler.setBounds(
			SearchPointType(numberOfVariables,0),
			SearchPointType(numberOfVariables,1)
		);
	}

	ResultType eval( const SearchPointType & x ) const {
		m_evaluationCounter++;

		ResultType value( numberOfObjectives() );

		int k = numberOfVariables() - numberOfObjectives() + 1 ;
		double g = 0.0 ;

		for( unsigned int i = numberOfVariables() - k; i < numberOfVariables(); i++ )
			g += sqr( x( i ) - 0.5 ) - std::cos( 20. * M_PI * ( x(i) - 0.5 ) );

		g = 100 * (k + g);

		 for( unsigned int i = 0; i < numberOfObjectives(); i++ ) {
		     double f = (1 + g);
		     for( unsigned int j = 0; j < numberOfObjectives() - (i + 1); j++)            
			 f *= std::cos( x[j] * 0.5 * M_PI );                
		     if (i != 0){
			 unsigned int aux = numberOfObjectives() - (i + 1);
			 f *=std ::sin(x[aux] * 0.5 * M_PI);
		     }
		     value( i ) = f;
		 }

		return value;
	}
private:
	std::size_t m_objectives;
	BoxConstraintHandler<SearchPointType> m_handler;
};


//~ /**
 //~ * \brief Specializes MultiObjectiveFunctionTraits for DTLZ3.
 //~ */
//~ template<> struct MultiObjectiveFunctionTraits<DTLZ3> {

	//~ /**
	//~ * \brief Models the reference Pareto-front type.
	//~ */
	//~ typedef std::vector< DTLZ3::ResultType > ParetoFrontType;

	//~ /**
	//~ * \brief Models the reference Pareto-set type.
	//~ */
	//~ typedef std::vector< DTLZ3::SearchPointType > ParetoSetType;

	//~ static std::vector< DTLZ3::ResultType > referenceFront( std::size_t noPoints, std::size_t n, std::size_t m ) {
		//~ if( m != 2 )
			//~ throw( shark::Exception( "DTLZ3: No reference front for no. of objectives other than 2 available." ) );
		//~ std::vector< DTLZ3::ResultType > result( noPoints, DTLZ3::ResultType( m ) );
		//~ for( std::size_t i = 0; i < result.size(); i++ ) {
			//~ result[ i ][ 0 ] = static_cast< double >( i ) / static_cast< double >( result.size() - 1 );
			//~ result[ i ][ 1 ] = ::sqrt( 1 - boost::math::pow<2>( result[ i ][ 0 ] ) );
		//~ }

		//~ return( result );
	//~ }
//~ };

ANNOUNCE_MULTI_OBJECTIVE_FUNCTION( DTLZ3, shark::moo::RealValuedObjectiveFunctionFactory );
}
#endif