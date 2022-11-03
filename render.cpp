#include <Bela.h>
#include "RNBO.h"
#include <string>
#include <MiscUtilities.h>
#include <algorithm>

// A list of exposed parameters is printed when the program starts. Enter here
// the indeces of those you want to control from analog ins.
// The first n analog ins will be used to set these parameters. These values
// are set at every block, which means that the default value or the value set
// by a preset will be immediately overridden if the corresponding paramter is
// controlled by analogIn
static std::vector<unsigned int> parametersFromAnalog = {0, 1, 2};

// has to be a pointer to ensure that it gets initialised after
// initialisation for the static PlatformInterfaceStdLib platformInstance has already taken place
static RNBO::CoreObject* rnbo;
static RNBO::PresetList* presetList;

void Bela_userSettings(BelaInitSettings *settings)
{
	settings->uniformSampleRate = 1;
	settings->interleave = 0;
	settings->analogOutputsPersist = 0;
}

bool setup(BelaContext *context, void *userData)
{
	// verify settings have been applied
	if(context->flags & BELA_FLAG_INTERLEAVED)
	{
		fprintf(stderr, "You need a non-interleaved buffer\n");
		return false;
	}
	if(context->analogSampleRate != context->audioSampleRate)
	{
		fprintf(stderr, "You need the analog and audio channels to have the same sampling rate\n");
		return false;
	}
	rnbo = new RNBO::CoreObject;
	printf("%d parameters available:\n", rnbo->getNumParameters());
	for(unsigned int n = 0; n < rnbo->getNumParameters(); ++n)
	{
		bool controlled = false;
		size_t i;
		for(i = 0; i < parametersFromAnalog.size() && i < context->analogInChannels; ++i)
		{
			if(n == parametersFromAnalog[i])
			{
				controlled = true;
				break;
			}
		}
		printf("[%d] %s", n, rnbo->getParameterName(n));
		if(controlled)
		       printf(" - controlled by analog in %d", i);
		printf("\n");
	}
	std::string presetFile = "presets.json";
	printf("Loading presets from %s\n", presetFile.c_str());
	std::string s = IoUtils::readTextFile(presetFile);
	if(s.size())
	{
		// load presets, see C++ snippets from https://rnbo.cycling74.com/learn/presets-with-snapshots
		presetList = new RNBO::PresetList(s);
		printf("Found %d presets\n", presetList->size());
		if(presetList->size())
		{
			unsigned int idx = 0;
			RNBO::UniquePresetPtr preset = presetList->presetAtIndex(idx);
			printf("Loading preset %d: %s\n", idx, presetList->presetNameAtIndex(idx).c_str());
			rnbo->setPreset(std::move(preset));
		}
	}
	rnbo->prepareToProcess(context->audioSampleRate, context->audioFrames, true);
	return true;
}

void render(BelaContext *context, void *userData)
{
	unsigned int nFrames = context->audioFrames;
	unsigned int nParameters = 0;
	if(parametersFromAnalog.size())
	{
		nParameters = rnbo->getNumParameters();
		nParameters = std::max(nParameters, context->analogInChannels);
		nParameters = std::max(nParameters, parametersFromAnalog.size());
		for(unsigned int c = 0; c < nParameters; ++c)
			rnbo->setParameterValueNormalized(parametersFromAnalog[c], analogReadNI(context, 0, c));
	}

	unsigned int maxInChannels = context->audioInChannels + context->analogInChannels - nParameters;
	unsigned int nInChannels = rnbo->getNumInputChannels();
	if(nInChannels > maxInChannels)
		nInChannels = maxInChannels;
	float* inputs[nInChannels];
	for(unsigned int c = 0; c < nInChannels; ++c)
	{
		if(c < context->audioInChannels)
			inputs[c] = (float*)(context->audioIn + c * nFrames);
		else
			inputs[c] = (float*)(context->analogIn + (c - nParameters - context->audioInChannels) * nFrames);
	}

	unsigned int maxOutChannels = context->audioOutChannels + context->analogOutChannels;
	unsigned int nOutChannels = rnbo->getNumOutputChannels();
	if(nOutChannels > maxOutChannels)
		nOutChannels = maxOutChannels;
	float* outputs[nOutChannels];
	for(unsigned int c = 0; c < nOutChannels; ++c)
	{
		if(c < context->audioOutChannels)
			outputs[c] = context->audioOut + c * nFrames;
		else
			outputs[c] = context->analogOut + (c - (context->audioOutChannels)) * nFrames;
	}
	rnbo->process(inputs, nInChannels, outputs, nOutChannels, nFrames);
}

void cleanup(BelaContext *context, void *userData)
{
	delete presetList;
	delete rnbo;
}
