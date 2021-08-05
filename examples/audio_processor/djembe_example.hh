#pragma once
#include "lib/CoreModules/djembeCore_neon.h"
#include "util/countzip.hh"
#include "util/math.hh"
#include "util/math_tables.hh"
#include "util/oscs.hh"
#include <array>

template<typename AudioStreamConf, size_t NumDjembes = 4>
class DjembeExample {

	using Djembe = DjembeCoreNeon<AudioStreamConf::SampleRate>;

	std::array<Djembe, NumDjembes> djembes;

	static constexpr uint32_t trigger_pulse_width = 10;
	float trigger_period_samples[NumDjembes];
	uint32_t trigger_counter[NumDjembes]{0, 0};

	using AudioInBuffer = typename AudioStreamConf::AudioInBuffer;
	using AudioOutBuffer = typename AudioStreamConf::AudioOutBuffer;

public:
	DjembeExample()
	{
		for (auto [i, djembe] : countzip(djembes)) {
			float f = i;

			// Distinct rhythm for each djembe:
			trigger_period_samples[i] = ((f + 3.f) / 10.f) * AudioStreamConf::SampleRate;

			// Distinct freq and "strike" for each djembe:
			djembe.set_param(Djembe::Freq, 1.f / (f + 2.3f));
			djembe.set_param(Djembe::Gain, 0.9f);
			djembe.set_param(Djembe::Sharpness, 0.9f);
			djembe.set_param(Djembe::Strike, 3.f / (f * 2.f + 4.f));
		}
	}

	void process(AudioInBuffer &in_buffer, AudioOutBuffer &out_buffer)
	{
		for (auto &out : out_buffer) {
			float left_mix = 0.f;
			float right_mix = 0.f;

			for (auto [i, djembe] : countzip(djembes)) {
				if (trigger_counter[i] == trigger_pulse_width)
					djembe.set_input(Djembe::Trigger, 1.f);
				else if (trigger_counter[i] == 0) {
					djembe.set_input(Djembe::Trigger, 0.f);
					trigger_counter[i] = trigger_period_samples[i];
				}
				trigger_counter[i]--;

				djembe.update();

				float o = djembe.get_output(0);
				float pan = static_cast<float>(i) / static_cast<float>(NumDjembes - 1);
				left_mix += o * pan;
				right_mix += o * (1.f - pan);
			}

			left_mix *= 2.f / static_cast<float>(NumDjembes);
			right_mix *= 2.f / static_cast<float>(NumDjembes);
			out.chan[0] = AudioStreamConf::AudioOutFrame::scaleOutput(left_mix);
			out.chan[1] = AudioStreamConf::AudioOutFrame::scaleOutput(right_mix);
		}
	};
};
