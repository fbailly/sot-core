/*
 * Copyright 2010,
 * François Bleibel,
 * Olivier Stasse,
 *
 * CNRS/AIST
 *
 * This file is part of sot-core.
 * sot-core is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 * sot-core is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.  You should
 * have received a copy of the GNU Lesser General Public License along
 * with sot-core.  If not, see <http://www.gnu.org/licenses/>.
 */

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* --- SOT --- */
#include <sot/core/motion-period.hh>
#include <sot/core/exception-feature.hh>
#include <sot/core/debug.hh>
#include <sot/core/factory.hh>

#include <jrl/mal/matrixabstractlayer.hh>

using namespace std;
using namespace dynamicgraph::sot;
using namespace dynamicgraph;


DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(MotionPeriod,"MotionPeriod");

/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */

MotionPeriod::
MotionPeriod( const string& fName )
  : Entity( fName )
    ,motionParams( 0 )
    ,motionSOUT( boost::bind(&MotionPeriod::computeMotion,this,_1,_2),
		 sotNOSIGNAL,
		 "MotionPeriod("+name+")::output(vector)::motion" )
{
  signalRegistration( motionSOUT );
  motionSOUT.setNeedUpdateFromAllChildren( true );
  resize(0);
}


/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */

ml::Vector&
MotionPeriod::computeMotion( ml::Vector& res,const int& time )
{
  sotDEBUGIN(15);

  res.resize( size );
  for( unsigned int i=0;i<size;++i )
    {
      const sotMotionParam & p = motionParams[i];
      double x= ( ( ((time-p.initPeriod) % p.period) +0.0 ) 
		  / (p.period+0.0) );
      res(i)=p.initAmplitude;
      switch( p.motionType )
	{
	case MOTION_CONSTANT: { res(i)+= p.amplitude;  break; }
	case MOTION_SIN: { res(i)+= p.amplitude*sin(M_PI*2*x); break; }
	case MOTION_COS: { res(i)+= p.amplitude*cos(M_PI*2*x); break; }
	  //case MOTION_: {res(i)+= p.amplitude; break}
	}
    }

  sotDEBUGOUT(15);
  return res;
}

void MotionPeriod::
resize( const unsigned int & _size ) 
{
  size = _size;
  motionParams.resize(size);
  for( unsigned int i=0;i<size;++i )
    {
      motionParams[i] .motionType = MOTION_CONSTANT;
      motionParams[i] .amplitude = 0;
      motionParams[i] .initPeriod = 0;
      motionParams[i] .period = 1;
      motionParams[i] .initAmplitude = 0;
    }
}


void MotionPeriod::
display( std::ostream& os ) const
{
  os <<"MotionPeriod <"<<name<<"> ... TODO";
}


#define SOT_PARAMS_CONFIG(ARGname,ARGtype)                                              \
  else if( cmdLine == #ARGname )                                                        \
   {                                                                                    \
     unsigned int rank; ARGtype period;                                                 \
     cmdArgs >> rank >>std::ws;                                                         \
     if( rank>=this->size ) { os <<"!! Error: size size too large." << std::endl; }     \
     if( cmdArgs.good() )                                                               \
     { cmdArgs>>period; motionParams[rank].ARGname = period; }                             \
     else { os << #ARGname << "[" << rank << "] = "                                     \
               <<  motionParams[rank].ARGname << std::endl; }                              \
    }

/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */
