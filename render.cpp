#include <Bela.h>
#include "RNBO.h"
#include <string>
#include <MiscUtilities.h>
#include <algorithm>

#define BELA_RNBO_USE_MIDI // uncomment to use MIDI
//#define BELA_RNBO_USE_TRILL // uncomment to use Trill
//#define BELA_RNBO_LOAD_DEPENDENCIES // uncomment to load dependencies such as audio files

// The code for BELA_RNBO_LOAD_DEPENDENCIES follows closely the example [here](https://rnbo.cycling74.com/learn/loading-file-dependencies)

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
// same but for mapping of trill RAW or DIFF pads
static std::vector<unsigned int> parametersFromTrillChannels = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29};
static std::vector<Trill*> trills {
	// add/edit more Trills here
	new Trill(1, Trill::CRAFT),
};
static std::vector<float> trillLocationParametersPast(parametersFromTrillLocation.size());
static std::vector<float> trillSizeParametersPast(parametersFromTrillSize.size());
static std::vector<float> trillChannelsParametersPast(parametersFromTrillChannels.size());
#endif // BELA_RNBO_USE_TRILL

// whether to show hidden parameters when printing the parameters list
bool showHiddenParameters = false;

// has to be a pointer to ensure that it gets initialised after
// initialisation for the static PlatformInterfaceStdLib platformInstance has already taken place
static RNBO::CoreObject* rnbo;
static RNBO::PresetList* presetList;
static std::vector<bool> digitalParametersPast(parametersFromDigital.size());
static RNBO::MidiEventList midiInput; // whether it's actually used or not depends on BELA_RNBO_USE_MIDI
#ifdef BELA_RNBO_USE_MIDI
static bool useMidi = false;
#include <libraries/Midi/Midi.h>
static Midi midi;
#endif // BELA_RNBO_USE_MIDI

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

#ifdef BELA_RNBO_LOAD_DEPENDENCIES
#include <iostream>
#include <fstream>
#include <sstream>
#include <libraries/sndfile/sndfile.h>

// since we used malloc to allocate our buffer, we pass a callback that uses
// free to release buffer memory when RNBO is done with it.
static void freeBuffer(RNBO::ExternalDataId id, char *data) {
	std::cout << "--- Freed" << "\n";
	free(data);
}
#endif // BELA_RNBO_LOAD_DEPENDENCIES

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
#ifdef BELA_RNBO_USE_MIDI
	useMidi = rnbo->getNumMidiInputPorts() > 0;
	if(useMidi)
	{
		midi.readFrom("hw:0,0,0");
		midi.enableParser(true);
	}
#endif // BELA_RNBO_USE_MIDI
#ifdef BELA_RNBO_LOAD_DEPENDENCIES
	// Read in the dependencies.json file as a std::string
	const std::string dependenciesJson("dependencies.json");
	std::ifstream t(dependenciesJson);
	std::stringstream buffer;
	buffer << t.rdbuf();

	// Parse dependencies into a RNBO DataRefList
	try {
		RNBO::DataRefList list(buffer.str());

		// Loop and load
		for (int i = 0; i < list.size(); i++) {
			// Get parsed info about this data reference
			std::string idstr = list.datarefIdAtIndex(i);
			RNBO::DataRefType type = list.datarefTypeAtIndex(i);
			std::string location = list.datarefLocationAtIndex(i);

			// The type can be either URL or File
			if (type == RNBO::DataRefType::File) {
				std::cout << "buffer id: " << idstr << "\n";
				std::cout << "file path: " << list.datarefLocationAtIndex(i) << "\n";

				std::string filepath = list.datarefLocationAtIndex(i);

				SF_INFO info;
				info.format = 0;
				SNDFILE *sf = sf_open(filepath.c_str(), SFM_READ, &info);

				// Use the file info to make a type
				RNBO::Float32AudioBuffer bufferType(info.channels, info.samplerate);

				if (sf) {
					// Make space to store the file
					const uint32_t sampleBufferSize = sizeof(float) * info.frames * info.channels;
					float *sampleBuffer = (float *) malloc(sampleBufferSize);

					int readSamples = sf_read_float(sf, (float *) sampleBuffer, sampleBufferSize);

					rnbo->setExternalData(
					idstr.c_str(),
					(char *) sampleBuffer,
					readSamples * sizeof(float) / sizeof(char),
					bufferType,
					&freeBuffer
					);
					std::cout << "--- Success: Read " << readSamples << " samples" << "\n";

					sf_close(sf);
				} else {
					std::cout << "--- Failed" << "\n";
				}
			}
		}
	} catch (std::exception& e) {
		std::cerr << "Error loading" << dependenciesJson << ": " << e.what() << "\n";
	}
#endif // BELA_RNBO_LOAD_DEPENDENCIES
	// map I/Os
	parametersFromAnalog.resize(std::min(parametersFromAnalog.size(), context->analogInChannels));
	parametersFromDigital.resize(std::min(parametersFromDigital.size(), context->digitalChannels));
#ifdef BELA_RNBO_USE_TRILL
	parametersFromTrillLocation.resize(std::min(parametersFromTrillLocation.size(), trills.size()));
	parametersFromTrillSize.resize(std::min(parametersFromTrillSize.size(), trills.size()));
	parametersFromTrillChannels.resize(trills.size() ? trills[0].rawData.size());
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
		ssize_t trillChannels = findIndex(n, parametersFromTrillChannels);
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
		if(trillChannels >= 0)
			printf(" - controlled by Trill channels %d", trillChannels);
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

template <typename T, typename F, typename A = void*>
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
#ifdef BELA_RNBO_USE_MIDI
	if(useMidi)
	{
		midiInput.clear();
		int num;
		while((num = midi.getParser()->numAvailableMessages()) > 0)
		{
			MidiChannelMessage message = midi.getParser()->getNextChannelMessage();
			size_t numDataBytes = message.getNumDataBytes();
			size_t msgSize = numDataBytes + 1;
			midi_byte_t msg[msgSize];
			msg[0] = message.getStatusByte();
			for(size_t n = 0; n < numDataBytes; ++n)
				msg[1 + n] = message.getDataByte(n);
			midiInput.addEvent(RNBO::MidiEvent(0, 0, msg, msgSize));
		}
	}
#endif // BELA_RNBO_USE_MIDI
#ifdef BELA_RNBO_USE_TRILL
	sendOnChange(trillLocationParametersPast, parametersFromTrillLocation, [](unsigned int c, void*) -> float { return trills[c]->compoundTouchLocation(); });
	sendOnChange(trillSizeParametersPast, parametersFromTrillSize, [](unsigned int c, void*) -> float { return trills[c]->compoundTouchSize(); });
	sendOnChange(trillChannelsParametersPast, parametersFromChannelslSize, [](unsigned int c, void*) -> float { return trills[0]->rawData[c]; });
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
	rnbo->process(inputs, nInChannels, outputs, nOutChannels, nFrames, &midiInput);
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
