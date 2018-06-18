#ifndef KGE_PHYSIC_TYPE_H
#define KGE_PHYSIC_TYPE_H

#define PHYSIC_ENABLE_DEBUG	1

namespace kge
{
	namespace ph
	{
		//*******************************
		enum CollitionType
		{
			ECTP_TRIGGER_DISABLE				= 0,
			ECTP_TRIGGER_ON_ENTER				= (1<<0),
			ECTP_TRIGGER_ON_LEAVE				= (1<<1),
			ECTP_TRIGGER_ON_STAY				= (1<<2),
			ECTP_TRIGGER_ALL					= (ECTP_TRIGGER_ON_ENTER | ECTP_TRIGGER_ON_LEAVE | ECTP_TRIGGER_ON_STAY)
		}; // CollitionType

		enum BodyFlag
		{
			EBFP_NORMAL				= 0,
			EBFP_DISABLE_GRAVITY	= (1<<0),
			EBFP_FROZEN_POS_X		= (1<<1),
			EBFP_FROZEN_POS_Y		= (1<<2),
			EBFP_FROZEN_POS_Z		= (1<<3),
			EBFP_FROZEN_ROT_X		= (1<<4),
			EBFP_FROZEN_ROT_Y		= (1<<5),
			EBFP_FROZEN_ROT_Z		= (1<<6),
			EBFP_FROZEN_POS		= EBFP_FROZEN_POS_X|EBFP_FROZEN_POS_Y|EBFP_FROZEN_POS_Z,
			EBFP_FROZEN_ROT		= EBFP_FROZEN_ROT_X|EBFP_FROZEN_ROT_Y|EBFP_FROZEN_ROT_Z,
			EBFP_FROZEN			= EBFP_FROZEN_POS|EBFP_FROZEN_ROT,
			EBFP_KINEMATIC			= (1<<7),
			EBFP_VISUALIZATION		= (1<<8),
			EBFP_DUMMY_0			= (1<<9),
			EBFP_FILTER_SLEEP_VEL	= (1<<10),
			EBFP_ENERGY_SLEEP_TEST	= (1<<11)
		};
		//*******************************
		enum PhysicsEngineType
		{
			EPET_PhysX = 0,

			EPET_Count

		}; // PhysicsEngineType
		//*******************************
		enum ActorType
		{
			EATP_DYNAMIC,
			EATP_STATIC,
			EATP_TRIGGER,
			EATP_KINEMATIC
		}; // ActorType
		//*******************************
		enum JointType
		{
			EJTP_SPHERICAL,
			EJTP_REVOLUTE,
			EJTP_PRISMATIC,
			EJTP_CYLINDRICAL,
			EJTP_FIXED,
			EJTP_DISTANCE,
			EJTP_POINT_IN_PLANE,
			EJTP_POINT_IN_LINE,
			EJTP_PULLEY
		}; // JointType
		//*******************************
		enum DistanceJointFlag
		{
			EDJF_DISABLE				= 0,				
			EDJF_MAX_DISTANCE_ENABLED	= 1 << 0,
			//brief true if the joint enforces the minimum separate distance.
			EDJF_MIN_DISTANCE_ENABLED	= 1 << 1,
			//brief true if the spring is enabled
			EDJF_MIN_MAX_ENABLE			= EDJF_MAX_DISTANCE_ENABLED | EDJF_MIN_DISTANCE_ENABLED ,
			EDJF_SPRING_ENABLED			= 1 << 2
		}; // DstanceJointFlag

	} // ph
} // kge

#endif // KGE_PHYSIC_TYPE_H