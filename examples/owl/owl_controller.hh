#pragma once
#include "Patch.h"
#include "ProgramVector.h"
#include "audio_codec_conf.hh"

extern ProgramVector programVector;
// This is expected to be a global object

struct PatchState {
	int16_t parameters[40];
	uint8_t parameters_size;
	uint16_t buttons;

	void load()
	{
		auto *pv = getProgramVector();
		parameters_size = pv->parameters_size;
		memcpy(parameters, pv->parameters, sizeof(int16_t) * parameters_size);
		buttons = pv->buttons;
	}

	void store()
	{
		auto *pv = getProgramVector();
		pv->parameters_size = parameters_size;
		memcpy(pv->parameters, parameters, sizeof(int16_t) * parameters_size);
		pv->buttons = buttons;
	}
};

template<typename P>
class PatchInitializer {
public:
	PatchInitializer() = default;
	P *getPatchInstance()
};

template<typename AudioStreamConf>
class OwlController {
	Patch **patches;
	PatchState *patch_states;
	size_t num_patches;
	size_t current_patch;

	using AudioInBuffer = typename AudioStreamConf::AudioInBuffer;
	using AudioOutBuffer = typename AudioStreamConf::AudioOutBuffer;

public:
	OwlController() = default;

	template<typename... Ps>
	OwlController(Ps...)
	{
		num_patches = sizeof...(Ps);
		patches = new Patch *[sizeof...(Ps)];
		patch_states = new PatchState[sizeof...(Ps)];

		auto *pv = getProgramVector();
		if (AudioStreamConf::SampleBits == 24 && AudioStreamConf::NumInChans == 2 && AudioStreamConf::NumOutChans == 2)
		{
			pv->audio_format = AUDIO_FORMAT_24B32_2X;
		} else {
#error Unknown format
		}
		pv->audio_blocksize = AudioStreamConf::BlockSize;
		pv->audio_samplingrate = AudioStreamConf::SampleRate;

		setSystemTables(pv);

#ifdef USE_MIDI_CALLBACK__DISABLE
		void *midiRxArgs[] = {(void *)SYSTEM_FUNCTION_MIDI, (void *)&onMidiCallback};
		getProgramVector()->serviceCall(OWL_SERVICE_REGISTER_CALLBACK, midiRxArgs, 2);

		midi_send_callback = NULL;
		void *midiTxArgs[] = {(void *)SYSTEM_FUNCTION_MIDI, &midi_send_callback};
		getProgramVector()->serviceCall(OWL_SERVICE_REQUEST_CALLBACK, midiTxArgs, 2);
#endif /* USE_MIDI_CALLBACK */

		if (samples != NULL)
			SampleBuffer::destroy(samples);
		samples = SampleBuffer::create(pv->audio_format, pv->audio_blocksize);
		if (samples == NULL)
			error(CONFIGURATION_ERROR_STATUS, "Unsupported audio format");

		OwlController<Ps...>(0, Ps...));
	}

	template<typename P, typename... Ps>
	OwlController(size_t index, PatchInitializer<P> &init, Ps... >)
	{
		patches[i] = init.getPatchInstance();
		patch_state[i].store();
		OwlController(index + 1, Ps...);
	}

	template<typename P>
	OwlController(PatchInitializer<P> &init >)
	{
		patches[i] = init.getPatchInstance();
		patch_state[i].store();
	}

	void nextPatch()
	{
		index++;
		if (index > num_patches) {
			index = 0;
		}
		patch_states[index].load();
	}

	void prevPatch()
	{
		index--;
		if (index > num_patches) {
			index = num_patches - 1;
		}
		patch_states[index].load();
	}

	void process(AudioInBuffer &in_buffer, AudioOutBuffer &out_buffer)
	{
        patches[index]->process()
		for (auto &out : out_buffer) {
			// retrig env on EOC and go to next note
			if (!env.GetCurrentSegment()) {
				env.Trigger();
				harm.SetFreq(scale[note]);
				note = (note + 1) % 6;
			}

			// calculate the new amplitudes based on env value
			float center = env.Process();
			for (int i = 0; i < NumOscs; i++) {
				float dist = fabsf(center - (float)i) + 1.f;
				harm.SetSingleAmp(1.f / ((float)dist * 10.f), i);
			}

			out.chan[0] = AudioStreamConf::AudioOutFrame::scaleOutput(harm.Process());
			out.chan[1] = out.chan[0];
		}
	}

	DaisyHarmonicExample()
	{
		harm.Init(AudioStreamConf::SampleRate);
		harm.SetFirstHarmIdx(1);

		// init envelope
		env.Init(AudioStreamConf::SampleRate);
		env.SetTime(daisysp::ADENV_SEG_ATTACK, 0.05f);
		env.SetTime(daisysp::ADENV_SEG_DECAY, 0.35f);
		env.SetMin(0.0);
		env.SetMax(15.f);
		env.SetCurve(0); // linear
	}
};
