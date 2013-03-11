//===========================================================================
/*!
* \file LZ8.h
*
* \brief Multi-objective optimization benchmark function LZ8.
*
*  The function is described in
*
* H. Li and Q. Zhang. 
* Multiobjective Optimization Problems with Complicated Pareto Sets, MOEA/D and NSGA-II, 
* IEEE Trans on Evolutionary Computation, 2(12):284-302, April 2009. 
*
* <BR><HR>
* This file is part of Shark. This library is free software;
* you can redistribute it and/or modify it under the terms of the
* GNU General Public License as published by the Free Software
* Foundation; either version 3, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this library; if not, see <http://www.gnu.org/licenses/>.
*/
//===========================================================================
#ifndef SHARK_OBJECTIVEFUNCTIONS_BENCHMARK_LZ8_H
#define SHARK_OBJECTIVEFUNCTIONS_BENCHMARK_LZ8_H

#include <shark/ObjectiveFunctions/AbstractMultiObjectiveFunction.h>
#include <shark/ObjectiveFunctions/BoxConstraintHandler.h>
#include <shark/Core/SearchSpaces/VectorSpace.h>

namespace shark {
/*! \brief Multi-objective optimization benchmark function LZ8.
*
*  The function is described in
*
*  H. Li and Q. Zhang. 
*  Multiobjective Optimization Problems with Complicated Pareto Sets, MOEA/D and NSGA-II, 
*  IEEE Trans on Evolutionary Computation, 2(12):284-302, April 2009. 
*/
struct LZ8 : public AbstractMultiObjectiveFunction< VectorSpace<double> >
{
	typedef AbstractMultiObjectiveFunction< VectorSpace<double> > super;

	LZ8(std::size_t numVariables = 0) : super( 2 ),m_handler(SearchPointType(numVariables,0),SearchPointType(numVariables,1) ){
		m_features |= CAN_PROPOSE_STARTING_POINT;
		m_features |= IS_CONSTRAINED_FEATURE;
		m_features |= HAS_CONSTRAINT_HANDLER;
		m_features |= CAN_PROVIDE_CLOSEST_FEASIBLE;
		m_name="LZ8";
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
	
	BoxConstraintHandler<SearchPointType> const& getConstraintHandler()const{
		return m_handler;
	}

	ResultType eval( const SearchPointType & x ) const {
		m_evaluationCounter++;

		ResultType value( 2, 0 );

		unsigned int counter1 = 0, counter2 = 0;
		for( unsigned int i = 1; i < x.size(); i++ ) {
			double y = x(i) - ::pow( x(0), 0.5*(1.0 + 3*(i-1)/(x.size()-1) ) );
			if( i % 2 == 0 ) {
				counter2++;
				value[1] += sqr( y );// - ::cos( 8*y*M_PI) + 1.;
				double product = 2.;
				for( unsigned int j = 0; j < x.size(); j++ )
					if( j%2 == 0 )
						product *= ::cos( 20 * y * M_PI / ::sqrt( static_cast<double>( j ) ) );
				value[1] -= product + 2.;
			} else {
				counter1++;
				value[0] += sqr( y );// - ::cos( 8*y*M_PI) + 1.;
				double product = 2.;
				for( unsigned int j = 0; j < x.size(); j++ )
					if( j%2 == 1 )
						product *= ::cos( 20 * y * M_PI / ::sqrt( static_cast<double>( j ) ) );
				value[0] -= product + 2.;
			}
		}

		value[0] *= 8./counter1;
		value[0] += x( 0 );

		value[1] *= 8./counter2;
		value[1] += 1 - ::sqrt( x( 0 ) );

		return( value );
	}
private:
	BoxConstraintHandler<SearchPointType> m_handler;
};

ANNOUNCE_MULTI_OBJECTIVE_FUNCTION( LZ8, shark::moo::RealValuedObjectiveFunctionFactory );
}
#endif