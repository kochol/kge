#ifndef KGE_PH_DISTANCE_JOINT_H
#define KGE_PH_DISTANCE_JOINT_H

namespace kge
{
	namespace ph
	{
		class DistanceJoint : public Joint
		{
		public:

			//Contructor
			DistanceJoint() { /* Nothing To Do */ }

			//Distructor
			virtual ~DistanceJoint() { /* Nothing To Do */ }

			JointType getType() { return EJTP_DISTANCE; }

		protected:
		}; // Class DistanceJoint
	} // ph
} // kge

#endif // KGE_PH_DISTANCE_JOINT_H