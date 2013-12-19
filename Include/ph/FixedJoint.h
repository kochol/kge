#ifndef KGE_PH_FIXED_JOINT_H
#define KGE_PH_FIXED_JOINT_H

namespace kge
{
	namespace ph
	{
		class FixedJoint : public Joint
		{
		public:
			FixedJoint() { /*Nothing To Do*/ }

			virtual ~FixedJoint() { /*Nothing To Do*/ }

			JointType getType() { return EJTP_FIXED; }
		protected:
		}; // class FixedJoint
	} // ph
} // kge

#endif // KGE_PH_FIXED_JOINT_H