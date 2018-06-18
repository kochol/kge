#ifndef KGE_PH_CYLINDRICAL_JOINT_H
#define KGE_PH_CYLINDRICAL_JOINT_H

namespace kge
{
	namespace ph
	{
		class CylindricalJoint : public Joint
		{
		public:
			CylindricalJoint() { /*Nothing To Do*/ }

			virtual ~CylindricalJoint() { /*Nothing To Do*/ }

			JointType getType() { return EJTP_CYLINDRICAL; }
		protected:
		}; //class CylindricalJoint
	} // ph
} // kge
#endif // KGE_PH_CYLINDRICAL_JOINT_H