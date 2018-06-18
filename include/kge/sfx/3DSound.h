// File name: structs.h
// Des: in file inteface asli kar ba sedaye 3D hast
// Date: 20/5/1386
// Programmer: Pooya Shahinfar (Pswin)


#include "../../Libs/al/al.h"
#include "../../Libs/al/alc.h"
#include "../../Libs/al/alut.h"

#include <vector>
#include "../math/vector.h"


namespace kge
{
	namespace sfx
	{
		namespace Sound3D
		{
		//! Sound Source class interface
		class KGE_API SoundSource
		{
		private:
			ALuint Source;
		public:
			//! Sazande.
			SoundSource();

			//! Mokhareb.
			~SoundSource();

			//  mogeiate manbae
			void SetPosition (math::Vector pos);
			math::Vector GetPosition ();
			
			// VELOCITY manbae
			void SetVelocity(math::Vector velocity);
			math::Vector GetVelocity();

			// voulome manbae
			void SetVolume (float value,SourceVolumeFactor factor = SVF_GAIN);
			float GetVolume (SourceVolumeFactor factor = SVF_GAIN);

			// sorate pakhshe manbae
			void  SetPitch(float value);
			float GetPitch();
			
			//parametr haye mohasebe gain nesbat be fasle manbae az shenavande
			float GetDistanceFactor (SourceDistanceFactor factor );
			void  SetDistanceFactor (SourceDistanceFactor factor,float value);

			// loop manbae
			bool GetLoop();
			void SetLoop(bool looping);
	
			// vaziate manbae
			SourceStates GetState(); 

			// zamime kadane bufferha
			void AttachBuffer(ALuint buffer);
			void AttachFromFile(char* filename);

			// hazfe buffer attach shode
			void RemoveBuffer(int i);

			// bool AttachBufferQueue (); "in the next reversion"

			//noe manbae (static,streaming...)
			SourceType GetType();

			//pakhshe seda az manbae
			void Play();

			//makse pakhshe seda
			void Pause();

			//iste pakhshe seda
			void Stop();
		
		};
	
		//! Sound Buffer Class
		class KGE_API SoundBuffer
		{
			
		public:
			// buffer
			ALuint Buffer;

			//! Sazande.
			SoundBuffer();

			//! Mokhareb.
			~SoundBuffer();

			//! load kardan buffer az file
			void LoadFromFile(char* filename);
		
		};
	
		//! Listener of Sound Class
		class KGE_API Listener
		{
		public:
			//! Sazande.
			Listener(){};

			//! Mokhareb.
			~Listener(){};
		
			//! gerftan mogiate shenavandeh
			math::Vector GetPostion();

			//! tanzim kardabe mogiate shenavandeh
			void SetPostion(math::Vector pos);

			//! tanzim kardan jahat girie shenavandeh
			void SetOrientation(math::Vector up,math::Vector at);

			//! velocity
			void SetVelocity(math::Vector vel);
			math::Vector GetVelocity();

			//! mizane shenavaie shenavandeh (default 1)
			void SetGain(float value);
			float GetGain();

			//! Distance Model
			void SetDistanceModel(DistanceGainType model);

		};

		} //end of name sapcae 3DSound

	}// end of namesapce sfx

} // end of namspace kge

#endif // KGE_STRUCTS_AV_H