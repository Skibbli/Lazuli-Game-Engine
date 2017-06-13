#include "stdafx.h"

#include "AudioSource.h"
#include "Core.h"

AudioSource::AudioSource()
{
	m_sampleInstance = nullptr;
	m_range = 999;
}

AudioSource::AudioSource(std::string _name, bool _positional, float _range)
{
	m_sampleInstance = nullptr;
	m_range = _range;

	m_sampleInstance = Resources::LoadAudioSample(_name).lock()->GetSampleInstance();
	al_attach_sample_instance_to_mixer(m_sampleInstance, al_get_default_mixer());
	m_isPositional = _positional;
}

void AudioSource::LoadAudioSample(std::string _name, bool _positional, float _range)
{
	if(m_sampleInstance)
		al_destroy_sample_instance(m_sampleInstance);

	m_sampleInstance = Resources::LoadAudioSample(_name).lock()->GetSampleInstance();
	al_attach_sample_instance_to_mixer(m_sampleInstance, al_get_default_mixer());
	m_isPositional = _positional;
	m_range = _range;
}

void AudioSource::Awake()
{
	
}

void AudioSource::Start()
{
	m_transform = m_gameObject.lock()->GetComponent<Transform>();
	m_cameraTransform = Core::GetCameras().at(0).lock()->GetGameObj().lock()->GetComponent<Transform>();

	al_set_sample_instance_playmode(m_sampleInstance, ALLEGRO_PLAYMODE_LOOP);
	al_play_sample_instance(m_sampleInstance);
}

void AudioSource::Update()
{
	if(m_isPositional)
	{
		float distToCam = glm::length(m_transform.lock()->GetWorldPosition() - m_cameraTransform.lock()->GetWorldPosition());
		float volume = m_range / distToCam;

		if(volume < 0.05f)
			volume = 0.0f;

		if(volume > 1.0f)
			volume = 1.0f;

		al_set_sample_instance_gain(m_sampleInstance, volume);
	}
}

void AudioSource::PlaySample()
{
	al_play_sample_instance(m_sampleInstance);
}

void AudioSource::StopSample()
{
	al_stop_sample_instance(m_sampleInstance);
}

AudioSource::~AudioSource()
{
	//al_destroy_sample_instance(m_sampleInstance);
}