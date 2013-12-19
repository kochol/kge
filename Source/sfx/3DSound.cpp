// File name: 3DSound.h
// Des: in file shamele classhaye Source va Buffer mibashad va hamchenin 
// tavabe marbot be shenavande va tavabe omomie Sedaye 3D ra shamel mishavad
// Date: 25/5/1386
// Programmer: Pooya Shahinfar (Pswin)

#include "../../include/io/logger.h"
#include "../../include/sfx/structs.h"
#include "../../include/sfx/3dsound.h"

#pragma comment(lib, "openal32.lib") 
#pragma comment(lib, "alut.lib") 
					

namespace kge
{
	namespace sfx
	{
		namespace Sound3D
		{

			//***************************
			//SoundSource class functions
			//***************************

			// ********
			// Sazande.
			// ********
			SoundSource::SoundSource ()
			{
				alGetError(); // clearing error bit
				alGenSources(1, &Source); // generate sound source
				if (alGetError() != AL_NO_ERROR) //checking for error
					kge::io::Logger::Log(io::ELM_Error , "Could not create sound source");
				SetPosition(math::Vector(0,0,0)); //set default position
				SetVelocity(math::Vector(0,0,0)); //set default velocity
				SetVolume(1);   // tanzim kardan volume seda
				SetPitch(1);    // tanzim kardane sorate pakhshe seda
			}
		
			// ********
			// mokhareb.
			// ********
			SoundSource::~SoundSource ()
			{
			}

			void SoundSource::Play ()
			{
				alSourcePlay(Source);
				if (alGetError() != AL_NO_ERROR) // checking for error
					kge::io::Logger::Log(io::ELM_Error , "Could not play source, cheek source state");
			}

			void SoundSource::Stop ()
			{
				alSourceStop(Source); 
				if (alGetError() != AL_NO_ERROR) //checking for error
					kge::io::Logger::Log(io::ELM_Error , "Could not stop source playing, cheek source state or type");
			}

			void SoundSource::Pause ()
			{
				alSourcePause(Source);
				if (alGetError() != AL_NO_ERROR) // checking for error
					kge::io::Logger::Log(io::ELM_Error , "Could not play source, cheek source state or type");
			}

			void SoundSource::SetPosition (math::Vector pos)
			{
				alGetError(); // clearing error bit
				alSource3f(Source,AL_POSITION,pos.x,pos.y,pos.z ); //set postion
				if (alGetError() != AL_NO_ERROR) // checking for error
					kge::io::Logger::Log(io::ELM_Error , "Could not change source postion");
			}

			math::Vector SoundSource::GetPosition ()
			{
				math::Vector pos;
				alGetError(); //clearing Error bit
				alGetSource3f(Source,AL_POSITION,&pos.x,&pos.y,&pos.z);
				if (alGetError() != AL_NO_ERROR) // checking for error
					kge::io::Logger::Log(io::ELM_Error , "Could not get source postion");
				return pos;
			}

			void SoundSource::SetVelocity(math::Vector  velocity)
			{
				alGetError(); //clearing Error bit
				alSource3f(Source,AL_VELOCITY,velocity.x ,velocity.y,velocity.z); //set postion
				if (alGetError() != AL_NO_ERROR) // checking for error
					kge::io::Logger::Log(io::ELM_Error , "Could not change source velocity");
			}
	
			math::Vector SoundSource::GetVelocity()
			{
				math::Vector vel;
				alGetError(); //clearing Error bit
				alGetSource3f(Source,AL_POSITION,&vel.x,&vel.y,&vel.z);
				if (alGetError() != AL_NO_ERROR) // checking for error
					kge::io::Logger::Log(io::ELM_Error , "Could not get source velocity");
				return vel;
			}			
			void SoundSource::SetVolume(float value,SourceVolumeFactor factor)
			{
				alGetError();  // clearing error bit
				switch (factor)
				{
				case SVF_GAIN: 
					alSourcef (Source, AL_GAIN, value );
					break;
				case SVF_MAX_GAIN :
					alSourcef (Source, AL_MAX_GAIN, value );
					break;
				case SVF_MIN_GAIN:
					alSourcef (Source, AL_MIN_GAIN, value );
					break;
				}

				if (alGetError() != AL_NO_ERROR)
					kge::io::Logger::Log(io::ELM_Error , "Could not change source volume");
			}

			float SoundSource::GetVolume(SourceVolumeFactor factor)
			{
				float value;
				alGetError();  // clearing error bit
				switch (factor)
				{
				case SVF_GAIN: 
					alGetSourcef (Source, AL_GAIN,&value);
					break;
				case SVF_MAX_GAIN :
					alGetSourcef (Source, AL_MAX_GAIN,&value);
					break;
				case SVF_MIN_GAIN:
					alGetSourcef (Source, AL_MIN_GAIN,&value);
					break;
				}

				if (alGetError() != AL_NO_ERROR)
					kge::io::Logger::Log(io::ELM_Error , "Could not get source volume");

				return value;
			}	


			void SoundSource::SetPitch(float value)
			{
				alGetError(); // clearing error bit
				alSourcef (Source, AL_PITCH, value ); // set source pitch
				if (alGetError() != AL_NO_ERROR) // checking for error
					kge::io::Logger::Log(io::ELM_Error , "Could not change source pitch");
			}		
			float SoundSource::GetPitch()
			{
				float value;
				alGetError(); // clearing error bit
				alGetSourcef (Source, AL_PITCH,&value); // get source pitch
				if (alGetError() != AL_NO_ERROR) // checking for error
					kge::io::Logger::Log(io::ELM_Error , "Could not get  source pitch");
				return value;
			}

			float SoundSource::GetDistanceFactor (SourceDistanceFactor factor){
				float value;
				alGetError(); // clearing error bit
				switch (factor)
				{
					case SDF_MAX_DISTANCE :
						alGetSourcef (Source, AL_MAX_DISTANCE,&value);
					break;
					case SDF_REFERENCE_DISTANCE :
						alGetSourcef (Source, AL_REFERENCE_DISTANCE,&value);
					break;
					case SDF_ROLLOFF_FACTOR:
						alGetSourcef (Source, AL_ROLLOFF_FACTOR,&value);
					break;
				}
				if (alGetError() != AL_NO_ERROR) // checking for error
					kge::io::Logger::Log(io::ELM_Error , "Could not get source distance factor");
				return value;
			}

			void  SoundSource::SetDistanceFactor (SourceDistanceFactor factor,float value)
			{
				alGetError(); //clearing error bit
				switch (factor)
				{
					case SDF_MAX_DISTANCE :
						alSourcef (Source, AL_MAX_DISTANCE,value);
					break;
					case SDF_REFERENCE_DISTANCE :
						alSourcef (Source, AL_REFERENCE_DISTANCE,value);
					break;
					case SDF_ROLLOFF_FACTOR:
						alSourcef (Source, AL_ROLLOFF_FACTOR,value);
					break;
				}
			if (alGetError() != AL_NO_ERROR) // checking for error
					kge::io::Logger::Log(io::ELM_Error , "Could not set source distance factor");
			}

			bool SoundSource::GetLoop()
			{
				int value;
				alGetSourcei (Source, AL_LOOPING,&value);
				if (value == 1)
					return true;
				else
					return false;
			}

			void SoundSource::SetLoop(bool looping)
			{
				if (looping == true)
					alSourcei (Source, AL_LOOPING,AL_TRUE);
				else
					alSourcei (Source, AL_LOOPING,AL_FALSE);
				if (alGetError() != AL_NO_ERROR) //cheaking for error
					io::Logger::Log("Can not change source looping");
			}

			SourceStates SoundSource::GetState()
			{
				ALint state;
				alGetSourcei(Source, AL_SOURCE_STATE, &state);
				alGetError(); // clearing error bit
				switch (state)
				{
					case AL_PLAYING :
					return SSS_PLAYING ;
					break;
					case AL_STOPPED :
						return SSS_STOPPED  ;
					break;
					case AL_PAUSED :
						return SSS_PAUSED  ;
					break;
					case AL_INITIAL :
						return SSS_INITIAL ;
					break;
				}
			if (alGetError() != AL_NO_ERROR) // checking for error
				kge::io::Logger::Log(io::ELM_Error , "Could not get source state");
			}			

			void SoundSource::AttachBuffer (ALuint buffer)
			{
			alGetError(); // clearing error bit
			alSourcei (Source, AL_BUFFER,   buffer   );
			if (alGetError() != AL_NO_ERROR) // checking for error
				kge::io::Logger::Log(io::ELM_Error , "Could not attach buffer to the sound source");
			}

			void SoundSource::AttachFromFile (char *filename)
			{

				ALuint Buf;
				alGenBuffers(1, &Buf); //generate a buffer object
				ALenum format;
				ALsizei size;
				ALvoid* data;
				ALsizei freq;
				ALboolean loop;
					alGetError();
					alutLoadWAVFile(filename, &format, &data, &size, &freq, &loop); // load wav file
					alBufferData(Buf, format, data, size, freq); // attach wav file to the buffer
					if (alGetError() != AL_NO_ERROR) // checking for error
						kge::io::Logger::Log(io::ELM_Error , "Could not load wav file");
					alutUnloadWAV(format, data, size, freq); // unload wav file
					if (alutGetError() !=ALUT_ERROR_NO_ERROR)
						kge::io::Logger::Log(io::ELM_Error , "Could not get  source pitch");
				alGetError(); // clearing error bit
				alSourcei (Source, AL_BUFFER,   Buf  );
				if (alGetError() != AL_NO_ERROR) // checking for error
					kge::io::Logger::Log(io::ELM_Error , "Could not attach buffer to the sound source");
				alDeleteBuffers(1,&Buf);
			}




			SourceType SoundSource::GetType()
			{
				ALint type;
				alGetSourcei(Source, AL_SOURCE_TYPE, &type);
				alGetError(); // clearing error bit
				switch (type)
				{
					case AL_STREAMING :
						return SST_STREAMING ;
					break;
					case AL_STATIC :
						return SST_STATIC ;
					break;
					case AL_UNDETERMINED :
						return SST_UNDETERMINED   ;
					break;
				}
				if (alGetError() != AL_NO_ERROR) // checking for error
					kge::io::Logger::Log(io::ELM_Error , "Could not get  source type");
			}

			//***************************
			//sound Buffer class
			//***************************
			
			//*********
			//sazande
			//*********
			SoundBuffer::SoundBuffer()
			{
				alGetError(); // clearing error bit
				alGenBuffers(1, &Buffer); //generate a buffer object
				if (alGetError() != AL_NO_ERROR) // checking for error
					kge::io::Logger::Log(io::ELM_Error , "Could not generate sound buffer");
			}

			//*********
			//mokhareb
			//*********
			SoundBuffer::~SoundBuffer()
			{	
			}

			//! LoadFromFile
			void SoundBuffer::LoadFromFile(char *filename)
			{
					ALenum format;
					ALsizei size;
					ALvoid* data;
					ALsizei freq;
					ALboolean loop;
						alGetError();
						alutLoadWAVFile(filename, &format, &data, &size, &freq, &loop); // load wav file
						alBufferData(Buffer, format, data, size, freq); // attach wav file to the buffer
						if (alGetError() != AL_NO_ERROR) // checking for error
							kge::io::Logger::Log(io::ELM_Error , "Could not load wav file");
						alutUnloadWAV(format, data, size, freq); // unload wav file
						if (alutGetError() !=ALUT_ERROR_NO_ERROR)
							kge::io::Logger::Log(io::ELM_Error , "Could not get  source pitch");
			}



			///////////////////////////////////////////////
			// listener
			///////////////////////////////////////////////


			void Listener::SetPostion(kge::math::Vector pos)
			{
				alListener3f(AL_POSITION,pos.x,pos.y,pos.z );
				if (alGetError() != AL_NO_ERROR) // checking for error
					kge::io::Logger::Log(io::ELM_Error , "Could not Set Listner Postion");	
			}

			kge::math::Vector Listener::GetPostion()
			{
				kge::math::Vector pos;
				alGetListener3f(AL_POSITION,&pos.x,&pos.y,&pos.z);
				if (alGetError() != AL_NO_ERROR) // checking for error
					kge::io::Logger::Log(io::ELM_Error , "Could not Get Listner position");	
				return pos;
			}

			void Listener::SetVelocity(kge::math::Vector vel)
			{
				alListener3f(AL_VELOCITY,vel.x,vel.y,vel.z );
				if (alGetError() != AL_NO_ERROR) // checking for error
					kge::io::Logger::Log(io::ELM_Error , "Could not Set Listner Velocity");
			}

			kge::math::Vector Listener::GetVelocity()
			{

				kge::math::Vector vel;
				alGetListener3f(AL_VELOCITY,&vel.x,&vel.y,&vel.z);
				if (alGetError() != AL_NO_ERROR) // checking for error
					kge::io::Logger::Log(io::ELM_Error , "Could not Get Listner velocity");
				return vel;

			}

			void Listener::SetDistanceModel(DistanceGainType model)
			{
				alGetError(); //clearing error bit
				switch(model)
				{
				case DGT_NO_ATTENUATION: alDistanceModel( AL_NONE ); break;
				case DGT_DEFAULT: alDistanceModel( AL_INVERSE_DISTANCE_CLAMPED ); break;
				case DGT_LINER: alDistanceModel( AL_LINEAR_DISTANCE_CLAMPED ); break;
				case DGT_INVERSE_CLAMPED: alDistanceModel( AL_INVERSE_DISTANCE_CLAMPED ); break;
				}

				if (alutGetError() !=ALUT_ERROR_NO_ERROR)
							kge::io::Logger::Log(io::ELM_Error , "Could not get  source pitch");
				
			}

			void Listener::SetOrientation(kge::math::Vector up,kge::math::Vector at)
			{
				ALfloat ListenerOri[] = { up.x, up.y,up.z, at.x , at.y, at.z };
				alListenerfv(AL_ORIENTATION, ListenerOri);
				if (alutGetError() !=ALUT_ERROR_NO_ERROR)
					kge::io::Logger::Log(io::ELM_Error , "Could not get  source pitch");	
			}
			float Listener::GetGain ()
			{
				float value;
				alGetListenerf(AL_GAIN,&value);
				return value;
			}
			void Listener::SetGain(float value)
			{
				alGetError(); //clearing error bit
				alListenerf(AL_GAIN,value);
				if (alutGetError() !=ALUT_ERROR_NO_ERROR)
							kge::io::Logger::Log(io::ELM_Error , "Could not set listener gain");

			}
			
		}// paiane fazaie namie Sound3D
	} // paiane fazaie namie sfx
} // painane fazaie namie kge


