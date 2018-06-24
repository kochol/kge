#ifndef KGE_COMPONENT_H
#define KGE_COMPONENT_H

#include <string>
#include "Node.h"
#ifdef KGE_USE_RAKNET
#	include <BitStream.h>
#endif // KGE_USE_RAKNET


namespace kge
{
	namespace en
	{
		class KGE_API Component: public Node
		{
		public:

			//! Constructor
			Component();

			//! Destructor
			virtual ~Component() {}

			//! Every component must declare this function for returning their class ID
			virtual int GetClassID() = 0;
			
			//! Every component must declare this function for returning their class name
			virtual std::string GetClassName() = 0;

			//! Clone a component
			virtual Component* Clone() = 0;

#ifdef KGE_USE_RAKNET
			//! Serialize the component to RakNet::BitStream
			virtual void Serialize(RakNet::BitStream& bs, bool write) {}

#endif // KGE_USE_RAKNET

		protected:


		}; // Component

	} // en

} // kge

#endif // KGE_COMPONENT_H
