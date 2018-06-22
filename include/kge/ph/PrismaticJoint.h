#ifndef KGE_PH_PRISMATIC_JOINT_H
#define KGE_PH_PRISMATIC_JOINT_H

namespace kge
{
	namespace ph
	{
		class PrismaticJoint : public Joint
		{
		public:
			//! Contructor
			PrismaticJoint() { /* Nothing To Do */}

			//Distructor
			virtual ~PrismaticJoint() { /* Nothing To Do */ }

			JointType getType(){ return EJTP_PRISMATIC; }
		protected:
		}; // class PrismaticJoint
	} // ph
} // kge

#endif // KGE_PH_PRISMATIC_JOINT_H