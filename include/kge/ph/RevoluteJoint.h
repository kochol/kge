#ifndef KGE_PH_REVOLUTE_JOINT_H
#define KGE_PH_REVOLUTE_JOINT_H

namespace kge
{
	namespace ph
	{
		class RevoluteJoint
		{
		public:
			RevoluteJoint() { /* Nothing To Do */ }

			virtual ~RevoluteJoint() { /*Nothing To Do */ }

			JointType getType(){ return EJTP_REVOLUTE; }

			virtual void setLimit( math::Vector *axis , float high , float low ) = 0;
		protected:
		}; // class RevoluteJoint
	} // ph
} // kge

#endif // KGE_PH_REVOLUTE_JOINT_H