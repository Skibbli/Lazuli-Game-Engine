#ifndef AUDIO_SAMPLE_H
#define AUDIO_SAMPLE_H

#include "stdafx.h"


/// A loaded audio file
class AudioSample
{
	public:
		AudioSample(std::string _name);
		~AudioSample();

		/// Gets the name of the audio sample
		std::string GetName();
		/// Returns an instance of this audio sample
		ALLEGRO_SAMPLE_INSTANCE* GetSampleInstance();

	private:
		/// The name of this audio sample
		std::string m_sampleName;

		/// The audio sample
		ALLEGRO_SAMPLE* m_sample;
};

#endif