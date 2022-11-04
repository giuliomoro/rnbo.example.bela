#include <Bela.h>
#include "RNBO.h"
#include <string>
#include <MiscUtilities.h>
#include <algorithm>

static const unsigned int kNoParam = -1; // use this below if you want to skip a channel

// A list of exposed parameters is printed when the program starts. Enter here
// the indeces of those you want to control from analog ins.
// The first n analog ins will be used to set these parameters. These values
// are set at every block, which means that the default value or the value set
// by a preset will be immediately overridden if the corresponding paramter is
// controlled by analogIn
static std::vector<unsigned int> parametersFromAnalog = {};
// same but for mapping digital ins to parameters. These are only updated upon
// change, so preset-loaded values are not necessarily overridden immediately
static std::vector<unsigned int> parametersFromDigital = {};
// whether to show hidden parameters when printing the parameters list
bool showHiddenParameters = false;

// has to be a pointer to ensure that it gets initialised after
// initialisation for the static PlatformInterfaceStdLib platformInstance has already taken place
static RNBO::CoreObject* rnbo;
static RNBO::PresetList* presetList;
static std::vector<bool> digitalParametersPast(parametersFromDigital.size());

void Bela_userSettings(BelaInitSettings *settings)
{
	settings->uniformSampleRate = 1;
	settings->interleave = 0;
	settings->analogOutputsPersist = 0;
}

template <typename T>
static ssize_t findIndex(const T value, std::vector<T> const& vals)
{
	ssize_t found = -1;
	for(size_t i = 0; i < vals.size(); ++i)
	{
		if(value == vals[i])
		{
			found = i;
			break;
		}
	}
	return found;
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
	parametersFromAnalog.resize(std::min(parametersFromAnalog.size(), context->analogInChannels));
	parametersFromDigital.resize(std::min(parametersFromDigital.size(), context->digitalChannels));
	unsigned int hiddenParameters = 0;
	printf("Available parameters: %u\n", rnbo->getNumParameters());
	for(unsigned int n = 0; n < rnbo->getNumParameters(); ++n)
	{
		RNBO::ParameterInfo info;
		rnbo->getParameterInfo(n, &info);
		if((!info.visible || info.debug) && !showHiddenParameters)
		{
			hiddenParameters++;
			continue;
		}
		printf("[%d] %s", n, rnbo->getParameterName(n));
		ssize_t analog = findIndex(n, parametersFromAnalog);
		ssize_t digital = findIndex(n, parametersFromDigital);
		if(analog >= 0)
			printf(" - controlled by analog in %d", analog);
		if(digital >= 0) {
			printf(" - controlled by digital in %d", digital);
			pinMode(context, 0, digital, INPUT);
		}
		printf("\n");
		if(analog >= 0 && digital >= 0)
			fprintf(stderr, "Parameter %d controlled by both analog and digital in. Digital in ignored\n", digital);
	}
	if(hiddenParameters)
		printf("+ %d hidden parameters\n", hiddenParameters);
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
	unsigned int nAnalogParameters = parametersFromAnalog.size();
	for(unsigned int c = 0; c < nAnalogParameters; ++c)
		rnbo->setParameterValueNormalized(parametersFromAnalog[c], analogReadNI(context, 0, c));
	for(unsigned int c = 0; c < parametersFromDigital.size(); ++c)
	{
		bool value = digitalRead(context, 0, c);
		// only send on change
		if(value != digitalParametersPast[c])
		{
			rnbo->setParameterValueNormalized(parametersFromDigital[c], value);
			digitalParametersPast[c] = value;
		}
	}

	unsigned int maxInChannels = context->audioInChannels + context->analogInChannels - nAnalogParameters;
	unsigned int nInChannels = rnbo->getNumInputChannels();
	if(nInChannels > maxInChannels)
		nInChannels = maxInChannels;
	float* inputs[nInChannels];
	for(unsigned int c = 0; c < nInChannels; ++c)
	{
		if(c < context->audioInChannels)
			inputs[c] = (float*)(context->audioIn + c * nFrames);
		else
			inputs[c] = (float*)(context->analogIn + (c - nAnalogParameters - context->audioInChannels) * nFrames);
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
