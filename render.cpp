#include <Bela.h>
#include "RNBO.h"
#include <string>
#include <MiscUtilities.h>
#include <algorithm>

//#define BELA_RNBO_USE_TRILL // uncomment to use Trill
#ifdef BELA_RNBO_USE_TRILL
#include <libraries/Trill/Trill.h>
#endif // BELA_RNBO_USE_TRILL

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
// mapping parameters to digital out, e.g.: to display a toggle state with an LED
static std::vector<unsigned int> parametersToDigital = {};
#ifdef BELA_RNBO_USE_TRILL
// same but for mapping Trill location to parameters.
static std::vector<unsigned int> parametersFromTrillLocation = {};
// same but for mapping Trill size to parameters.
static std::vector<unsigned int> parametersFromTrillSize = {};
static std::vector<Trill*> trills {
	// add/edit more Trills here
	new Trill(1, Trill::BAR),
};
static std::vector<float> trillLocationParametersPast(parametersFromTrillLocation.size());
static std::vector<float> trillSizeParametersPast(parametersFromTrillSize.size());
#endif // BELA_RNBO_USE_TRILL

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

#ifdef BELA_RNBO_USE_TRILL
static void loop(void*)
{
	while(!Bela_stopRequested())
	{
		for(auto& t : trills)
			t->readI2C();
		usleep(5000);
	}
}
#endif // BELA_RNBO_USE_TRILL

bool setup(BelaContext *context, void *userData)
{
#ifdef BELA_RNBO_USE_TRILL
	Bela_runAuxiliaryTask(loop);
#endif // BELA_RNBO_USE_TRILL
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
#ifdef BELA_RNBO_USE_TRILL
	parametersFromTrillLocation.resize(std::min(parametersFromTrillLocation.size(), trills.size()));
	parametersFromTrillSize.resize(std::min(parametersFromTrillSize.size(), trills.size()));
#endif // BELA_RNBO_USE_TRILL
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
		ssize_t digitalIn = findIndex(n, parametersFromDigital);
		ssize_t digitalOut = findIndex(n, parametersToDigital);
#ifdef BELA_RNBO_USE_TRILL
		ssize_t trillLocation = findIndex(n, parametersFromTrillLocation);
		ssize_t trillSize = findIndex(n, parametersFromTrillSize);
#endif // BELA_RNBO_USE_TRILL
		if(analog >= 0)
			printf(" - controlled by analog in %d", analog);
		if(digitalIn >= 0) {
			printf(" - controlled by digital in %d", digitalIn);
			pinMode(context, 0, digitalIn, INPUT);
		}
		if(digitalOut >= 0) {
			printf(" - controlling digital out %d", digitalOut);
			pinMode(context, 0, digitalOut, OUTPUT);
		}
#ifdef BELA_RNBO_USE_TRILL
		if(trillLocation >= 0)
			printf(" - controlled by Trill location %d", trillLocation);
		if(trillSize >= 0)
			printf(" - controlled by Trill size %d", trillSize);
#endif // BELA_RNBO_USE_TRILL
		printf("\n");
		if(analog >= 0 && digitalIn >= 0)
			fprintf(stderr, "Parameter %d controlled by both analog and digital in. Digital in ignored\n", n);
	}
	if(hiddenParameters)
		printf("(%d hidden parameters)\n", hiddenParameters);
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

template <typename T, typename F, typename A>
static void sendOnChange(std::vector<T>& past, std::vector<unsigned int>& parameters, F func, A arg = (void*)nullptr)
{
	for(unsigned int c = 0; c < parameters.size(); ++c)
	{
		float value = func(c, arg);
		// only send on change
		if(value != past[c])
		{
			if(kNoParam != parameters[c])
			{
				rnbo->setParameterValueNormalized(parameters[c], value);
				past[c] = value;
			}
		}
	}
}

void render(BelaContext *context, void *userData)
{
	unsigned int nFrames = context->audioFrames;
	unsigned int nAnalogParameters = parametersFromAnalog.size();
	for(unsigned int c = 0; c < nAnalogParameters; ++c)
	{
		if(kNoParam != parametersFromAnalog[c])
			rnbo->setParameterValueNormalized(parametersFromAnalog[c], analogReadNI(context, 0, c));
	}
	sendOnChange(digitalParametersPast, parametersFromDigital, [](unsigned int c, BelaContext* context ) -> float { return digitalRead(context, 0, c); }, context);
	for(unsigned int c = 0; c < parametersToDigital.size(); ++c)
	{
		if(kNoParam != parametersToDigital[c])
			digitalWrite(context, 0, c, rnbo->getParameterValue(parametersToDigital[c]) > 0.5f);
	}
#ifdef BELA_RNBO_USE_TRILL
	sendOnChange(trillLocationParametersPast, parametersFromTrillLocation, [](unsigned int c, void*) { return trills[c]->compoundTouchLocation(); });
	sendOnChange(trillSizeParametersPast, parametersFromTrillSize, [](unsigned int c, void*) { return trills[c]->compoundTouchSize(); });
#endif // BELA_RNBO_USE_TRILL

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
#ifdef BELA_RNBO_USE_TRILL
	for(auto& p : trills)
		delete p;
#endif // BELA_RNBO_USE_TRILL
	delete presetList;
	delete rnbo;
}
