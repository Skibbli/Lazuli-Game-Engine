#include "stdafx.h"

#include "AudioSample.h"


AudioSample::AudioSample(std::string _name)
{
	m_sample = nullptr;
	m_sampleName = _name;
	_name = "Assets/Audio/" + _name;

	m_sample = al_load_sample((_name + ".ogg").c_str());

	if(m_sample == nullptr)
	{
		m_sample = al_load_sample((_name + ".mp3").c_str());
	}

	if(m_sample == nullptr)
	{
		m_sample = al_load_sample((_name + ".wav").c_str());
	}
}

std::string AudioSample::GetName()
{
	return m_sampleName;
}

ALLEGRO_SAMPLE_INSTANCE* AudioSample::GetSampleInstance()
{
	return al_create_sample_instance(m_sample);
}

AudioSample::~AudioSample()
{
}
