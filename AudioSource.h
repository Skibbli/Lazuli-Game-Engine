#ifndef AUDIO_SOURCE_H
#define AUDIO_SOURCE_H

#include "stdafx.h"

#include "Component.h"
#include "Resources.h"
#include "Transform.h"


/// Handles an audio clip and plays it in the scene
class AudioSource : public Component
{
	public:
		AudioSource();
		AudioSource(std::string _name, bool _positional, float _range);
		~AudioSource();

		/// Load the audio sample with the parameters provided
		void LoadAudioSample(std::string _name, bool _positional, float _range);
		/// Play the sample attached to this audio source
		void PlaySample();
		/// Stop playing the sample attached to this audio source
		void StopSample();

		void AudioSource::Awake();
		void AudioSource::Start();
		void AudioSource::Update();


	private:
		/// Whether or not this audio source is position in the scene
		bool m_isPositional;
		/// The range of the audio source if it's in the scene
		float m_range;

		ALLEGRO_SAMPLE_INSTANCE* m_sampleInstance;

		/// The transform of the game object this audio source is attached to
		weak<Transform> m_transform;
		/// The transform of the camera in the scene
		weak<Transform> m_cameraTransform;
};

#endif