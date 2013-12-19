#ifndef KGE_PH_JOINT_H
#define KGE_PH_JOINT_H

namespace kge
{
	namespace ph
	{
		class Joint
		{
		public:
			Joint() { /*Nothing To Do*/ }

			virtual ~Joint() { /*Nothing To Do*/ }

			virtual JointType getType	( void ) = 0;
		protected:
		}; // class Joint
	} // ph
} // ph
#endif // KGE_PH_JOINT_H
