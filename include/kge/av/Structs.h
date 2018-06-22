// File name: structs.h
// Des: This file contains av namespace enums and structs.
// Date: 20/5/1386
// Programmers: Pooya Shahinfar (Pswin), Ali Akbar Mohammadi (Kochol)

#ifndef KGE_STRUCTS_AV_H
#define KGE_STRUCTS_AV_H

namespace kge
{
//! Audio/Vidoe classes
namespace av
{
	//! Sound Source Status
	// az in enum baraye moshakhas kardane halat manbae seda estefade mishavad
	enum SourceStates
	{
		///  megdar dahi nashode
		SSS_INITIAL ,
		/// manbae dar hal pakhshe seda mibashad
		SSS_PLAYING,
		/// manbae faal nist va hich sedaie ra pakhsh nemikond
		SSS_STOPPED,
		/// mabae maks karde va hich sedaye ra pakhsh nemikonad
		SSS_PAUSED
	};
	
	//! sound source type
	// in enum noe manbae ra moshakhas mikonad
	enum SourceType
	{
		/// halat pishfarz ast ke neshan midahd hich bufferi ekhtesas dade nashode va manbae amade nist.
		SST_UNDETERMINED,
		/// noe manbae static mibashad.
		SST_STATIC, 
		/// noe manbae be sorate streaming (jariani) mibashad,bra mosigie poshte sahne monaseb ast.
		SST_STREAMING
	};
	
	//! Distance Model 
	// modelhaye mohasebe fasle manbae va tanzim gain manabe
	enum DistanceGainType
	{
		/// be mohasebe fasle tavajoh nakon
		DGT_NO_ATTENUATION,
		// noe mohasebe pishfarz
		DGT_DEFAULT,
		/// noe mahasebe khati fasle
		DGT_LINER,
		/// noe manbae be sorate streaming (jariani) mibashad,bra mosigie poshte sahne monaseb ast.
		DGT_INVERSE_CLAMPED
	};
	
	//! Sounde Source Distance factors
	// factorhaye mohasebe fasle har manbae
	enum SourceDistanceFactor
	{
		/// bishtarin faselie ke manbe gabel shenidan hast (dar halate khati)
		SDF_MAX_DISTANCE,
		// faseleie ke volume manbae dar on nogte nesf mishvad 
		SDF_REFERENCE_DISTANCE,
		///	mizane ofte seda nesbat be fasle (pishfarz = 1) 
		SDF_ROLLOFF_FACTOR
	};
	
	enum SourceVolumeFactor
	{
		/// mizane volume manbae
		SVF_GAIN,
		// bishtarin volume manbae 
		SVF_MAX_GAIN,
		//kamtarin volume manbae
		SVF_MIN_GAIN
	};

} // av

} // kge

#endif // KGE_STRUCTS_AV_H
