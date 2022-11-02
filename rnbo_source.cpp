/*******************************************************************************************************************
Cycling '74 License for Max-Generated Code for Export
Copyright (c) 2022 Cycling '74
The code that Max generates automatically and that end users are capable of exporting and using, and any
  associated documentation files (the “Software”) is a work of authorship for which Cycling '74 is the author
  and owner for copyright purposes.  A license is hereby granted, free of charge, to any person obtaining a
  copy of the Software (“Licensee”) to use, copy, modify, merge, publish, and distribute copies of the Software,
  and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The Software is licensed to Licensee only for non-commercial use. Users who wish to make commercial use of the
  Software must contact the copyright owner to determine if a license for commercial use is available, and the
  terms and conditions for same, which may include fees or royalties. For commercial use, please send inquiries
  to licensing@cycling74.com.  The determination of whether a use is commercial use or non-commercial use is based
  upon the use, not the user. The Software may be used by individuals, institutions, governments, corporations, or
  other business whether for-profit or non-profit so long as the use itself is not a commercialization of the
  materials or a use that generates or is intended to generate income, revenue, sales or profit.
The above copyright notice and this license shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
  THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.
*******************************************************************************************************************/

#include "RNBO_Common.h"
#include "RNBO_AudioSignal.h"

namespace RNBO {


#define floor(x) ((long)(x))

#if defined(__GNUC__) || defined(__clang__)
    #define RNBO_RESTRICT __restrict__
#elif defined(_MSC_VER)
    #define RNBO_RESTRICT __restrict
#endif

#define FIXEDSIZEARRAYINIT(...) { }

class rnbomatic : public PatcherInterfaceImpl {
public:

class RNBOSubpatcher_13 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_13()
    {
    }
    
    ~RNBOSubpatcher_13()
    {
    }
    
    virtual rnbomatic* getPatcher() const {
        return static_cast<rnbomatic *>(_parentPatcher);
    }
    
    rnbomatic* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
        getEngine()->flushClockEvents(this, -1494586265, false);
        getEngine()->flushClockEvents(this, 2098551528, false);
    }
    
    inline number safediv(number num, number denom) {
        return (denom == 0.0 ? 0.0 : num / denom);
    }
    
    number safepow(number base, number exponent) {
        return fixnan(rnbo_pow(base, exponent));
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    ParameterIndex getParameterIndexForID(ConstCharPointer paramid) const {
        RNBO_UNUSED(paramid);
        return INVALID_INDEX;
    }
    
    Index getNumMidiInputPorts() const {
        return 0;
    }
    
    void processMidiEvent(MillisecondTime , int , ConstByteArray , Index ) {}
    
    Index getNumMidiOutputPorts() const {
        return 0;
    }
    
    void process(
        SampleValue ** inputs,
        Index numInputs,
        SampleValue ** outputs,
        Index numOutputs,
        Index n
    ) {
        this->vs = n;
        this->updateTime(this->getEngine()->getCurrentTime());
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        SampleValue * out2 = (numOutputs >= 2 && outputs[1] ? outputs[1] : this->dummyBuffer);
        SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        SampleValue * in4 = (numInputs >= 4 && inputs[3] ? inputs[3] : this->zeroBuffer);
        SampleValue * in5 = (numInputs >= 5 && inputs[4] ? inputs[4] : this->zeroBuffer);
        this->numbertilde_02_perform(in4, this->dummyBuffer, n);
    
        this->scale_tilde_02_perform(
            in4,
            this->scale_tilde_02_lowin,
            in5,
            this->scale_tilde_02_lowout,
            this->scale_tilde_02_highout,
            this->scale_tilde_02_pow,
            this->signals[0],
            n
        );
    
        this->onepole_tilde_02_perform(in2, this->signals[0], this->signals[1], n);
        this->dspexpr_02_perform(in2, this->signals[1], out2, n);
        this->numbertilde_01_perform(this->signals[0], this->dummyBuffer, n);
    
        this->scale_tilde_01_perform(
            in3,
            this->scale_tilde_01_lowin,
            in5,
            this->scale_tilde_01_lowout,
            this->scale_tilde_01_highout,
            this->scale_tilde_01_pow,
            this->signals[0],
            n
        );
    
        this->onepole_tilde_01_perform(in1, this->signals[0], this->signals[1], n);
        this->dspexpr_01_perform(in1, this->signals[1], out1, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 2; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
            this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
            this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
            this->didAllocateSignals = true;
        }
    
        const bool sampleRateChanged = sampleRate != this->sr;
        const bool maxvsChanged = maxBlockSize != this->maxvs;
        const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;
    
        if (sampleRateChanged || maxvsChanged) {
            this->vs = maxBlockSize;
            this->maxvs = maxBlockSize;
            this->sr = sampleRate;
            this->invsr = 1 / sampleRate;
        }
    
        this->numbertilde_02_dspsetup(forceDSPSetup);
        this->onepole_tilde_02_dspsetup(forceDSPSetup);
        this->numbertilde_01_dspsetup(forceDSPSetup);
        this->onepole_tilde_01_dspsetup(forceDSPSetup);
    
        if (sampleRateChanged)
            this->onSampleRateChanged(sampleRate);
    }
    
    void setProbingTarget(MessageTag id) {
        switch (id) {
        default:
            this->setProbingIndex(-1);
            break;
        }
    }
    
    void setProbingIndex(ProbingIndex ) {}
    
    Index getProbingChannels(MessageTag outletId) const {
        RNBO_UNUSED(outletId);
        return 0;
    }
    
    Index getIsMuted()  {
        return this->isMuted;
    }
    
    void setIsMuted(Index v)  {
        this->isMuted = v;
    }
    
    Index getPatcherSerial() const {
        return 0;
    }
    
    void getState(PatcherStateInterface& ) {}
    
    void setState() {}
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
        default:
            return 0;
        }
    }
    
    ParameterIndex getNumSignalInParameters() const {
        return 0;
    }
    
    ParameterIndex getNumSignalOutParameters() const {
        return 0;
    }
    
    ParameterIndex getNumParameters() const {
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
    }
    
    ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
        if (steps == 1) {
            if (normalizedValue > 0) {
                normalizedValue = 1.;
            }
        } else {
            ParameterValue oneStep = (number)1. / (steps - 1);
            ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
            normalizedValue = numberOfSteps * oneStep;
        }
    
        return normalizedValue;
    }
    
    ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        default:
            return value;
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
    }
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case -1494586265:
            this->numbertilde_02_value_set(value);
            break;
        case 2098551528:
            this->numbertilde_01_value_set(value);
            break;
        }
    }
    
    void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}
    
    void processOutletEvent(
        EngineLink* sender,
        OutletIndex index,
        ParameterValue value,
        MillisecondTime time
    ) {
        this->updateTime(time);
        this->processOutletAtCurrentTime(sender, index, value);
    }
    
    void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("sig"):
            if (TAG("LowCut~/number~_obj-25") == objectId)
                this->numbertilde_01_sig_number_set(payload);
    
            if (TAG("LowCut~/number~_obj-26") == objectId)
                this->numbertilde_02_sig_number_set(payload);
    
            break;
        case TAG("mode"):
            if (TAG("LowCut~/number~_obj-25") == objectId)
                this->numbertilde_01_mode_set(payload);
    
            if (TAG("LowCut~/number~_obj-26") == objectId)
                this->numbertilde_02_mode_set(payload);
    
            break;
        }
    }
    
    void processListMessage(
        MessageTag tag,
        MessageTag objectId,
        MillisecondTime time,
        const list& payload
    ) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("sig"):
            if (TAG("LowCut~/number~_obj-25") == objectId)
                this->numbertilde_01_sig_list_set(payload);
    
            if (TAG("LowCut~/number~_obj-26") == objectId)
                this->numbertilde_02_sig_list_set(payload);
    
            break;
        }
    }
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("monitor"):
            return "monitor";
        case TAG("LowCut~/number~_obj-25"):
            return "LowCut~/number~_obj-25";
        case TAG("assign"):
            return "assign";
        case TAG("setup"):
            return "setup";
        case TAG("LowCut~/number~_obj-26"):
            return "LowCut~/number~_obj-26";
        case TAG("sig"):
            return "sig";
        case TAG("mode"):
            return "mode";
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        default:
            return nullptr;
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 0;
    }
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void processDataViewUpdate(DataRefIndex , MillisecondTime ) {}
    
    void initialize() {
        this->assign_defaults();
        this->setState();
    }
    
    protected:
    
    void numbertilde_01_sig_number_set(number v) {
        this->numbertilde_01_outValue = v;
    }
    
    void numbertilde_01_sig_list_set(const list& v) {
        this->numbertilde_01_outValue = v[0];
    }
    
    void numbertilde_01_mode_set(number v) {
        if (v == 1) {
            this->numbertilde_01_currentMode = 0;
        } else if (v == 2) {
            this->numbertilde_01_currentMode = 1;
        }
    }
    
    void numbertilde_02_sig_number_set(number v) {
        this->numbertilde_02_outValue = v;
    }
    
    void numbertilde_02_sig_list_set(const list& v) {
        this->numbertilde_02_outValue = v[0];
    }
    
    void numbertilde_02_mode_set(number v) {
        if (v == 1) {
            this->numbertilde_02_currentMode = 0;
        } else if (v == 2) {
            this->numbertilde_02_currentMode = 1;
        }
    }
    
    void numbertilde_02_value_set(number ) {}
    
    void numbertilde_01_value_set(number ) {}
    
    number msToSamps(MillisecondTime ms, number sampleRate) {
        return ms * sampleRate * 0.001;
    }
    
    MillisecondTime sampsToMs(SampleIndex samps) {
        return samps * (this->invsr * 1000);
    }
    
    Index getMaxBlockSize() const {
        return this->maxvs;
    }
    
    number getSampleRate() const {
        return this->sr;
    }
    
    bool hasFixedVectorSize() const {
        return false;
    }
    
    Index getNumInputChannels() const {
        return 5;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {
        this->numbertilde_01_init();
        this->numbertilde_02_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {}
    
    void numbertilde_02_perform(const Sample * input_signal, Sample * output, Index n) {
        auto __numbertilde_02_currentIntervalInSamples = this->numbertilde_02_currentIntervalInSamples;
        auto __numbertilde_02_lastValue = this->numbertilde_02_lastValue;
        auto __numbertilde_02_currentInterval = this->numbertilde_02_currentInterval;
        auto __numbertilde_02_rampInSamples = this->numbertilde_02_rampInSamples;
        auto __numbertilde_02_outValue = this->numbertilde_02_outValue;
        auto __numbertilde_02_currentMode = this->numbertilde_02_currentMode;
        number monitorvalue = 0;
    
        for (Index i = 0; i < n; i++) {
            monitorvalue += input_signal[(Index)i];
    
            if (__numbertilde_02_currentMode == 0) {
                output[(Index)i] = this->numbertilde_02_smooth_next(
                    __numbertilde_02_outValue,
                    __numbertilde_02_rampInSamples,
                    __numbertilde_02_rampInSamples
                );
            } else {
                output[(Index)i] = input_signal[(Index)i];
            }
        }
    
        __numbertilde_02_currentInterval -= n;
        monitorvalue /= n;
    
        if (monitorvalue != __numbertilde_02_lastValue && __numbertilde_02_currentInterval <= 0) {
            __numbertilde_02_currentInterval = __numbertilde_02_currentIntervalInSamples;
    
            this->getEngine()->scheduleClockEventWithValue(
                this,
                -1494586265,
                this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                monitorvalue
            );;
    
            __numbertilde_02_lastValue = monitorvalue;
    
            this->getEngine()->sendListMessage(
                TAG("monitor"),
                TAG("LowCut~/number~_obj-26"),
                {monitorvalue},
                this->_currentTime
            );;
        }
    
        this->numbertilde_02_currentInterval = __numbertilde_02_currentInterval;
        this->numbertilde_02_lastValue = __numbertilde_02_lastValue;
    }
    
    void scale_tilde_02_perform(
        const Sample * x,
        number lowin,
        const Sample * hiin,
        number lowout,
        number highout,
        number pow,
        Sample * out1,
        Index n
    ) {
        RNBO_UNUSED(pow);
        RNBO_UNUSED(highout);
        RNBO_UNUSED(lowout);
        RNBO_UNUSED(lowin);
        number outdiff = 250 - 50;
        Index i;
    
        for (i = 0; i < n; i++) {
            number inscale = this->safediv(1., hiin[(Index)i] - 0);
            number value = (x[(Index)i] - 0) * inscale;
            value = value * outdiff + 50;
            out1[(Index)i] = value;
        }
    }
    
    void onepole_tilde_02_perform(const Sample * x, const Sample * freqInHz, Sample * out1, Index n) {
        auto __onepole_tilde_02_lastY = this->onepole_tilde_02_lastY;
        auto __onepole_tilde_02_b1 = this->onepole_tilde_02_b1;
        auto __onepole_tilde_02_a0 = this->onepole_tilde_02_a0;
        auto __onepole_tilde_02_needsUpdate = this->onepole_tilde_02_needsUpdate;
        auto __onepole_tilde_02_freq = this->onepole_tilde_02_freq;
        Index i;
    
        for (i = 0; i < n; i++) {
            if (__onepole_tilde_02_freq != freqInHz[(Index)i] || (bool)(__onepole_tilde_02_needsUpdate)) {
                __onepole_tilde_02_freq = freqInHz[(Index)i];
                __onepole_tilde_02_a0 = 1 - rnbo_exp(-6.28318530717958647692 * freqInHz[(Index)i] / this->sr);
                __onepole_tilde_02_a0 = (__onepole_tilde_02_a0 > 0.99999 ? 0.99999 : (__onepole_tilde_02_a0 < 0.00001 ? 0.00001 : __onepole_tilde_02_a0));
                __onepole_tilde_02_b1 = 1 - __onepole_tilde_02_a0;
                __onepole_tilde_02_needsUpdate = false;
            }
    
            __onepole_tilde_02_lastY = __onepole_tilde_02_a0 * x[(Index)i] + __onepole_tilde_02_b1 * __onepole_tilde_02_lastY;
            out1[(Index)i] = __onepole_tilde_02_lastY;
        }
    
        this->onepole_tilde_02_freq = __onepole_tilde_02_freq;
        this->onepole_tilde_02_needsUpdate = __onepole_tilde_02_needsUpdate;
        this->onepole_tilde_02_a0 = __onepole_tilde_02_a0;
        this->onepole_tilde_02_b1 = __onepole_tilde_02_b1;
        this->onepole_tilde_02_lastY = __onepole_tilde_02_lastY;
    }
    
    void dspexpr_02_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] - in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void numbertilde_01_perform(const Sample * input_signal, Sample * output, Index n) {
        auto __numbertilde_01_currentIntervalInSamples = this->numbertilde_01_currentIntervalInSamples;
        auto __numbertilde_01_lastValue = this->numbertilde_01_lastValue;
        auto __numbertilde_01_currentInterval = this->numbertilde_01_currentInterval;
        auto __numbertilde_01_rampInSamples = this->numbertilde_01_rampInSamples;
        auto __numbertilde_01_outValue = this->numbertilde_01_outValue;
        auto __numbertilde_01_currentMode = this->numbertilde_01_currentMode;
        number monitorvalue = 0;
    
        for (Index i = 0; i < n; i++) {
            monitorvalue += input_signal[(Index)i];
    
            if (__numbertilde_01_currentMode == 0) {
                output[(Index)i] = this->numbertilde_01_smooth_next(
                    __numbertilde_01_outValue,
                    __numbertilde_01_rampInSamples,
                    __numbertilde_01_rampInSamples
                );
            } else {
                output[(Index)i] = input_signal[(Index)i];
            }
        }
    
        __numbertilde_01_currentInterval -= n;
        monitorvalue /= n;
    
        if (monitorvalue != __numbertilde_01_lastValue && __numbertilde_01_currentInterval <= 0) {
            __numbertilde_01_currentInterval = __numbertilde_01_currentIntervalInSamples;
    
            this->getEngine()->scheduleClockEventWithValue(
                this,
                2098551528,
                this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                monitorvalue
            );;
    
            __numbertilde_01_lastValue = monitorvalue;
    
            this->getEngine()->sendListMessage(
                TAG("monitor"),
                TAG("LowCut~/number~_obj-25"),
                {monitorvalue},
                this->_currentTime
            );;
        }
    
        this->numbertilde_01_currentInterval = __numbertilde_01_currentInterval;
        this->numbertilde_01_lastValue = __numbertilde_01_lastValue;
    }
    
    void scale_tilde_01_perform(
        const Sample * x,
        number lowin,
        const Sample * hiin,
        number lowout,
        number highout,
        number pow,
        Sample * out1,
        Index n
    ) {
        RNBO_UNUSED(pow);
        RNBO_UNUSED(highout);
        RNBO_UNUSED(lowout);
        RNBO_UNUSED(lowin);
        number outdiff = 250 - 50;
        Index i;
    
        for (i = 0; i < n; i++) {
            number inscale = this->safediv(1., hiin[(Index)i] - 0);
            number value = (x[(Index)i] - 0) * inscale;
            value = value * outdiff + 50;
            out1[(Index)i] = value;
        }
    }
    
    void onepole_tilde_01_perform(const Sample * x, const Sample * freqInHz, Sample * out1, Index n) {
        auto __onepole_tilde_01_lastY = this->onepole_tilde_01_lastY;
        auto __onepole_tilde_01_b1 = this->onepole_tilde_01_b1;
        auto __onepole_tilde_01_a0 = this->onepole_tilde_01_a0;
        auto __onepole_tilde_01_needsUpdate = this->onepole_tilde_01_needsUpdate;
        auto __onepole_tilde_01_freq = this->onepole_tilde_01_freq;
        Index i;
    
        for (i = 0; i < n; i++) {
            if (__onepole_tilde_01_freq != freqInHz[(Index)i] || (bool)(__onepole_tilde_01_needsUpdate)) {
                __onepole_tilde_01_freq = freqInHz[(Index)i];
                __onepole_tilde_01_a0 = 1 - rnbo_exp(-6.28318530717958647692 * freqInHz[(Index)i] / this->sr);
                __onepole_tilde_01_a0 = (__onepole_tilde_01_a0 > 0.99999 ? 0.99999 : (__onepole_tilde_01_a0 < 0.00001 ? 0.00001 : __onepole_tilde_01_a0));
                __onepole_tilde_01_b1 = 1 - __onepole_tilde_01_a0;
                __onepole_tilde_01_needsUpdate = false;
            }
    
            __onepole_tilde_01_lastY = __onepole_tilde_01_a0 * x[(Index)i] + __onepole_tilde_01_b1 * __onepole_tilde_01_lastY;
            out1[(Index)i] = __onepole_tilde_01_lastY;
        }
    
        this->onepole_tilde_01_freq = __onepole_tilde_01_freq;
        this->onepole_tilde_01_needsUpdate = __onepole_tilde_01_needsUpdate;
        this->onepole_tilde_01_a0 = __onepole_tilde_01_a0;
        this->onepole_tilde_01_b1 = __onepole_tilde_01_b1;
        this->onepole_tilde_01_lastY = __onepole_tilde_01_lastY;
    }
    
    void dspexpr_01_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] - in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void onepole_tilde_01_reset() {
        this->onepole_tilde_01_lastY = 0;
        this->onepole_tilde_01_a0 = 0;
        this->onepole_tilde_01_b1 = 0;
    }
    
    void onepole_tilde_01_dspsetup(bool force) {
        if ((bool)(this->onepole_tilde_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->onepole_tilde_01_needsUpdate = true;
        this->onepole_tilde_01_reset();
        this->onepole_tilde_01_setupDone = true;
    }
    
    void onepole_tilde_02_reset() {
        this->onepole_tilde_02_lastY = 0;
        this->onepole_tilde_02_a0 = 0;
        this->onepole_tilde_02_b1 = 0;
    }
    
    void onepole_tilde_02_dspsetup(bool force) {
        if ((bool)(this->onepole_tilde_02_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->onepole_tilde_02_needsUpdate = true;
        this->onepole_tilde_02_reset();
        this->onepole_tilde_02_setupDone = true;
    }
    
    number numbertilde_01_smooth_d_next(number x) {
        number temp = (number)(x - this->numbertilde_01_smooth_d_prev);
        this->numbertilde_01_smooth_d_prev = x;
        return temp;
    }
    
    void numbertilde_01_smooth_d_dspsetup() {
        this->numbertilde_01_smooth_d_reset();
    }
    
    void numbertilde_01_smooth_d_reset() {
        this->numbertilde_01_smooth_d_prev = 0;
    }
    
    number numbertilde_01_smooth_next(number x, number up, number down) {
        if (this->numbertilde_01_smooth_d_next(x) != 0.) {
            if (x > this->numbertilde_01_smooth_prev) {
                number _up = up;
    
                if (_up < 1)
                    _up = 1;
    
                this->numbertilde_01_smooth_index = _up;
                this->numbertilde_01_smooth_increment = (x - this->numbertilde_01_smooth_prev) / _up;
            } else if (x < this->numbertilde_01_smooth_prev) {
                number _down = down;
    
                if (_down < 1)
                    _down = 1;
    
                this->numbertilde_01_smooth_index = _down;
                this->numbertilde_01_smooth_increment = (x - this->numbertilde_01_smooth_prev) / _down;
            }
        }
    
        if (this->numbertilde_01_smooth_index > 0) {
            this->numbertilde_01_smooth_prev += this->numbertilde_01_smooth_increment;
            this->numbertilde_01_smooth_index -= 1;
        } else {
            this->numbertilde_01_smooth_prev = x;
        }
    
        return this->numbertilde_01_smooth_prev;
    }
    
    void numbertilde_01_smooth_reset() {
        this->numbertilde_01_smooth_prev = 0;
        this->numbertilde_01_smooth_index = 0;
        this->numbertilde_01_smooth_increment = 0;
        this->numbertilde_01_smooth_d_reset();
    }
    
    void numbertilde_01_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("LowCut~/number~_obj-25"), 1, this->_currentTime);
    }
    
    void numbertilde_01_dspsetup(bool force) {
        if ((bool)(this->numbertilde_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->numbertilde_01_currentIntervalInSamples = this->mstosamps(100);
        this->numbertilde_01_currentInterval = this->numbertilde_01_currentIntervalInSamples;
        this->numbertilde_01_rampInSamples = this->mstosamps(this->numbertilde_01_ramp);
        this->numbertilde_01_setupDone = true;
        this->numbertilde_01_smooth_d_dspsetup();
    }
    
    number numbertilde_02_smooth_d_next(number x) {
        number temp = (number)(x - this->numbertilde_02_smooth_d_prev);
        this->numbertilde_02_smooth_d_prev = x;
        return temp;
    }
    
    void numbertilde_02_smooth_d_dspsetup() {
        this->numbertilde_02_smooth_d_reset();
    }
    
    void numbertilde_02_smooth_d_reset() {
        this->numbertilde_02_smooth_d_prev = 0;
    }
    
    number numbertilde_02_smooth_next(number x, number up, number down) {
        if (this->numbertilde_02_smooth_d_next(x) != 0.) {
            if (x > this->numbertilde_02_smooth_prev) {
                number _up = up;
    
                if (_up < 1)
                    _up = 1;
    
                this->numbertilde_02_smooth_index = _up;
                this->numbertilde_02_smooth_increment = (x - this->numbertilde_02_smooth_prev) / _up;
            } else if (x < this->numbertilde_02_smooth_prev) {
                number _down = down;
    
                if (_down < 1)
                    _down = 1;
    
                this->numbertilde_02_smooth_index = _down;
                this->numbertilde_02_smooth_increment = (x - this->numbertilde_02_smooth_prev) / _down;
            }
        }
    
        if (this->numbertilde_02_smooth_index > 0) {
            this->numbertilde_02_smooth_prev += this->numbertilde_02_smooth_increment;
            this->numbertilde_02_smooth_index -= 1;
        } else {
            this->numbertilde_02_smooth_prev = x;
        }
    
        return this->numbertilde_02_smooth_prev;
    }
    
    void numbertilde_02_smooth_reset() {
        this->numbertilde_02_smooth_prev = 0;
        this->numbertilde_02_smooth_index = 0;
        this->numbertilde_02_smooth_increment = 0;
        this->numbertilde_02_smooth_d_reset();
    }
    
    void numbertilde_02_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("LowCut~/number~_obj-26"), 1, this->_currentTime);
    }
    
    void numbertilde_02_dspsetup(bool force) {
        if ((bool)(this->numbertilde_02_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->numbertilde_02_currentIntervalInSamples = this->mstosamps(100);
        this->numbertilde_02_currentInterval = this->numbertilde_02_currentIntervalInSamples;
        this->numbertilde_02_rampInSamples = this->mstosamps(this->numbertilde_02_ramp);
        this->numbertilde_02_setupDone = true;
        this->numbertilde_02_smooth_d_dspsetup();
    }
    
    bool stackprotect_check() {
        this->stackprotect_count++;
    
        if (this->stackprotect_count > 128) {
            console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
            return true;
        }
    
        return false;
    }
    
    void updateTime(MillisecondTime time) {
        this->_currentTime = time;
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));
    
        if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
            this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;
    
        if (this->sampleOffsetIntoNextAudioBuffer < 0)
            this->sampleOffsetIntoNextAudioBuffer = 0;
    }
    
    void assign_defaults()
    {
        dspexpr_01_in1 = 0;
        dspexpr_01_in2 = 0;
        onepole_tilde_01_x = 0;
        onepole_tilde_01_freqInHz = 1;
        dspexpr_02_in1 = 0;
        dspexpr_02_in2 = 0;
        onepole_tilde_02_x = 0;
        onepole_tilde_02_freqInHz = 1;
        scale_tilde_01_x = 0;
        scale_tilde_01_lowin = 0;
        scale_tilde_01_hiin = 1;
        scale_tilde_01_lowout = 50;
        scale_tilde_01_highout = 250;
        scale_tilde_01_pow = 1;
        numbertilde_01_input_number = 0;
        numbertilde_01_ramp = 0;
        scale_tilde_02_x = 0;
        scale_tilde_02_lowin = 0;
        scale_tilde_02_hiin = 1;
        scale_tilde_02_lowout = 50;
        scale_tilde_02_highout = 250;
        scale_tilde_02_pow = 1;
        numbertilde_02_input_number = 0;
        numbertilde_02_ramp = 0;
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        signals[1] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        onepole_tilde_01_freq = 0;
        onepole_tilde_01_needsUpdate = false;
        onepole_tilde_01_lastY = 0;
        onepole_tilde_01_a0 = 0;
        onepole_tilde_01_b1 = 0;
        onepole_tilde_01_setupDone = false;
        onepole_tilde_02_freq = 0;
        onepole_tilde_02_needsUpdate = false;
        onepole_tilde_02_lastY = 0;
        onepole_tilde_02_a0 = 0;
        onepole_tilde_02_b1 = 0;
        onepole_tilde_02_setupDone = false;
        numbertilde_01_currentInterval = 0;
        numbertilde_01_currentIntervalInSamples = 0;
        numbertilde_01_lastValue = 0;
        numbertilde_01_outValue = 0;
        numbertilde_01_rampInSamples = 0;
        numbertilde_01_currentMode = 1;
        numbertilde_01_smooth_d_prev = 0;
        numbertilde_01_smooth_prev = 0;
        numbertilde_01_smooth_index = 0;
        numbertilde_01_smooth_increment = 0;
        numbertilde_01_setupDone = false;
        numbertilde_02_currentInterval = 0;
        numbertilde_02_currentIntervalInSamples = 0;
        numbertilde_02_lastValue = 0;
        numbertilde_02_outValue = 0;
        numbertilde_02_rampInSamples = 0;
        numbertilde_02_currentMode = 1;
        numbertilde_02_smooth_d_prev = 0;
        numbertilde_02_smooth_prev = 0;
        numbertilde_02_smooth_index = 0;
        numbertilde_02_smooth_increment = 0;
        numbertilde_02_setupDone = false;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number dspexpr_01_in1;
        number dspexpr_01_in2;
        number onepole_tilde_01_x;
        number onepole_tilde_01_freqInHz;
        number dspexpr_02_in1;
        number dspexpr_02_in2;
        number onepole_tilde_02_x;
        number onepole_tilde_02_freqInHz;
        number scale_tilde_01_x;
        number scale_tilde_01_lowin;
        number scale_tilde_01_hiin;
        number scale_tilde_01_lowout;
        number scale_tilde_01_highout;
        number scale_tilde_01_pow;
        number numbertilde_01_input_number;
        number numbertilde_01_ramp;
        number scale_tilde_02_x;
        number scale_tilde_02_lowin;
        number scale_tilde_02_hiin;
        number scale_tilde_02_lowout;
        number scale_tilde_02_highout;
        number scale_tilde_02_pow;
        number numbertilde_02_input_number;
        number numbertilde_02_ramp;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[2];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        number onepole_tilde_01_freq;
        bool onepole_tilde_01_needsUpdate;
        number onepole_tilde_01_lastY;
        number onepole_tilde_01_a0;
        number onepole_tilde_01_b1;
        bool onepole_tilde_01_setupDone;
        number onepole_tilde_02_freq;
        bool onepole_tilde_02_needsUpdate;
        number onepole_tilde_02_lastY;
        number onepole_tilde_02_a0;
        number onepole_tilde_02_b1;
        bool onepole_tilde_02_setupDone;
        SampleIndex numbertilde_01_currentInterval;
        SampleIndex numbertilde_01_currentIntervalInSamples;
        number numbertilde_01_lastValue;
        number numbertilde_01_outValue;
        number numbertilde_01_rampInSamples;
        Int numbertilde_01_currentMode;
        number numbertilde_01_smooth_d_prev;
        number numbertilde_01_smooth_prev;
        number numbertilde_01_smooth_index;
        number numbertilde_01_smooth_increment;
        bool numbertilde_01_setupDone;
        SampleIndex numbertilde_02_currentInterval;
        SampleIndex numbertilde_02_currentIntervalInSamples;
        number numbertilde_02_lastValue;
        number numbertilde_02_outValue;
        number numbertilde_02_rampInSamples;
        Int numbertilde_02_currentMode;
        number numbertilde_02_smooth_d_prev;
        number numbertilde_02_smooth_prev;
        number numbertilde_02_smooth_index;
        number numbertilde_02_smooth_increment;
        bool numbertilde_02_setupDone;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_14 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_14()
    {
    }
    
    ~RNBOSubpatcher_14()
    {
    }
    
    virtual rnbomatic* getPatcher() const {
        return static_cast<rnbomatic *>(_parentPatcher);
    }
    
    rnbomatic* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
        getEngine()->flushClockEvents(this, 2098551528, false);
        getEngine()->flushClockEvents(this, 1396722025, false);
        getEngine()->flushClockEvents(this, -1494586265, false);
    }
    
    inline number safediv(number num, number denom) {
        return (denom == 0.0 ? 0.0 : num / denom);
    }
    
    number safepow(number base, number exponent) {
        return fixnan(rnbo_pow(base, exponent));
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    ParameterIndex getParameterIndexForID(ConstCharPointer paramid) const {
        RNBO_UNUSED(paramid);
        return INVALID_INDEX;
    }
    
    Index getNumMidiInputPorts() const {
        return 0;
    }
    
    void processMidiEvent(MillisecondTime , int , ConstByteArray , Index ) {}
    
    Index getNumMidiOutputPorts() const {
        return 0;
    }
    
    void process(
        SampleValue ** inputs,
        Index numInputs,
        SampleValue ** outputs,
        Index numOutputs,
        Index n
    ) {
        this->vs = n;
        this->updateTime(this->getEngine()->getCurrentTime());
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        SampleValue * out2 = (numOutputs >= 2 && outputs[1] ? outputs[1] : this->dummyBuffer);
        SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        SampleValue * in4 = (numInputs >= 4 && inputs[3] ? inputs[3] : this->zeroBuffer);
        SampleValue * in5 = (numInputs >= 5 && inputs[4] ? inputs[4] : this->zeroBuffer);
        this->numbertilde_04_perform(in4, this->dummyBuffer, n);
    
        this->scale_tilde_04_perform(
            in4,
            this->scale_tilde_04_lowin,
            in5,
            this->scale_tilde_04_lowout,
            this->scale_tilde_04_highout,
            this->scale_tilde_04_pow,
            this->signals[0],
            n
        );
    
        this->onepole_tilde_04_perform(in2, this->signals[0], out2, n);
        this->numbertilde_03_perform(this->signals[0], this->dummyBuffer, n);
    
        this->scale_tilde_03_perform(
            in3,
            this->scale_tilde_03_lowin,
            in5,
            this->scale_tilde_03_lowout,
            this->scale_tilde_03_highout,
            this->scale_tilde_03_pow,
            this->signals[0],
            n
        );
    
        this->onepole_tilde_03_perform(in1, this->signals[0], out1, n);
        this->numbertilde_05_perform(in5, this->dummyBuffer, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 1; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
            this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
            this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
            this->didAllocateSignals = true;
        }
    
        const bool sampleRateChanged = sampleRate != this->sr;
        const bool maxvsChanged = maxBlockSize != this->maxvs;
        const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;
    
        if (sampleRateChanged || maxvsChanged) {
            this->vs = maxBlockSize;
            this->maxvs = maxBlockSize;
            this->sr = sampleRate;
            this->invsr = 1 / sampleRate;
        }
    
        this->numbertilde_04_dspsetup(forceDSPSetup);
        this->onepole_tilde_04_dspsetup(forceDSPSetup);
        this->numbertilde_03_dspsetup(forceDSPSetup);
        this->onepole_tilde_03_dspsetup(forceDSPSetup);
        this->numbertilde_05_dspsetup(forceDSPSetup);
    
        if (sampleRateChanged)
            this->onSampleRateChanged(sampleRate);
    }
    
    void setProbingTarget(MessageTag id) {
        switch (id) {
        default:
            this->setProbingIndex(-1);
            break;
        }
    }
    
    void setProbingIndex(ProbingIndex ) {}
    
    Index getProbingChannels(MessageTag outletId) const {
        RNBO_UNUSED(outletId);
        return 0;
    }
    
    Index getIsMuted()  {
        return this->isMuted;
    }
    
    void setIsMuted(Index v)  {
        this->isMuted = v;
    }
    
    Index getPatcherSerial() const {
        return 0;
    }
    
    void getState(PatcherStateInterface& ) {}
    
    void setState() {}
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
        default:
            return 0;
        }
    }
    
    ParameterIndex getNumSignalInParameters() const {
        return 0;
    }
    
    ParameterIndex getNumSignalOutParameters() const {
        return 0;
    }
    
    ParameterIndex getNumParameters() const {
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
    }
    
    ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
        if (steps == 1) {
            if (normalizedValue > 0) {
                normalizedValue = 1.;
            }
        } else {
            ParameterValue oneStep = (number)1. / (steps - 1);
            ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
            normalizedValue = numberOfSteps * oneStep;
        }
    
        return normalizedValue;
    }
    
    ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        default:
            return value;
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
    }
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case 2098551528:
            this->numbertilde_04_value_set(value);
            break;
        case 1396722025:
            this->numbertilde_03_value_set(value);
            break;
        case -1494586265:
            this->numbertilde_05_value_set(value);
            break;
        }
    }
    
    void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}
    
    void processOutletEvent(
        EngineLink* sender,
        OutletIndex index,
        ParameterValue value,
        MillisecondTime time
    ) {
        this->updateTime(time);
        this->processOutletAtCurrentTime(sender, index, value);
    }
    
    void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("sig"):
            if (TAG("HiCut~/number~_obj-25") == objectId)
                this->numbertilde_03_sig_number_set(payload);
    
            if (TAG("HiCut~/number~_obj-26") == objectId)
                this->numbertilde_04_sig_number_set(payload);
    
            if (TAG("HiCut~/number~_obj-1") == objectId)
                this->numbertilde_05_sig_number_set(payload);
    
            break;
        case TAG("mode"):
            if (TAG("HiCut~/number~_obj-25") == objectId)
                this->numbertilde_03_mode_set(payload);
    
            if (TAG("HiCut~/number~_obj-26") == objectId)
                this->numbertilde_04_mode_set(payload);
    
            if (TAG("HiCut~/number~_obj-1") == objectId)
                this->numbertilde_05_mode_set(payload);
    
            break;
        }
    }
    
    void processListMessage(
        MessageTag tag,
        MessageTag objectId,
        MillisecondTime time,
        const list& payload
    ) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("sig"):
            if (TAG("HiCut~/number~_obj-25") == objectId)
                this->numbertilde_03_sig_list_set(payload);
    
            if (TAG("HiCut~/number~_obj-26") == objectId)
                this->numbertilde_04_sig_list_set(payload);
    
            if (TAG("HiCut~/number~_obj-1") == objectId)
                this->numbertilde_05_sig_list_set(payload);
    
            break;
        }
    }
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("monitor"):
            return "monitor";
        case TAG("HiCut~/number~_obj-25"):
            return "HiCut~/number~_obj-25";
        case TAG("assign"):
            return "assign";
        case TAG("setup"):
            return "setup";
        case TAG("HiCut~/number~_obj-26"):
            return "HiCut~/number~_obj-26";
        case TAG("HiCut~/number~_obj-1"):
            return "HiCut~/number~_obj-1";
        case TAG("sig"):
            return "sig";
        case TAG("mode"):
            return "mode";
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        default:
            return nullptr;
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 0;
    }
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void processDataViewUpdate(DataRefIndex , MillisecondTime ) {}
    
    void initialize() {
        this->assign_defaults();
        this->setState();
    }
    
    protected:
    
    void numbertilde_03_sig_number_set(number v) {
        this->numbertilde_03_outValue = v;
    }
    
    void numbertilde_03_sig_list_set(const list& v) {
        this->numbertilde_03_outValue = v[0];
    }
    
    void numbertilde_03_mode_set(number v) {
        if (v == 1) {
            this->numbertilde_03_currentMode = 0;
        } else if (v == 2) {
            this->numbertilde_03_currentMode = 1;
        }
    }
    
    void numbertilde_04_sig_number_set(number v) {
        this->numbertilde_04_outValue = v;
    }
    
    void numbertilde_04_sig_list_set(const list& v) {
        this->numbertilde_04_outValue = v[0];
    }
    
    void numbertilde_04_mode_set(number v) {
        if (v == 1) {
            this->numbertilde_04_currentMode = 0;
        } else if (v == 2) {
            this->numbertilde_04_currentMode = 1;
        }
    }
    
    void numbertilde_05_sig_number_set(number v) {
        this->numbertilde_05_outValue = v;
    }
    
    void numbertilde_05_sig_list_set(const list& v) {
        this->numbertilde_05_outValue = v[0];
    }
    
    void numbertilde_05_mode_set(number v) {
        if (v == 1) {
            this->numbertilde_05_currentMode = 0;
        } else if (v == 2) {
            this->numbertilde_05_currentMode = 1;
        }
    }
    
    void numbertilde_04_value_set(number ) {}
    
    void numbertilde_03_value_set(number ) {}
    
    void numbertilde_05_value_set(number ) {}
    
    number msToSamps(MillisecondTime ms, number sampleRate) {
        return ms * sampleRate * 0.001;
    }
    
    MillisecondTime sampsToMs(SampleIndex samps) {
        return samps * (this->invsr * 1000);
    }
    
    Index getMaxBlockSize() const {
        return this->maxvs;
    }
    
    number getSampleRate() const {
        return this->sr;
    }
    
    bool hasFixedVectorSize() const {
        return false;
    }
    
    Index getNumInputChannels() const {
        return 5;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {
        this->numbertilde_03_init();
        this->numbertilde_04_init();
        this->numbertilde_05_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {}
    
    void numbertilde_04_perform(const Sample * input_signal, Sample * output, Index n) {
        auto __numbertilde_04_currentIntervalInSamples = this->numbertilde_04_currentIntervalInSamples;
        auto __numbertilde_04_lastValue = this->numbertilde_04_lastValue;
        auto __numbertilde_04_currentInterval = this->numbertilde_04_currentInterval;
        auto __numbertilde_04_rampInSamples = this->numbertilde_04_rampInSamples;
        auto __numbertilde_04_outValue = this->numbertilde_04_outValue;
        auto __numbertilde_04_currentMode = this->numbertilde_04_currentMode;
        number monitorvalue = 0;
    
        for (Index i = 0; i < n; i++) {
            monitorvalue += input_signal[(Index)i];
    
            if (__numbertilde_04_currentMode == 0) {
                output[(Index)i] = this->numbertilde_04_smooth_next(
                    __numbertilde_04_outValue,
                    __numbertilde_04_rampInSamples,
                    __numbertilde_04_rampInSamples
                );
            } else {
                output[(Index)i] = input_signal[(Index)i];
            }
        }
    
        __numbertilde_04_currentInterval -= n;
        monitorvalue /= n;
    
        if (monitorvalue != __numbertilde_04_lastValue && __numbertilde_04_currentInterval <= 0) {
            __numbertilde_04_currentInterval = __numbertilde_04_currentIntervalInSamples;
    
            this->getEngine()->scheduleClockEventWithValue(
                this,
                2098551528,
                this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                monitorvalue
            );;
    
            __numbertilde_04_lastValue = monitorvalue;
    
            this->getEngine()->sendListMessage(
                TAG("monitor"),
                TAG("HiCut~/number~_obj-26"),
                {monitorvalue},
                this->_currentTime
            );;
        }
    
        this->numbertilde_04_currentInterval = __numbertilde_04_currentInterval;
        this->numbertilde_04_lastValue = __numbertilde_04_lastValue;
    }
    
    void scale_tilde_04_perform(
        const Sample * x,
        number lowin,
        const Sample * hiin,
        number lowout,
        number highout,
        number pow,
        Sample * out1,
        Index n
    ) {
        RNBO_UNUSED(pow);
        RNBO_UNUSED(highout);
        RNBO_UNUSED(lowout);
        RNBO_UNUSED(lowin);
        number outdiff = 20000 - 1000;
        Index i;
    
        for (i = 0; i < n; i++) {
            number inscale = this->safediv(1., hiin[(Index)i] - 0);
            number value = (x[(Index)i] - 0) * inscale;
            value = value * outdiff + 1000;
            out1[(Index)i] = value;
        }
    }
    
    void onepole_tilde_04_perform(const Sample * x, const Sample * freqInHz, Sample * out1, Index n) {
        auto __onepole_tilde_04_lastY = this->onepole_tilde_04_lastY;
        auto __onepole_tilde_04_b1 = this->onepole_tilde_04_b1;
        auto __onepole_tilde_04_a0 = this->onepole_tilde_04_a0;
        auto __onepole_tilde_04_needsUpdate = this->onepole_tilde_04_needsUpdate;
        auto __onepole_tilde_04_freq = this->onepole_tilde_04_freq;
        Index i;
    
        for (i = 0; i < n; i++) {
            if (__onepole_tilde_04_freq != freqInHz[(Index)i] || (bool)(__onepole_tilde_04_needsUpdate)) {
                __onepole_tilde_04_freq = freqInHz[(Index)i];
                __onepole_tilde_04_a0 = 1 - rnbo_exp(-6.28318530717958647692 * freqInHz[(Index)i] / this->sr);
                __onepole_tilde_04_a0 = (__onepole_tilde_04_a0 > 0.99999 ? 0.99999 : (__onepole_tilde_04_a0 < 0.00001 ? 0.00001 : __onepole_tilde_04_a0));
                __onepole_tilde_04_b1 = 1 - __onepole_tilde_04_a0;
                __onepole_tilde_04_needsUpdate = false;
            }
    
            __onepole_tilde_04_lastY = __onepole_tilde_04_a0 * x[(Index)i] + __onepole_tilde_04_b1 * __onepole_tilde_04_lastY;
            out1[(Index)i] = __onepole_tilde_04_lastY;
        }
    
        this->onepole_tilde_04_freq = __onepole_tilde_04_freq;
        this->onepole_tilde_04_needsUpdate = __onepole_tilde_04_needsUpdate;
        this->onepole_tilde_04_a0 = __onepole_tilde_04_a0;
        this->onepole_tilde_04_b1 = __onepole_tilde_04_b1;
        this->onepole_tilde_04_lastY = __onepole_tilde_04_lastY;
    }
    
    void numbertilde_03_perform(const Sample * input_signal, Sample * output, Index n) {
        auto __numbertilde_03_currentIntervalInSamples = this->numbertilde_03_currentIntervalInSamples;
        auto __numbertilde_03_lastValue = this->numbertilde_03_lastValue;
        auto __numbertilde_03_currentInterval = this->numbertilde_03_currentInterval;
        auto __numbertilde_03_rampInSamples = this->numbertilde_03_rampInSamples;
        auto __numbertilde_03_outValue = this->numbertilde_03_outValue;
        auto __numbertilde_03_currentMode = this->numbertilde_03_currentMode;
        number monitorvalue = 0;
    
        for (Index i = 0; i < n; i++) {
            monitorvalue += input_signal[(Index)i];
    
            if (__numbertilde_03_currentMode == 0) {
                output[(Index)i] = this->numbertilde_03_smooth_next(
                    __numbertilde_03_outValue,
                    __numbertilde_03_rampInSamples,
                    __numbertilde_03_rampInSamples
                );
            } else {
                output[(Index)i] = input_signal[(Index)i];
            }
        }
    
        __numbertilde_03_currentInterval -= n;
        monitorvalue /= n;
    
        if (monitorvalue != __numbertilde_03_lastValue && __numbertilde_03_currentInterval <= 0) {
            __numbertilde_03_currentInterval = __numbertilde_03_currentIntervalInSamples;
    
            this->getEngine()->scheduleClockEventWithValue(
                this,
                1396722025,
                this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                monitorvalue
            );;
    
            __numbertilde_03_lastValue = monitorvalue;
    
            this->getEngine()->sendListMessage(
                TAG("monitor"),
                TAG("HiCut~/number~_obj-25"),
                {monitorvalue},
                this->_currentTime
            );;
        }
    
        this->numbertilde_03_currentInterval = __numbertilde_03_currentInterval;
        this->numbertilde_03_lastValue = __numbertilde_03_lastValue;
    }
    
    void scale_tilde_03_perform(
        const Sample * x,
        number lowin,
        const Sample * hiin,
        number lowout,
        number highout,
        number pow,
        Sample * out1,
        Index n
    ) {
        RNBO_UNUSED(pow);
        RNBO_UNUSED(highout);
        RNBO_UNUSED(lowout);
        RNBO_UNUSED(lowin);
        number outdiff = 20000 - 1000;
        Index i;
    
        for (i = 0; i < n; i++) {
            number inscale = this->safediv(1., hiin[(Index)i] - 0);
            number value = (x[(Index)i] - 0) * inscale;
            value = value * outdiff + 1000;
            out1[(Index)i] = value;
        }
    }
    
    void onepole_tilde_03_perform(const Sample * x, const Sample * freqInHz, Sample * out1, Index n) {
        auto __onepole_tilde_03_lastY = this->onepole_tilde_03_lastY;
        auto __onepole_tilde_03_b1 = this->onepole_tilde_03_b1;
        auto __onepole_tilde_03_a0 = this->onepole_tilde_03_a0;
        auto __onepole_tilde_03_needsUpdate = this->onepole_tilde_03_needsUpdate;
        auto __onepole_tilde_03_freq = this->onepole_tilde_03_freq;
        Index i;
    
        for (i = 0; i < n; i++) {
            if (__onepole_tilde_03_freq != freqInHz[(Index)i] || (bool)(__onepole_tilde_03_needsUpdate)) {
                __onepole_tilde_03_freq = freqInHz[(Index)i];
                __onepole_tilde_03_a0 = 1 - rnbo_exp(-6.28318530717958647692 * freqInHz[(Index)i] / this->sr);
                __onepole_tilde_03_a0 = (__onepole_tilde_03_a0 > 0.99999 ? 0.99999 : (__onepole_tilde_03_a0 < 0.00001 ? 0.00001 : __onepole_tilde_03_a0));
                __onepole_tilde_03_b1 = 1 - __onepole_tilde_03_a0;
                __onepole_tilde_03_needsUpdate = false;
            }
    
            __onepole_tilde_03_lastY = __onepole_tilde_03_a0 * x[(Index)i] + __onepole_tilde_03_b1 * __onepole_tilde_03_lastY;
            out1[(Index)i] = __onepole_tilde_03_lastY;
        }
    
        this->onepole_tilde_03_freq = __onepole_tilde_03_freq;
        this->onepole_tilde_03_needsUpdate = __onepole_tilde_03_needsUpdate;
        this->onepole_tilde_03_a0 = __onepole_tilde_03_a0;
        this->onepole_tilde_03_b1 = __onepole_tilde_03_b1;
        this->onepole_tilde_03_lastY = __onepole_tilde_03_lastY;
    }
    
    void numbertilde_05_perform(const Sample * input_signal, Sample * output, Index n) {
        auto __numbertilde_05_currentIntervalInSamples = this->numbertilde_05_currentIntervalInSamples;
        auto __numbertilde_05_lastValue = this->numbertilde_05_lastValue;
        auto __numbertilde_05_currentInterval = this->numbertilde_05_currentInterval;
        auto __numbertilde_05_rampInSamples = this->numbertilde_05_rampInSamples;
        auto __numbertilde_05_outValue = this->numbertilde_05_outValue;
        auto __numbertilde_05_currentMode = this->numbertilde_05_currentMode;
        number monitorvalue = 0;
    
        for (Index i = 0; i < n; i++) {
            monitorvalue += input_signal[(Index)i];
    
            if (__numbertilde_05_currentMode == 0) {
                output[(Index)i] = this->numbertilde_05_smooth_next(
                    __numbertilde_05_outValue,
                    __numbertilde_05_rampInSamples,
                    __numbertilde_05_rampInSamples
                );
            } else {
                output[(Index)i] = input_signal[(Index)i];
            }
        }
    
        __numbertilde_05_currentInterval -= n;
        monitorvalue /= n;
    
        if (monitorvalue != __numbertilde_05_lastValue && __numbertilde_05_currentInterval <= 0) {
            __numbertilde_05_currentInterval = __numbertilde_05_currentIntervalInSamples;
    
            this->getEngine()->scheduleClockEventWithValue(
                this,
                -1494586265,
                this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                monitorvalue
            );;
    
            __numbertilde_05_lastValue = monitorvalue;
    
            this->getEngine()->sendListMessage(
                TAG("monitor"),
                TAG("HiCut~/number~_obj-1"),
                {monitorvalue},
                this->_currentTime
            );;
        }
    
        this->numbertilde_05_currentInterval = __numbertilde_05_currentInterval;
        this->numbertilde_05_lastValue = __numbertilde_05_lastValue;
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void onepole_tilde_03_reset() {
        this->onepole_tilde_03_lastY = 0;
        this->onepole_tilde_03_a0 = 0;
        this->onepole_tilde_03_b1 = 0;
    }
    
    void onepole_tilde_03_dspsetup(bool force) {
        if ((bool)(this->onepole_tilde_03_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->onepole_tilde_03_needsUpdate = true;
        this->onepole_tilde_03_reset();
        this->onepole_tilde_03_setupDone = true;
    }
    
    void onepole_tilde_04_reset() {
        this->onepole_tilde_04_lastY = 0;
        this->onepole_tilde_04_a0 = 0;
        this->onepole_tilde_04_b1 = 0;
    }
    
    void onepole_tilde_04_dspsetup(bool force) {
        if ((bool)(this->onepole_tilde_04_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->onepole_tilde_04_needsUpdate = true;
        this->onepole_tilde_04_reset();
        this->onepole_tilde_04_setupDone = true;
    }
    
    number numbertilde_03_smooth_d_next(number x) {
        number temp = (number)(x - this->numbertilde_03_smooth_d_prev);
        this->numbertilde_03_smooth_d_prev = x;
        return temp;
    }
    
    void numbertilde_03_smooth_d_dspsetup() {
        this->numbertilde_03_smooth_d_reset();
    }
    
    void numbertilde_03_smooth_d_reset() {
        this->numbertilde_03_smooth_d_prev = 0;
    }
    
    number numbertilde_03_smooth_next(number x, number up, number down) {
        if (this->numbertilde_03_smooth_d_next(x) != 0.) {
            if (x > this->numbertilde_03_smooth_prev) {
                number _up = up;
    
                if (_up < 1)
                    _up = 1;
    
                this->numbertilde_03_smooth_index = _up;
                this->numbertilde_03_smooth_increment = (x - this->numbertilde_03_smooth_prev) / _up;
            } else if (x < this->numbertilde_03_smooth_prev) {
                number _down = down;
    
                if (_down < 1)
                    _down = 1;
    
                this->numbertilde_03_smooth_index = _down;
                this->numbertilde_03_smooth_increment = (x - this->numbertilde_03_smooth_prev) / _down;
            }
        }
    
        if (this->numbertilde_03_smooth_index > 0) {
            this->numbertilde_03_smooth_prev += this->numbertilde_03_smooth_increment;
            this->numbertilde_03_smooth_index -= 1;
        } else {
            this->numbertilde_03_smooth_prev = x;
        }
    
        return this->numbertilde_03_smooth_prev;
    }
    
    void numbertilde_03_smooth_reset() {
        this->numbertilde_03_smooth_prev = 0;
        this->numbertilde_03_smooth_index = 0;
        this->numbertilde_03_smooth_increment = 0;
        this->numbertilde_03_smooth_d_reset();
    }
    
    void numbertilde_03_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("HiCut~/number~_obj-25"), 1, this->_currentTime);
    }
    
    void numbertilde_03_dspsetup(bool force) {
        if ((bool)(this->numbertilde_03_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->numbertilde_03_currentIntervalInSamples = this->mstosamps(100);
        this->numbertilde_03_currentInterval = this->numbertilde_03_currentIntervalInSamples;
        this->numbertilde_03_rampInSamples = this->mstosamps(this->numbertilde_03_ramp);
        this->numbertilde_03_setupDone = true;
        this->numbertilde_03_smooth_d_dspsetup();
    }
    
    number numbertilde_04_smooth_d_next(number x) {
        number temp = (number)(x - this->numbertilde_04_smooth_d_prev);
        this->numbertilde_04_smooth_d_prev = x;
        return temp;
    }
    
    void numbertilde_04_smooth_d_dspsetup() {
        this->numbertilde_04_smooth_d_reset();
    }
    
    void numbertilde_04_smooth_d_reset() {
        this->numbertilde_04_smooth_d_prev = 0;
    }
    
    number numbertilde_04_smooth_next(number x, number up, number down) {
        if (this->numbertilde_04_smooth_d_next(x) != 0.) {
            if (x > this->numbertilde_04_smooth_prev) {
                number _up = up;
    
                if (_up < 1)
                    _up = 1;
    
                this->numbertilde_04_smooth_index = _up;
                this->numbertilde_04_smooth_increment = (x - this->numbertilde_04_smooth_prev) / _up;
            } else if (x < this->numbertilde_04_smooth_prev) {
                number _down = down;
    
                if (_down < 1)
                    _down = 1;
    
                this->numbertilde_04_smooth_index = _down;
                this->numbertilde_04_smooth_increment = (x - this->numbertilde_04_smooth_prev) / _down;
            }
        }
    
        if (this->numbertilde_04_smooth_index > 0) {
            this->numbertilde_04_smooth_prev += this->numbertilde_04_smooth_increment;
            this->numbertilde_04_smooth_index -= 1;
        } else {
            this->numbertilde_04_smooth_prev = x;
        }
    
        return this->numbertilde_04_smooth_prev;
    }
    
    void numbertilde_04_smooth_reset() {
        this->numbertilde_04_smooth_prev = 0;
        this->numbertilde_04_smooth_index = 0;
        this->numbertilde_04_smooth_increment = 0;
        this->numbertilde_04_smooth_d_reset();
    }
    
    void numbertilde_04_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("HiCut~/number~_obj-26"), 1, this->_currentTime);
    }
    
    void numbertilde_04_dspsetup(bool force) {
        if ((bool)(this->numbertilde_04_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->numbertilde_04_currentIntervalInSamples = this->mstosamps(100);
        this->numbertilde_04_currentInterval = this->numbertilde_04_currentIntervalInSamples;
        this->numbertilde_04_rampInSamples = this->mstosamps(this->numbertilde_04_ramp);
        this->numbertilde_04_setupDone = true;
        this->numbertilde_04_smooth_d_dspsetup();
    }
    
    number numbertilde_05_smooth_d_next(number x) {
        number temp = (number)(x - this->numbertilde_05_smooth_d_prev);
        this->numbertilde_05_smooth_d_prev = x;
        return temp;
    }
    
    void numbertilde_05_smooth_d_dspsetup() {
        this->numbertilde_05_smooth_d_reset();
    }
    
    void numbertilde_05_smooth_d_reset() {
        this->numbertilde_05_smooth_d_prev = 0;
    }
    
    number numbertilde_05_smooth_next(number x, number up, number down) {
        if (this->numbertilde_05_smooth_d_next(x) != 0.) {
            if (x > this->numbertilde_05_smooth_prev) {
                number _up = up;
    
                if (_up < 1)
                    _up = 1;
    
                this->numbertilde_05_smooth_index = _up;
                this->numbertilde_05_smooth_increment = (x - this->numbertilde_05_smooth_prev) / _up;
            } else if (x < this->numbertilde_05_smooth_prev) {
                number _down = down;
    
                if (_down < 1)
                    _down = 1;
    
                this->numbertilde_05_smooth_index = _down;
                this->numbertilde_05_smooth_increment = (x - this->numbertilde_05_smooth_prev) / _down;
            }
        }
    
        if (this->numbertilde_05_smooth_index > 0) {
            this->numbertilde_05_smooth_prev += this->numbertilde_05_smooth_increment;
            this->numbertilde_05_smooth_index -= 1;
        } else {
            this->numbertilde_05_smooth_prev = x;
        }
    
        return this->numbertilde_05_smooth_prev;
    }
    
    void numbertilde_05_smooth_reset() {
        this->numbertilde_05_smooth_prev = 0;
        this->numbertilde_05_smooth_index = 0;
        this->numbertilde_05_smooth_increment = 0;
        this->numbertilde_05_smooth_d_reset();
    }
    
    void numbertilde_05_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("HiCut~/number~_obj-1"), 1, this->_currentTime);
    }
    
    void numbertilde_05_dspsetup(bool force) {
        if ((bool)(this->numbertilde_05_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->numbertilde_05_currentIntervalInSamples = this->mstosamps(100);
        this->numbertilde_05_currentInterval = this->numbertilde_05_currentIntervalInSamples;
        this->numbertilde_05_rampInSamples = this->mstosamps(this->numbertilde_05_ramp);
        this->numbertilde_05_setupDone = true;
        this->numbertilde_05_smooth_d_dspsetup();
    }
    
    bool stackprotect_check() {
        this->stackprotect_count++;
    
        if (this->stackprotect_count > 128) {
            console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
            return true;
        }
    
        return false;
    }
    
    void updateTime(MillisecondTime time) {
        this->_currentTime = time;
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));
    
        if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
            this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;
    
        if (this->sampleOffsetIntoNextAudioBuffer < 0)
            this->sampleOffsetIntoNextAudioBuffer = 0;
    }
    
    void assign_defaults()
    {
        onepole_tilde_03_x = 0;
        onepole_tilde_03_freqInHz = 1;
        onepole_tilde_04_x = 0;
        onepole_tilde_04_freqInHz = 1;
        scale_tilde_03_x = 0;
        scale_tilde_03_lowin = 0;
        scale_tilde_03_hiin = 1;
        scale_tilde_03_lowout = 1000;
        scale_tilde_03_highout = 20000;
        scale_tilde_03_pow = 1;
        numbertilde_03_input_number = 0;
        numbertilde_03_ramp = 0;
        scale_tilde_04_x = 0;
        scale_tilde_04_lowin = 0;
        scale_tilde_04_hiin = 1;
        scale_tilde_04_lowout = 1000;
        scale_tilde_04_highout = 20000;
        scale_tilde_04_pow = 1;
        numbertilde_04_input_number = 0;
        numbertilde_04_ramp = 0;
        numbertilde_05_input_number = 0;
        numbertilde_05_ramp = 0;
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        onepole_tilde_03_freq = 0;
        onepole_tilde_03_needsUpdate = false;
        onepole_tilde_03_lastY = 0;
        onepole_tilde_03_a0 = 0;
        onepole_tilde_03_b1 = 0;
        onepole_tilde_03_setupDone = false;
        onepole_tilde_04_freq = 0;
        onepole_tilde_04_needsUpdate = false;
        onepole_tilde_04_lastY = 0;
        onepole_tilde_04_a0 = 0;
        onepole_tilde_04_b1 = 0;
        onepole_tilde_04_setupDone = false;
        numbertilde_03_currentInterval = 0;
        numbertilde_03_currentIntervalInSamples = 0;
        numbertilde_03_lastValue = 0;
        numbertilde_03_outValue = 0;
        numbertilde_03_rampInSamples = 0;
        numbertilde_03_currentMode = 1;
        numbertilde_03_smooth_d_prev = 0;
        numbertilde_03_smooth_prev = 0;
        numbertilde_03_smooth_index = 0;
        numbertilde_03_smooth_increment = 0;
        numbertilde_03_setupDone = false;
        numbertilde_04_currentInterval = 0;
        numbertilde_04_currentIntervalInSamples = 0;
        numbertilde_04_lastValue = 0;
        numbertilde_04_outValue = 0;
        numbertilde_04_rampInSamples = 0;
        numbertilde_04_currentMode = 1;
        numbertilde_04_smooth_d_prev = 0;
        numbertilde_04_smooth_prev = 0;
        numbertilde_04_smooth_index = 0;
        numbertilde_04_smooth_increment = 0;
        numbertilde_04_setupDone = false;
        numbertilde_05_currentInterval = 0;
        numbertilde_05_currentIntervalInSamples = 0;
        numbertilde_05_lastValue = 0;
        numbertilde_05_outValue = 0;
        numbertilde_05_rampInSamples = 0;
        numbertilde_05_currentMode = 1;
        numbertilde_05_smooth_d_prev = 0;
        numbertilde_05_smooth_prev = 0;
        numbertilde_05_smooth_index = 0;
        numbertilde_05_smooth_increment = 0;
        numbertilde_05_setupDone = false;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number onepole_tilde_03_x;
        number onepole_tilde_03_freqInHz;
        number onepole_tilde_04_x;
        number onepole_tilde_04_freqInHz;
        number scale_tilde_03_x;
        number scale_tilde_03_lowin;
        number scale_tilde_03_hiin;
        number scale_tilde_03_lowout;
        number scale_tilde_03_highout;
        number scale_tilde_03_pow;
        number numbertilde_03_input_number;
        number numbertilde_03_ramp;
        number scale_tilde_04_x;
        number scale_tilde_04_lowin;
        number scale_tilde_04_hiin;
        number scale_tilde_04_lowout;
        number scale_tilde_04_highout;
        number scale_tilde_04_pow;
        number numbertilde_04_input_number;
        number numbertilde_04_ramp;
        number numbertilde_05_input_number;
        number numbertilde_05_ramp;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[1];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        number onepole_tilde_03_freq;
        bool onepole_tilde_03_needsUpdate;
        number onepole_tilde_03_lastY;
        number onepole_tilde_03_a0;
        number onepole_tilde_03_b1;
        bool onepole_tilde_03_setupDone;
        number onepole_tilde_04_freq;
        bool onepole_tilde_04_needsUpdate;
        number onepole_tilde_04_lastY;
        number onepole_tilde_04_a0;
        number onepole_tilde_04_b1;
        bool onepole_tilde_04_setupDone;
        SampleIndex numbertilde_03_currentInterval;
        SampleIndex numbertilde_03_currentIntervalInSamples;
        number numbertilde_03_lastValue;
        number numbertilde_03_outValue;
        number numbertilde_03_rampInSamples;
        Int numbertilde_03_currentMode;
        number numbertilde_03_smooth_d_prev;
        number numbertilde_03_smooth_prev;
        number numbertilde_03_smooth_index;
        number numbertilde_03_smooth_increment;
        bool numbertilde_03_setupDone;
        SampleIndex numbertilde_04_currentInterval;
        SampleIndex numbertilde_04_currentIntervalInSamples;
        number numbertilde_04_lastValue;
        number numbertilde_04_outValue;
        number numbertilde_04_rampInSamples;
        Int numbertilde_04_currentMode;
        number numbertilde_04_smooth_d_prev;
        number numbertilde_04_smooth_prev;
        number numbertilde_04_smooth_index;
        number numbertilde_04_smooth_increment;
        bool numbertilde_04_setupDone;
        SampleIndex numbertilde_05_currentInterval;
        SampleIndex numbertilde_05_currentIntervalInSamples;
        number numbertilde_05_lastValue;
        number numbertilde_05_outValue;
        number numbertilde_05_rampInSamples;
        Int numbertilde_05_currentMode;
        number numbertilde_05_smooth_d_prev;
        number numbertilde_05_smooth_prev;
        number numbertilde_05_smooth_index;
        number numbertilde_05_smooth_increment;
        bool numbertilde_05_setupDone;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_15 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_15()
    {
    }
    
    ~RNBOSubpatcher_15()
    {
    }
    
    virtual rnbomatic* getPatcher() const {
        return static_cast<rnbomatic *>(_parentPatcher);
    }
    
    rnbomatic* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
        getEngine()->flushClockEvents(this, 892732297, false);
        getEngine()->flushClockEvents(this, 760652352, false);
    }
    
    SampleIndex currentsampletime() {
        return this->audioProcessSampleCount + this->sampleOffsetIntoNextAudioBuffer;
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    inline number safediv(number num, number denom) {
        return (denom == 0.0 ? 0.0 : num / denom);
    }
    
    number safepow(number base, number exponent) {
        return fixnan(rnbo_pow(base, exponent));
    }
    
    number scale(
        number x,
        number lowin,
        number hiin,
        number lowout,
        number highout,
        number pow
    ) {
        number inscale = this->safediv(1., hiin - lowin);
        number outdiff = highout - lowout;
        number value = (x - lowin) * inscale;
    
        if (pow != 1) {
            if (value > 0)
                value = this->safepow(value, pow);
            else
                value = -this->safepow(-value, pow);
        }
    
        value = value * outdiff + lowout;
        return value;
    }
    
    number samplerate() {
        return this->sr;
    }
    
    ParameterIndex getParameterIndexForID(ConstCharPointer paramid) const {
        if (!stringCompare(paramid, "number_obj-15/value")) {
            return 0;
        }
    
        if (!stringCompare(paramid, "toggle_obj-38/value")) {
            return 1;
        }
    
        if (!stringCompare(paramid, "number_obj-47/value")) {
            return 2;
        }
    
        if (!stringCompare(paramid, "number_obj-41/value")) {
            return 3;
        }
    
        if (!stringCompare(paramid, "number_obj-18/value")) {
            return 4;
        }
    
        return INVALID_INDEX;
    }
    
    Index getNumMidiInputPorts() const {
        return 0;
    }
    
    void processMidiEvent(MillisecondTime , int , ConstByteArray , Index ) {}
    
    Index getNumMidiOutputPorts() const {
        return 0;
    }
    
    void process(
        SampleValue ** inputs,
        Index numInputs,
        SampleValue ** outputs,
        Index numOutputs,
        Index n
    ) {
        this->vs = n;
        this->updateTime(this->getEngine()->getCurrentTime());
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        SampleValue * in3 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        this->line_01_perform(this->signals[0], n);
        this->phasor_01_perform(this->signals[0], this->signals[1], n);
        this->selector_01_perform(this->selector_01_onoff, this->signals[1], in3, out1, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 2; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
            this->phasor_01_sigbuf = resizeSignal(this->phasor_01_sigbuf, this->maxvs, maxBlockSize);
            this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
            this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
            this->didAllocateSignals = true;
        }
    
        const bool sampleRateChanged = sampleRate != this->sr;
        const bool maxvsChanged = maxBlockSize != this->maxvs;
        const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;
    
        if (sampleRateChanged || maxvsChanged) {
            this->vs = maxBlockSize;
            this->maxvs = maxBlockSize;
            this->sr = sampleRate;
            this->invsr = 1 / sampleRate;
        }
    
        this->phasor_01_dspsetup(forceDSPSetup);
    
        if (sampleRateChanged)
            this->onSampleRateChanged(sampleRate);
    }
    
    void setProbingTarget(MessageTag id) {
        switch (id) {
        default:
            this->setProbingIndex(-1);
            break;
        }
    }
    
    void setProbingIndex(ProbingIndex ) {}
    
    Index getProbingChannels(MessageTag outletId) const {
        RNBO_UNUSED(outletId);
        return 0;
    }
    
    Index getIsMuted()  {
        return this->isMuted;
    }
    
    void setIsMuted(Index v)  {
        this->isMuted = v;
    }
    
    Index getPatcherSerial() const {
        return 0;
    }
    
    void getState(PatcherStateInterface& ) {}
    
    void setState() {}
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex index, ParameterValue v, MillisecondTime time) {
        this->updateTime(time);
    
        switch (index) {
        case 0:
            this->numberobj_01_value_set(v);
            break;
        case 1:
            this->toggle_01_value_set(v);
            break;
        case 2:
            this->numberobj_02_value_set(v);
            break;
        case 3:
            this->numberobj_03_value_set(v);
            break;
        case 4:
            this->numberobj_04_value_set(v);
            break;
        }
    }
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
        case 0:
            return this->numberobj_01_value;
        case 1:
            return this->toggle_01_value;
        case 2:
            return this->numberobj_02_value;
        case 3:
            return this->numberobj_03_value;
        case 4:
            return this->numberobj_04_value;
        default:
            return 0;
        }
    }
    
    ParameterIndex getNumSignalInParameters() const {
        return 0;
    }
    
    ParameterIndex getNumSignalOutParameters() const {
        return 0;
    }
    
    ParameterIndex getNumParameters() const {
        return 5;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "numberobj_01_value";
        case 1:
            return "toggle_01_value";
        case 2:
            return "numberobj_02_value";
        case 3:
            return "numberobj_03_value";
        case 4:
            return "numberobj_04_value";
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "LFO~/number_obj-15/value";
        case 1:
            return "LFO~/toggle_obj-38/value";
        case 2:
            return "LFO~/number_obj-47/value";
        case 3:
            return "LFO~/number_obj-41/value";
        case 4:
            return "LFO~/number_obj-18/value";
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
        {
            switch (index) {
            case 0:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 1:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 2:
                info->type = ParameterTypeNumber;
                info->initialValue = 2.09375;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 3:
                info->type = ParameterTypeNumber;
                info->initialValue = 50;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 4:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            }
        }
    }
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
    }
    
    ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
        if (steps == 1) {
            if (normalizedValue > 0) {
                normalizedValue = 1.;
            }
        } else {
            ParameterValue oneStep = (number)1. / (steps - 1);
            ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
            normalizedValue = numberOfSteps * oneStep;
        }
    
        return normalizedValue;
    }
    
    ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
                ParameterValue normalizedValue = (value - 0) / (1 - 0);
                return normalizedValue;
            }
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return 0 + value * (1 - 0);
                }
            }
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        default:
            return value;
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
    }
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(value);
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case 892732297:
            this->loadmess_01_startupbang_bang();
            break;
        case 760652352:
            this->line_01_target_bang();
            break;
        }
    }
    
    void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}
    
    void processOutletEvent(
        EngineLink* sender,
        OutletIndex index,
        ParameterValue value,
        MillisecondTime time
    ) {
        this->updateTime(time);
        this->processOutletAtCurrentTime(sender, index, value);
    }
    
    void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("format"):
            if (TAG("LFO~/number_obj-15") == objectId)
                this->numberobj_01_format_set(payload);
    
            if (TAG("LFO~/number_obj-47") == objectId)
                this->numberobj_02_format_set(payload);
    
            if (TAG("LFO~/number_obj-41") == objectId)
                this->numberobj_03_format_set(payload);
    
            if (TAG("LFO~/number_obj-18") == objectId)
                this->numberobj_04_format_set(payload);
    
            break;
        case TAG("lfo-mode"):
            this->inport_01_value_number_set(payload);
            break;
        }
    }
    
    void processListMessage(
        MessageTag tag,
        MessageTag objectId,
        MillisecondTime time,
        const list& payload
    ) {
        RNBO_UNUSED(objectId);
        this->updateTime(time);
    
        switch (tag) {
        case TAG("lfo-mode"):
            this->inport_01_value_list_set(payload);
            break;
        }
    }
    
    void processBangMessage(MessageTag tag, MessageTag objectId, MillisecondTime time) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("lfo-mode"):
            this->inport_01_value_bang_bang();
            break;
        case TAG("startupbang"):
            if (TAG("LFO~/loadmess_obj-42") == objectId)
                this->loadmess_01_startupbang_bang();
    
            break;
        }
    }
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("setup"):
            return "setup";
        case TAG("LFO~/number_obj-15"):
            return "LFO~/number_obj-15";
        case TAG("LFO~/number_obj-47"):
            return "LFO~/number_obj-47";
        case TAG("LFO~/number_obj-41"):
            return "LFO~/number_obj-41";
        case TAG("LFO~/number_obj-18"):
            return "LFO~/number_obj-18";
        case TAG("format"):
            return "format";
        case TAG("lfo-mode"):
            return "lfo-mode";
        case TAG(""):
            return "";
        case TAG("startupbang"):
            return "startupbang";
        case TAG("LFO~/loadmess_obj-42"):
            return "LFO~/loadmess_obj-42";
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        default:
            return nullptr;
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 0;
    }
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void processDataViewUpdate(DataRefIndex , MillisecondTime ) {}
    
    void initialize() {
        this->assign_defaults();
        this->setState();
    }
    
    protected:
    
    void numberobj_01_value_set(number v) {
        this->numberobj_01_value_setter(v);
        v = this->numberobj_01_value;
        this->sendParameter(0, false);
        this->numberobj_01_output_set(v);
    }
    
    void numberobj_01_format_set(number v) {
        if (v == 0) {
            this->numberobj_01_currentFormat = 0;
        } else if (v == 1) {
            this->numberobj_01_currentFormat = 1;
        } else if (v == 2) {
            this->numberobj_01_currentFormat = 2;
        } else if (v == 3) {
            this->numberobj_01_currentFormat = 3;
        } else if (v == 4) {
            this->numberobj_01_currentFormat = 4;
        } else if (v == 5) {
            this->numberobj_01_currentFormat = 5;
        } else if (v == 6) {
            this->numberobj_01_currentFormat = 6;
        }
    }
    
    void toggle_01_value_set(number v) {
        this->toggle_01_value = v;
        this->sendParameter(1, false);
        this->expr_01_in1_set(v);
    }
    
    void inport_01_value_bang_bang() {
        this->inport_01_out_bang_bang();
    }
    
    void inport_01_value_number_set(number v) {
        this->inport_01_out_number_set(v);
    }
    
    void inport_01_value_list_set(const list& v) {
        this->inport_01_out_list_set(v);
    }
    
    void loadmess_01_startupbang_bang() {
        this->loadmess_01_message_bang();
    }
    
    void numberobj_02_format_set(number v) {
        if (v == 0) {
            this->numberobj_02_currentFormat = 0;
        } else if (v == 1) {
            this->numberobj_02_currentFormat = 1;
        } else if (v == 2) {
            this->numberobj_02_currentFormat = 2;
        } else if (v == 3) {
            this->numberobj_02_currentFormat = 3;
        } else if (v == 4) {
            this->numberobj_02_currentFormat = 4;
        } else if (v == 5) {
            this->numberobj_02_currentFormat = 5;
        } else if (v == 6) {
            this->numberobj_02_currentFormat = 6;
        }
    }
    
    void numberobj_03_format_set(number v) {
        if (v == 0) {
            this->numberobj_03_currentFormat = 0;
        } else if (v == 1) {
            this->numberobj_03_currentFormat = 1;
        } else if (v == 2) {
            this->numberobj_03_currentFormat = 2;
        } else if (v == 3) {
            this->numberobj_03_currentFormat = 3;
        } else if (v == 4) {
            this->numberobj_03_currentFormat = 4;
        } else if (v == 5) {
            this->numberobj_03_currentFormat = 5;
        } else if (v == 6) {
            this->numberobj_03_currentFormat = 6;
        }
    }
    
    void eventinlet_01_out1_bang_bang() {
        this->numberobj_03_value_bang();
    }
    
    void eventinlet_01_out1_list_set(const list& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_03_value_set(converted);
        }
    }
    
    void numberobj_04_value_set(number v) {
        this->numberobj_04_value_setter(v);
        v = this->numberobj_04_value;
        this->sendParameter(4, false);
        this->numberobj_04_output_set(v);
    }
    
    void numberobj_04_format_set(number v) {
        if (v == 0) {
            this->numberobj_04_currentFormat = 0;
        } else if (v == 1) {
            this->numberobj_04_currentFormat = 1;
        } else if (v == 2) {
            this->numberobj_04_currentFormat = 2;
        } else if (v == 3) {
            this->numberobj_04_currentFormat = 3;
        } else if (v == 4) {
            this->numberobj_04_currentFormat = 4;
        } else if (v == 5) {
            this->numberobj_04_currentFormat = 5;
        } else if (v == 6) {
            this->numberobj_04_currentFormat = 6;
        }
    }
    
    void eventinlet_02_out1_bang_bang() {
        this->numberobj_04_value_bang();
    }
    
    void eventinlet_02_out1_number_set(number v) {
        this->numberobj_04_value_set(v);
    }
    
    void eventinlet_02_out1_list_set(const list& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_04_value_set(converted);
        }
    }
    
    void line_01_target_bang() {}
    
    number msToSamps(MillisecondTime ms, number sampleRate) {
        return ms * sampleRate * 0.001;
    }
    
    MillisecondTime sampsToMs(SampleIndex samps) {
        return samps * (this->invsr * 1000);
    }
    
    Index getMaxBlockSize() const {
        return this->maxvs;
    }
    
    number getSampleRate() const {
        return this->sr;
    }
    
    bool hasFixedVectorSize() const {
        return false;
    }
    
    Index getNumInputChannels() const {
        return 1;
    }
    
    Index getNumOutputChannels() const {
        return 1;
    }
    
    void initializeObjects() {
        this->numberobj_01_init();
        this->numberobj_02_init();
        this->numberobj_03_init();
        this->numberobj_04_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {
        this->updateTime(this->getEngine()->getCurrentTime());
        this->getEngine()->scheduleClockEvent(this, 892732297, 0 + this->_currentTime);;
    }
    
    void allocateDataRefs() {}
    
    void line_01_segments_set(const list& v) {
        this->line_01_segments = jsCreateListCopy(v);
    
        if ((bool)(v->length)) {
            SampleIndex currentTime = this->currentsampletime();
            number lastRampValue = this->line_01_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;
    
            for (Index i = 0; i < this->line_01_activeRamps->length; i += 3) {
                rampEnd = this->line_01_activeRamps[(Index)(i + 2)];
    
                if (rampEnd > currentTime) {
                    this->line_01_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->line_01_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->line_01_activeRamps[(Index)i] - valueDiff;
                    this->line_01_activeRamps[(Index)i] = lastRampValue;
                    this->line_01_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->line_01_activeRamps[(Index)i];
                }
            }
    
            if (rampEnd < currentTime) {
                this->line_01_activeRamps->push(lastRampValue);
                this->line_01_activeRamps->push(0);
                this->line_01_activeRamps->push(currentTime);
            }
    
            number lastRampEnd = currentTime;
    
            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;
    
                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);
                } else {
                    rampTimeInSamples = this->mstosamps(this->line_01_time);
                }
    
                if (rampTimeInSamples <= 0)
                    rampTimeInSamples = 1;
    
                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->line_01_activeRamps->push(destinationValue);
                this->line_01_activeRamps->push(inc);
                this->line_01_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
    
    void numberobj_02_output_set(number v) {
        {
            list converted = {v};
            this->line_01_segments_set(converted);
        }
    }
    
    void numberobj_02_value_set(number v) {
        this->numberobj_02_value_setter(v);
        v = this->numberobj_02_value;
        this->sendParameter(2, false);
        this->numberobj_02_output_set(v);
    }
    
    void scale_01_out_set(const list& v) {
        this->scale_01_out = jsCreateListCopy(v);
    
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_02_value_set(converted);
        }
    }
    
    void scale_01_input_set(const list& v) {
        this->scale_01_input = jsCreateListCopy(v);
        list tmp = {};
    
        for (Index i = 0; i < v->length; i++) {
            tmp->push(this->scale(
                v[(Index)i],
                this->scale_01_inlow,
                this->scale_01_inhigh,
                this->scale_01_outlow,
                this->scale_01_outhigh,
                this->scale_01_power
            ));
        }
    
        this->scale_01_out_set(tmp);
    }
    
    void numberobj_03_output_set(number v) {
        {
            list converted = {v};
            this->scale_01_input_set(converted);
        }
    }
    
    void numberobj_03_value_set(number v) {
        this->numberobj_03_value_setter(v);
        v = this->numberobj_03_value;
        this->sendParameter(3, false);
        this->numberobj_03_output_set(v);
    }
    
    void eventinlet_01_out1_number_set(number v) {
        this->numberobj_03_value_set(v);
    }
    
    void selector_01_onoff_set(number v) {
        this->selector_01_onoff = v;
    }
    
    void numberobj_01_output_set(number v) {
        this->selector_01_onoff_set(v);
    }
    
    void expr_01_out1_set(number v) {
        this->expr_01_out1 = v;
        this->numberobj_01_value_set(this->expr_01_out1);
    }
    
    void expr_01_in1_set(number in1) {
        this->expr_01_in1 = in1;
        this->expr_01_out1_set(this->expr_01_in1 + this->expr_01_in2);//#map:LFO~/+_obj-40:1
    }
    
    void toggle_01_value_bang() {
        if (this->stackprotect_check())
            return;
    
        this->toggle_01_value_set((this->toggle_01_value == 1 ? 0 : 1));
    }
    
    void inport_01_out_bang_bang() {
        this->toggle_01_value_bang();
    }
    
    void inport_01_out_number_set(number v) {
        this->toggle_01_value_set(v);
    }
    
    void inport_01_out_list_set(const list& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->toggle_01_value_set(converted);
        }
    }
    
    void loadmess_01_message_bang() {
        list v = this->loadmess_01_message;
    
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->toggle_01_value_set(converted);
        }
    }
    
    void numberobj_03_value_bang() {
        number v = this->numberobj_03_value;
        this->sendParameter(3, false);
        this->numberobj_03_output_set(v);
    }
    
    void phasor_01_phase_set(number v) {
        if (v >= 0 && (bool)(this->phasor_01_sigbuf)) {
            this->phasor_01_sigbuf[(Index)this->sampleOffsetIntoNextAudioBuffer] = v;
        }
    }
    
    void numberobj_04_output_set(number v) {
        this->phasor_01_phase_set(v);
    }
    
    void numberobj_04_value_bang() {
        number v = this->numberobj_04_value;
        this->sendParameter(4, false);
        this->numberobj_04_output_set(v);
    }
    
    void line_01_perform(Sample * out, Index n) {
        auto __line_01_currentValue = this->line_01_currentValue;
        Index i = 0;
    
        if ((bool)(this->line_01_activeRamps->length)) {
            while ((bool)(this->line_01_activeRamps->length) && i < n) {
                number destinationValue = this->line_01_activeRamps[0];
                number inc = this->line_01_activeRamps[1];
                number rampTimeInSamples = this->line_01_activeRamps[2] - this->audioProcessSampleCount - i;
                number val = __line_01_currentValue;
    
                while (rampTimeInSamples > 0 && i < n) {
                    out[(Index)i] = val;
                    val += inc;
                    i++;
                    rampTimeInSamples--;
                }
    
                if (rampTimeInSamples <= 0) {
                    val = destinationValue;
                    this->line_01_activeRamps->splice(0, 3);
    
                    if ((bool)(!(bool)(this->line_01_activeRamps->length))) this->getEngine()->scheduleClockEventWithValue(
                        this,
                        760652352,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;
                }
    
                __line_01_currentValue = val;
            }
        }
    
        while (i < n) {
            out[(Index)i] = __line_01_currentValue;
            i++;
        }
    
        this->line_01_currentValue = __line_01_currentValue;
    }
    
    void phasor_01_perform(const Sample * freq, Sample * out, Index n) {
        auto __phasor_01_sigbuf = this->phasor_01_sigbuf;
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = this->phasor_01_ph_next(freq[(Index)i], __phasor_01_sigbuf[(Index)i]);
            __phasor_01_sigbuf[(Index)i] = -1;
        }
    }
    
    void selector_01_perform(
        number onoff,
        const Sample * in1,
        const Sample * in2,
        Sample * out,
        Index n
    ) {
        Index i;
    
        for (i = 0; i < n; i++) {
            if (onoff >= 1 && onoff < 2)
                out[(Index)i] = in1[(Index)i];
            else if (onoff >= 2 && onoff < 3)
                out[(Index)i] = in2[(Index)i];
            else
                out[(Index)i] = 0;
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void numberobj_01_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_01_currentFormat != 6) {
            localvalue = rnbo_trunc(localvalue);
        }
    
        this->numberobj_01_value = localvalue;
    }
    
    void numberobj_02_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_02_currentFormat != 6) {
            localvalue = rnbo_trunc(localvalue);
        }
    
        this->numberobj_02_value = localvalue;
    }
    
    void numberobj_03_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_03_currentFormat != 6) {
            localvalue = rnbo_trunc(localvalue);
        }
    
        this->numberobj_03_value = localvalue;
    }
    
    void numberobj_04_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_04_currentFormat != 6) {
            localvalue = rnbo_trunc(localvalue);
        }
    
        this->numberobj_04_value = localvalue;
    }
    
    void numberobj_01_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("LFO~/number_obj-15"), 1, this->_currentTime);
    }
    
    void numberobj_01_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_01_value;
    }
    
    void numberobj_01_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_01_value_set(preset["value"]);
    }
    
    void toggle_01_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->toggle_01_value;
    }
    
    void toggle_01_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->toggle_01_value_set(preset["value"]);
    }
    
    void numberobj_02_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("LFO~/number_obj-47"), 1, this->_currentTime);
    }
    
    void numberobj_02_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_02_value;
    }
    
    void numberobj_02_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_02_value_set(preset["value"]);
    }
    
    void numberobj_03_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("LFO~/number_obj-41"), 1, this->_currentTime);
    }
    
    void numberobj_03_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_03_value;
    }
    
    void numberobj_03_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_03_value_set(preset["value"]);
    }
    
    number phasor_01_ph_next(number freq, number reset) {
        {
            {
                if (reset >= 0.)
                    this->phasor_01_ph_currentPhase = reset;
            }
        }
    
        number pincr = freq * this->phasor_01_ph_conv;
    
        if (this->phasor_01_ph_currentPhase < 0.)
            this->phasor_01_ph_currentPhase = 1. + this->phasor_01_ph_currentPhase;
    
        if (this->phasor_01_ph_currentPhase > 1.)
            this->phasor_01_ph_currentPhase = this->phasor_01_ph_currentPhase - 1.;
    
        number tmp = this->phasor_01_ph_currentPhase;
        this->phasor_01_ph_currentPhase += pincr;
        return tmp;
    }
    
    void phasor_01_ph_reset() {
        this->phasor_01_ph_currentPhase = 0;
    }
    
    void phasor_01_ph_dspsetup() {
        this->phasor_01_ph_conv = (number)1 / this->sr;
    }
    
    void phasor_01_dspsetup(bool force) {
        if ((bool)(this->phasor_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->phasor_01_conv = (number)1 / this->samplerate();
        this->phasor_01_setupDone = true;
        this->phasor_01_ph_dspsetup();
    }
    
    void numberobj_04_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("LFO~/number_obj-18"), 1, this->_currentTime);
    }
    
    void numberobj_04_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_04_value;
    }
    
    void numberobj_04_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_04_value_set(preset["value"]);
    }
    
    bool stackprotect_check() {
        this->stackprotect_count++;
    
        if (this->stackprotect_count > 128) {
            console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
            return true;
        }
    
        return false;
    }
    
    void updateTime(MillisecondTime time) {
        this->_currentTime = time;
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));
    
        if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
            this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;
    
        if (this->sampleOffsetIntoNextAudioBuffer < 0)
            this->sampleOffsetIntoNextAudioBuffer = 0;
    }
    
    void assign_defaults()
    {
        numberobj_01_value = 1;
        numberobj_01_value_setter(numberobj_01_value);
        expr_01_in1 = 0;
        expr_01_in2 = 1;
        expr_01_out1 = 0;
        toggle_01_value = 0;
        selector_01_onoff = 1;
        loadmess_01_message = { 0 };
        line_01_time = 10;
        numberobj_02_value = 2.09375;
        numberobj_02_value_setter(numberobj_02_value);
        scale_01_inlow = 0;
        scale_01_inhigh = 100;
        scale_01_outlow = 0.125;
        scale_01_outhigh = 8;
        scale_01_power = 2;
        numberobj_03_value = 50;
        numberobj_03_value_setter(numberobj_03_value);
        phasor_01_freq = 0;
        numberobj_04_value = 0;
        numberobj_04_value_setter(numberobj_04_value);
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        signals[1] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        numberobj_01_currentFormat = 6;
        numberobj_01_lastValue = 0;
        line_01_currentValue = 0;
        numberobj_02_currentFormat = 6;
        numberobj_02_lastValue = 0;
        numberobj_03_currentFormat = 6;
        numberobj_03_lastValue = 0;
        phasor_01_sigbuf = nullptr;
        phasor_01_lastLockedPhase = 0;
        phasor_01_conv = 0;
        phasor_01_ph_currentPhase = 0;
        phasor_01_ph_conv = 0;
        phasor_01_setupDone = false;
        numberobj_04_currentFormat = 6;
        numberobj_04_lastValue = 0;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number numberobj_01_value;
        number expr_01_in1;
        number expr_01_in2;
        number expr_01_out1;
        number toggle_01_value;
        number selector_01_onoff;
        list loadmess_01_message;
        list line_01_segments;
        number line_01_time;
        number numberobj_02_value;
        list scale_01_input;
        number scale_01_inlow;
        number scale_01_inhigh;
        number scale_01_outlow;
        number scale_01_outhigh;
        number scale_01_power;
        list scale_01_out;
        number numberobj_03_value;
        number phasor_01_freq;
        number numberobj_04_value;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[2];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        Int numberobj_01_currentFormat;
        number numberobj_01_lastValue;
        list line_01_activeRamps;
        number line_01_currentValue;
        Int numberobj_02_currentFormat;
        number numberobj_02_lastValue;
        Int numberobj_03_currentFormat;
        number numberobj_03_lastValue;
        signal phasor_01_sigbuf;
        number phasor_01_lastLockedPhase;
        number phasor_01_conv;
        number phasor_01_ph_currentPhase;
        number phasor_01_ph_conv;
        bool phasor_01_setupDone;
        Int numberobj_04_currentFormat;
        number numberobj_04_lastValue;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_16 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_16()
    {
    }
    
    ~RNBOSubpatcher_16()
    {
    }
    
    virtual rnbomatic* getPatcher() const {
        return static_cast<rnbomatic *>(_parentPatcher);
    }
    
    rnbomatic* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
    }
    
    template <typename T> void listswapelements(T& arr, Int a, Int b) {
        auto tmp = arr[(Index)a];
        arr[(Index)a] = arr[(Index)b];
        arr[(Index)b] = tmp;
    }
    
    template <typename T> Int listpartition(T& arr, T& sortindices, Int l, Int h, bool ascending) {
        number x = arr[(Index)h];
        Int i = (Int)(l - 1);
    
        for (Int j = (Int)(l); j <= h - 1; j++) {
            bool asc = (bool)((bool)(ascending) && arr[(Index)j] <= x);
            bool desc = (bool)((bool)(!(bool)(ascending)) && arr[(Index)j] >= x);
    
            if ((bool)(asc) || (bool)(desc)) {
                i++;
                this->listswapelements(arr, i, j);
                this->listswapelements(sortindices, i, j);
            }
        }
    
        i++;
        this->listswapelements(arr, i, h);
        this->listswapelements(sortindices, i, h);
        return i;
    }
    
    template <typename T> void listquicksort(T& arr, T& sortindices, Int l, Int h, bool ascending) {
        if (l < h) {
            Int p = (Int)(this->listpartition(arr, sortindices, l, h, ascending));
            this->listquicksort(arr, sortindices, l, p - 1, ascending);
            this->listquicksort(arr, sortindices, p + 1, h, ascending);
        }
    }
    
    array<list, 2> listsort(const list& input, number order) {
        array<list, 2> tmp = {};
        tmp[0] = input;
        tmp[1] = {};
    
        for (Index i = 0; i < tmp[0]->length; i++) {
            tmp[1]->push(i);
        }
    
        this->listquicksort(tmp[0], tmp[1], 0, (Int)(tmp[0]->length - 1), (order >= 0 ? 1 : 0));
        return tmp;
    }
    
    inline number safediv(number num, number denom) {
        return (denom == 0.0 ? 0.0 : num / denom);
    }
    
    number safepow(number base, number exponent) {
        return fixnan(rnbo_pow(base, exponent));
    }
    
    number scale(
        number x,
        number lowin,
        number hiin,
        number lowout,
        number highout,
        number pow
    ) {
        number inscale = this->safediv(1., hiin - lowin);
        number outdiff = highout - lowout;
        number value = (x - lowin) * inscale;
    
        if (pow != 1) {
            if (value > 0)
                value = this->safepow(value, pow);
            else
                value = -this->safepow(-value, pow);
        }
    
        value = value * outdiff + lowout;
        return value;
    }
    
    MillisecondTime currenttime() {
        return this->_currentTime;
    }
    
    ParameterIndex getParameterIndexForID(ConstCharPointer paramid) const {
        if (!stringCompare(paramid, "button_obj-35/bangval")) {
            return 0;
        }
    
        if (!stringCompare(paramid, "number_obj-75/value")) {
            return 1;
        }
    
        if (!stringCompare(paramid, "number_obj-64/value")) {
            return 2;
        }
    
        if (!stringCompare(paramid, "number_obj-19/value")) {
            return 3;
        }
    
        if (!stringCompare(paramid, "number_obj-17/value")) {
            return 4;
        }
    
        if (!stringCompare(paramid, "number_obj-6/value")) {
            return 5;
        }
    
        if (!stringCompare(paramid, "button_obj-3/bangval")) {
            return 6;
        }
    
        if (!stringCompare(paramid, "message_obj-7/bangval")) {
            return 7;
        }
    
        if (!stringCompare(paramid, "number_obj-70/value")) {
            return 8;
        }
    
        if (!stringCompare(paramid, "button_obj-52/bangval")) {
            return 9;
        }
    
        return INVALID_INDEX;
    }
    
    Index getNumMidiInputPorts() const {
        return 0;
    }
    
    void processMidiEvent(MillisecondTime , int , ConstByteArray , Index ) {}
    
    Index getNumMidiOutputPorts() const {
        return 0;
    }
    
    void process(
        SampleValue ** inputs,
        Index numInputs,
        SampleValue ** outputs,
        Index numOutputs,
        Index n
    ) {
        RNBO_UNUSED(numOutputs);
        RNBO_UNUSED(outputs);
        RNBO_UNUSED(numInputs);
        RNBO_UNUSED(inputs);
        this->vs = n;
        this->updateTime(this->getEngine()->getCurrentTime());
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
            this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
            this->didAllocateSignals = true;
        }
    
        const bool sampleRateChanged = sampleRate != this->sr;
        const bool maxvsChanged = maxBlockSize != this->maxvs;
        const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;
    
        if (sampleRateChanged || maxvsChanged) {
            this->vs = maxBlockSize;
            this->maxvs = maxBlockSize;
            this->sr = sampleRate;
            this->invsr = 1 / sampleRate;
        }
    
        RNBO_UNUSED(forceDSPSetup);
    
        if (sampleRateChanged)
            this->onSampleRateChanged(sampleRate);
    }
    
    void setProbingTarget(MessageTag id) {
        switch (id) {
        default:
            this->setProbingIndex(-1);
            break;
        }
    }
    
    void setProbingIndex(ProbingIndex ) {}
    
    Index getProbingChannels(MessageTag outletId) const {
        RNBO_UNUSED(outletId);
        return 0;
    }
    
    Index getIsMuted()  {
        return this->isMuted;
    }
    
    void setIsMuted(Index v)  {
        this->isMuted = v;
    }
    
    Index getPatcherSerial() const {
        return 0;
    }
    
    void getState(PatcherStateInterface& ) {}
    
    void setState() {}
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex index, ParameterValue v, MillisecondTime time) {
        this->updateTime(time);
    
        switch (index) {
        case 0:
            this->button_01_bangval_bang();
            break;
        case 1:
            this->numberobj_05_value_set(v);
            break;
        case 2:
            this->numberobj_06_value_set(v);
            break;
        case 3:
            this->numberobj_07_value_set(v);
            break;
        case 4:
            this->numberobj_08_value_set(v);
            break;
        case 5:
            this->numberobj_09_value_set(v);
            break;
        case 6:
            this->button_02_bangval_bang();
            break;
        case 7:
            this->message_01_bangval_bang();
            break;
        case 8:
            this->numberobj_10_value_set(v);
            break;
        case 9:
            this->button_03_bangval_bang();
            break;
        }
    }
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
        case 1:
            return this->numberobj_05_value;
        case 2:
            return this->numberobj_06_value;
        case 3:
            return this->numberobj_07_value;
        case 4:
            return this->numberobj_08_value;
        case 5:
            return this->numberobj_09_value;
        case 8:
            return this->numberobj_10_value;
        default:
            return 0;
        }
    }
    
    ParameterIndex getNumSignalInParameters() const {
        return 0;
    }
    
    ParameterIndex getNumSignalOutParameters() const {
        return 0;
    }
    
    ParameterIndex getNumParameters() const {
        return 10;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "button_01_bangval";
        case 1:
            return "numberobj_05_value";
        case 2:
            return "numberobj_06_value";
        case 3:
            return "numberobj_07_value";
        case 4:
            return "numberobj_08_value";
        case 5:
            return "numberobj_09_value";
        case 6:
            return "button_02_bangval";
        case 7:
            return "message_01_bangval";
        case 8:
            return "numberobj_10_value";
        case 9:
            return "button_03_bangval";
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "TapTempo/button_obj-35/bangval";
        case 1:
            return "TapTempo/number_obj-75/value";
        case 2:
            return "TapTempo/number_obj-64/value";
        case 3:
            return "TapTempo/number_obj-19/value";
        case 4:
            return "TapTempo/number_obj-17/value";
        case 5:
            return "TapTempo/number_obj-6/value";
        case 6:
            return "TapTempo/button_obj-3/bangval";
        case 7:
            return "TapTempo/message_obj-7/bangval";
        case 8:
            return "TapTempo/number_obj-70/value";
        case 9:
            return "TapTempo/button_obj-52/bangval";
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
        {
            switch (index) {
            case 0:
                info->type = ParameterTypeBang;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = false;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 1:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 2:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 3:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 4:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 5:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 6:
                info->type = ParameterTypeBang;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = false;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 7:
                info->type = ParameterTypeBang;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = false;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 8:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 9:
                info->type = ParameterTypeBang;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = false;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            }
        }
    }
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
    }
    
    ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
        if (steps == 1) {
            if (normalizedValue > 0) {
                normalizedValue = 1.;
            }
        } else {
            ParameterValue oneStep = (number)1. / (steps - 1);
            ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
            normalizedValue = numberOfSteps * oneStep;
        }
    
        return normalizedValue;
    }
    
    ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 8:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
                ParameterValue normalizedValue = (value - 0) / (1 - 0);
                return normalizedValue;
            }
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 8:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return 0 + value * (1 - 0);
                }
            }
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        default:
            return value;
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
    }
    
    void processClockEvent(MillisecondTime , ClockId , bool , ParameterValue ) {}
    
    void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}
    
    void processOutletEvent(
        EngineLink* sender,
        OutletIndex index,
        ParameterValue value,
        MillisecondTime time
    ) {
        this->updateTime(time);
        this->processOutletAtCurrentTime(sender, index, value);
    }
    
    void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("format"):
            if (TAG("TapTempo/number_obj-75") == objectId)
                this->numberobj_05_format_set(payload);
    
            if (TAG("TapTempo/number_obj-64") == objectId)
                this->numberobj_06_format_set(payload);
    
            if (TAG("TapTempo/number_obj-19") == objectId)
                this->numberobj_07_format_set(payload);
    
            if (TAG("TapTempo/number_obj-17") == objectId)
                this->numberobj_08_format_set(payload);
    
            if (TAG("TapTempo/number_obj-6") == objectId)
                this->numberobj_09_format_set(payload);
    
            if (TAG("TapTempo/number_obj-70") == objectId)
                this->numberobj_10_format_set(payload);
    
            break;
        case TAG("tap-tempo"):
            this->inport_02_value_number_set(payload);
            break;
        case TAG("tap-mult"):
            this->inport_03_value_number_set(payload);
            break;
        }
    }
    
    void processListMessage(
        MessageTag tag,
        MessageTag objectId,
        MillisecondTime time,
        const list& payload
    ) {
        RNBO_UNUSED(objectId);
        this->updateTime(time);
    
        switch (tag) {
        case TAG("tap-tempo"):
            this->inport_02_value_list_set(payload);
            break;
        case TAG("tap-mult"):
            this->inport_03_value_list_set(payload);
            break;
        }
    }
    
    void processBangMessage(MessageTag tag, MessageTag objectId, MillisecondTime time) {
        RNBO_UNUSED(objectId);
        this->updateTime(time);
    
        switch (tag) {
        case TAG("tap-tempo"):
            this->inport_02_value_bang_bang();
            break;
        case TAG("tap-mult"):
            this->inport_03_value_bang_bang();
            break;
        }
    }
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("setup"):
            return "setup";
        case TAG("TapTempo/number_obj-75"):
            return "TapTempo/number_obj-75";
        case TAG("TapTempo/number_obj-64"):
            return "TapTempo/number_obj-64";
        case TAG("TapTempo/number_obj-19"):
            return "TapTempo/number_obj-19";
        case TAG("TapTempo/number_obj-17"):
            return "TapTempo/number_obj-17";
        case TAG("TapTempo/number_obj-6"):
            return "TapTempo/number_obj-6";
        case TAG("TapTempo/number_obj-70"):
            return "TapTempo/number_obj-70";
        case TAG("format"):
            return "format";
        case TAG("tap-tempo"):
            return "tap-tempo";
        case TAG(""):
            return "";
        case TAG("tap-mult"):
            return "tap-mult";
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        default:
            return nullptr;
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 0;
    }
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void processDataViewUpdate(DataRefIndex , MillisecondTime ) {}
    
    void initialize() {
        this->assign_defaults();
        this->setState();
    }
    
    protected:
    
    void button_01_bangval_bang() {
        this->sendParameter(0, true);
    }
    
    void numberobj_05_value_set(number v) {
        this->numberobj_05_value_setter(v);
        v = this->numberobj_05_value;
        this->sendParameter(1, false);
        this->numberobj_05_output_set(v);
    }
    
    void numberobj_05_format_set(number v) {
        if (v == 0) {
            this->numberobj_05_currentFormat = 0;
        } else if (v == 1) {
            this->numberobj_05_currentFormat = 1;
        } else if (v == 2) {
            this->numberobj_05_currentFormat = 2;
        } else if (v == 3) {
            this->numberobj_05_currentFormat = 3;
        } else if (v == 4) {
            this->numberobj_05_currentFormat = 4;
        } else if (v == 5) {
            this->numberobj_05_currentFormat = 5;
        } else if (v == 6) {
            this->numberobj_05_currentFormat = 6;
        }
    }
    
    void numberobj_06_value_set(number v) {
        this->numberobj_06_value_setter(v);
        v = this->numberobj_06_value;
        this->sendParameter(2, false);
        this->numberobj_06_output_set(v);
    }
    
    void numberobj_06_format_set(number v) {
        if (v == 0) {
            this->numberobj_06_currentFormat = 0;
        } else if (v == 1) {
            this->numberobj_06_currentFormat = 1;
        } else if (v == 2) {
            this->numberobj_06_currentFormat = 2;
        } else if (v == 3) {
            this->numberobj_06_currentFormat = 3;
        } else if (v == 4) {
            this->numberobj_06_currentFormat = 4;
        } else if (v == 5) {
            this->numberobj_06_currentFormat = 5;
        } else if (v == 6) {
            this->numberobj_06_currentFormat = 6;
        }
    }
    
    void numberobj_07_format_set(number v) {
        if (v == 0) {
            this->numberobj_07_currentFormat = 0;
        } else if (v == 1) {
            this->numberobj_07_currentFormat = 1;
        } else if (v == 2) {
            this->numberobj_07_currentFormat = 2;
        } else if (v == 3) {
            this->numberobj_07_currentFormat = 3;
        } else if (v == 4) {
            this->numberobj_07_currentFormat = 4;
        } else if (v == 5) {
            this->numberobj_07_currentFormat = 5;
        } else if (v == 6) {
            this->numberobj_07_currentFormat = 6;
        }
    }
    
    void numberobj_08_format_set(number v) {
        if (v == 0) {
            this->numberobj_08_currentFormat = 0;
        } else if (v == 1) {
            this->numberobj_08_currentFormat = 1;
        } else if (v == 2) {
            this->numberobj_08_currentFormat = 2;
        } else if (v == 3) {
            this->numberobj_08_currentFormat = 3;
        } else if (v == 4) {
            this->numberobj_08_currentFormat = 4;
        } else if (v == 5) {
            this->numberobj_08_currentFormat = 5;
        } else if (v == 6) {
            this->numberobj_08_currentFormat = 6;
        }
    }
    
    void numberobj_09_value_set(number v) {
        this->numberobj_09_value_setter(v);
        v = this->numberobj_09_value;
        this->sendParameter(5, false);
        this->numberobj_09_output_set(v);
    }
    
    void numberobj_09_format_set(number v) {
        if (v == 0) {
            this->numberobj_09_currentFormat = 0;
        } else if (v == 1) {
            this->numberobj_09_currentFormat = 1;
        } else if (v == 2) {
            this->numberobj_09_currentFormat = 2;
        } else if (v == 3) {
            this->numberobj_09_currentFormat = 3;
        } else if (v == 4) {
            this->numberobj_09_currentFormat = 4;
        } else if (v == 5) {
            this->numberobj_09_currentFormat = 5;
        } else if (v == 6) {
            this->numberobj_09_currentFormat = 6;
        }
    }
    
    void button_02_bangval_bang() {
        this->sendParameter(6, true);
        this->timer_01_end_bang();
        this->timer_01_start_bang();
    }
    
    void inport_02_value_bang_bang() {
        this->inport_02_out_bang_bang();
    }
    
    void inport_02_value_number_set(number v) {
        this->inport_02_out_number_set(v);
    }
    
    void inport_02_value_list_set(const list& v) {
        this->inport_02_out_list_set(v);
    }
    
    void numberobj_10_value_set(number v) {
        this->numberobj_10_value_setter(v);
        v = this->numberobj_10_value;
        this->sendParameter(8, false);
        this->numberobj_10_output_set(v);
    }
    
    void numberobj_10_format_set(number v) {
        if (v == 0) {
            this->numberobj_10_currentFormat = 0;
        } else if (v == 1) {
            this->numberobj_10_currentFormat = 1;
        } else if (v == 2) {
            this->numberobj_10_currentFormat = 2;
        } else if (v == 3) {
            this->numberobj_10_currentFormat = 3;
        } else if (v == 4) {
            this->numberobj_10_currentFormat = 4;
        } else if (v == 5) {
            this->numberobj_10_currentFormat = 5;
        } else if (v == 6) {
            this->numberobj_10_currentFormat = 6;
        }
    }
    
    void inport_03_value_bang_bang() {
        this->inport_03_out_bang_bang();
    }
    
    void inport_03_value_number_set(number v) {
        this->inport_03_out_number_set(v);
    }
    
    void inport_03_value_list_set(const list& v) {
        this->inport_03_out_list_set(v);
    }
    
    number msToSamps(MillisecondTime ms, number sampleRate) {
        return ms * sampleRate * 0.001;
    }
    
    MillisecondTime sampsToMs(SampleIndex samps) {
        return samps * (this->invsr * 1000);
    }
    
    Index getMaxBlockSize() const {
        return this->maxvs;
    }
    
    number getSampleRate() const {
        return this->sr;
    }
    
    bool hasFixedVectorSize() const {
        return false;
    }
    
    Index getNumInputChannels() const {
        return 0;
    }
    
    Index getNumOutputChannels() const {
        return 0;
    }
    
    void initializeObjects() {
        this->numberobj_05_init();
        this->numberobj_06_init();
        this->numberobj_07_init();
        this->numberobj_08_init();
        this->numberobj_09_init();
        this->numberobj_10_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {}
    
    void eventoutlet_01_in1_number_set(number v) {
        this->getPatcher()->p_04_out1_number_set(v);
    }
    
    void trigger_01_out2_set(number v) {
        this->eventoutlet_01_in1_number_set(v);
    }
    
    void eventoutlet_02_in1_list_set(const list& v) {
        this->getPatcher()->p_04_out2_list_set((list)v);
    }
    
    void message_01_out_set(const list& v) {
        this->eventoutlet_02_in1_list_set(v);
    }
    
    void message_01_bangval_bang() {
        this->sendParameter(7, true);
        this->message_01_out_set({0});
    }
    
    void trigger_01_out1_bang() {
        this->message_01_bangval_bang();
    }
    
    void trigger_01_input_number_set(number v) {
        this->trigger_01_out2_set(v);
        this->trigger_01_out1_bang();
    }
    
    void numberobj_08_output_set(number v) {
        this->trigger_01_input_number_set(v);
    }
    
    void numberobj_08_value_set(number v) {
        this->numberobj_08_value_setter(v);
        v = this->numberobj_08_value;
        this->sendParameter(4, false);
        this->numberobj_08_output_set(v);
    }
    
    void scale_02_out_set(const list& v) {
        this->scale_02_out = jsCreateListCopy(v);
    
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_08_value_set(converted);
        }
    }
    
    void scale_02_input_set(const list& v) {
        this->scale_02_input = jsCreateListCopy(v);
        list tmp = {};
    
        for (Index i = 0; i < v->length; i++) {
            tmp->push(this->scale(
                v[(Index)i],
                this->scale_02_inlow,
                this->scale_02_inhigh,
                this->scale_02_outlow,
                this->scale_02_outhigh,
                this->scale_02_power
            ));
        }
    
        this->scale_02_out_set(tmp);
    }
    
    void expr_03_out1_set(number v) {
        this->expr_03_out1 = v;
    
        {
            list converted = {this->expr_03_out1};
            this->scale_02_input_set(converted);
        }
    }
    
    void expr_03_in1_set(number in1) {
        this->expr_03_in1 = in1;
    
        this->expr_03_out1_set(
            (this->expr_03_in1 > this->expr_03_in3 ? this->expr_03_in3 : (this->expr_03_in1 < this->expr_03_in2 ? this->expr_03_in2 : this->expr_03_in1))
        );//#map:TapTempo/clip_obj-14:1
    }
    
    void numberobj_07_output_set(number v) {
        this->expr_03_in1_set(v);
    }
    
    void numberobj_07_value_set(number v) {
        this->numberobj_07_value_setter(v);
        v = this->numberobj_07_value;
        this->sendParameter(3, false);
        this->numberobj_07_output_set(v);
    }
    
    void expr_04_out1_set(number v) {
        this->expr_04_out1 = v;
        this->numberobj_07_value_set(this->expr_04_out1);
    }
    
    void expr_04_in1_set(number in1) {
        this->expr_04_in1 = in1;
        this->expr_04_out1_set((this->expr_04_in1 == 0. ? 0. : this->expr_04_in2 / this->expr_04_in1));//#map:TapTempo/!/_obj-10:1
    }
    
    void numberobj_05_output_set(number v) {
        this->expr_04_in1_set(v);
    }
    
    void expr_02_out1_set(number v) {
        this->expr_02_out1 = v;
        this->numberobj_05_value_set(this->expr_02_out1);
    }
    
    void expr_02_in1_set(number in1) {
        this->expr_02_in1 = in1;
        this->expr_02_out1_set(this->expr_02_in1 * this->expr_02_in2);//#map:TapTempo/*_obj-72:1
    }
    
    void numberobj_06_output_set(number v) {
        this->expr_02_in1_set(v);
    }
    
    void listmedian_01_out1_set(number v) {
        this->numberobj_06_value_set(v);
        this->button_01_bangval_bang();
    }
    
    void listmedian_01_input_set(const list& input) {
        this->listmedian_01_input = jsCreateListCopy(input);
    
        if (input->length == 1) {
            this->listmedian_01_out1_set(input[0]);
            return;
        }
    
        array<list, 2> sortedlistAndIndices = this->listsort(input, 1);
        const list& sortedList = sortedlistAndIndices[0];
        number median;
        number l = sortedList->length / (number)2;
        number lf = rnbo_floor(l);
    
        if (l > lf) {
            median = sortedList[(Index)lf];
        } else {
            median = (sortedList[(Index)(l - 1)] + sortedList[(Index)l]) / (number)2;
        }
    
        {
            this->listmedian_01_out1_set(median);
            return;
        }
    }
    
    void gate_01_out1_list_set(const list& v) {
        this->gate_01_out1_list = jsCreateListCopy(v);
        this->listmedian_01_input_set(v);
    }
    
    void gate_01_input_list_set(const list& v) {
        if (this->gate_01_onoff == 1)
            this->gate_01_out1_list_set(v);
    }
    
    void listgroup_01_out_set(const list& v) {
        this->gate_01_input_list_set(v);
    }
    
    void listgroup_01_input_number_set(number v) {
        this->listgroup_01_grouped = this->listgroup_01_grouped->concat(v);
        this->listgroup_01_checkOutput();
    }
    
    void numberobj_09_output_set(number v) {
        this->listgroup_01_input_number_set(v);
    }
    
    void split_01_out1_set(number v) {
        this->numberobj_09_value_set(v);
    }
    
    void gate_01_onoff_set(number v) {
        this->gate_01_onoff = v;
    }
    
    void trigger_02_out3_set(number v) {
        this->gate_01_onoff_set(v);
    }
    
    void listgroup_01_input_bang_bang() {
        if (this->listgroup_01_grouped->length > 0) {
            this->listgroup_01_out_set(this->listgroup_01_grouped);
            this->listgroup_01_grouped->length = 0;
        }
    }
    
    void trigger_02_out2_bang() {
        this->listgroup_01_input_bang_bang();
    }
    
    void trigger_02_out1_set(number v) {
        this->gate_01_onoff_set(v);
    }
    
    void trigger_02_input_bang_bang() {
        this->trigger_02_out3_set(0);
        this->trigger_02_out2_bang();
        this->trigger_02_out1_set(1);
    }
    
    void button_03_bangval_bang() {
        this->sendParameter(9, true);
        this->trigger_02_input_bang_bang();
    }
    
    void split_01_out2_set(number v) {
        RNBO_UNUSED(v);
        this->button_03_bangval_bang();
    }
    
    void split_01_input_set(number v) {
        this->split_01_input = v;
    
        if (v >= this->split_01_min && v <= this->split_01_max)
            this->split_01_out1_set(v);
        else
            this->split_01_out2_set(v);
    }
    
    void timer_01_out_set(number v) {
        this->split_01_input_set(v);
    }
    
    void timer_01_end_bang() {
        this->timer_01_elapsed = this->currenttime();
        this->timer_01_out_set(this->timer_01_elapsed - this->timer_01_starttime);
        this->timer_01_elapsed = 0;
    }
    
    void timer_01_start_bang() {
        this->timer_01_starttime = this->currenttime();
    }
    
    void inport_02_out_bang_bang() {
        this->button_02_bangval_bang();
    }
    
    void inport_02_out_number_set(number v) {
        RNBO_UNUSED(v);
        this->button_02_bangval_bang();
    }
    
    void inport_02_out_list_set(const list& v) {
        RNBO_UNUSED(v);
        this->button_02_bangval_bang();
    }
    
    void expr_02_in2_set(number v) {
        this->expr_02_in2 = v;
    }
    
    void numberobj_10_output_set(number v) {
        this->expr_02_in2_set(v);
    }
    
    void numberobj_10_value_bang() {
        number v = this->numberobj_10_value;
        this->sendParameter(8, false);
        this->numberobj_10_output_set(v);
    }
    
    void inport_03_out_bang_bang() {
        this->numberobj_10_value_bang();
    }
    
    void inport_03_out_number_set(number v) {
        this->numberobj_10_value_set(v);
    }
    
    void inport_03_out_list_set(const list& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_10_value_set(converted);
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void numberobj_05_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_05_currentFormat != 6) {
            localvalue = rnbo_trunc(localvalue);
        }
    
        this->numberobj_05_value = localvalue;
    }
    
    void numberobj_06_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_06_currentFormat != 6) {
            localvalue = rnbo_trunc(localvalue);
        }
    
        this->numberobj_06_value = localvalue;
    }
    
    void numberobj_07_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_07_currentFormat != 6) {
            localvalue = rnbo_trunc(localvalue);
        }
    
        this->numberobj_07_value = localvalue;
    }
    
    void numberobj_08_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_08_currentFormat != 6) {
            localvalue = rnbo_trunc(localvalue);
        }
    
        this->numberobj_08_value = localvalue;
    }
    
    void numberobj_09_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_09_currentFormat != 6) {
            localvalue = rnbo_trunc(localvalue);
        }
    
        this->numberobj_09_value = localvalue;
    }
    
    void numberobj_10_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_10_currentFormat != 6) {
            localvalue = rnbo_trunc(localvalue);
        }
    
        this->numberobj_10_value = localvalue;
    }
    
    void numberobj_05_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("TapTempo/number_obj-75"), 1, this->_currentTime);
    }
    
    void numberobj_05_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_05_value;
    }
    
    void numberobj_05_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_05_value_set(preset["value"]);
    }
    
    void numberobj_06_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("TapTempo/number_obj-64"), 1, this->_currentTime);
    }
    
    void numberobj_06_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_06_value;
    }
    
    void numberobj_06_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_06_value_set(preset["value"]);
    }
    
    void numberobj_07_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("TapTempo/number_obj-19"), 1, this->_currentTime);
    }
    
    void numberobj_07_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_07_value;
    }
    
    void numberobj_07_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_07_value_set(preset["value"]);
    }
    
    void numberobj_08_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("TapTempo/number_obj-17"), 1, this->_currentTime);
    }
    
    void numberobj_08_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_08_value;
    }
    
    void numberobj_08_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_08_value_set(preset["value"]);
    }
    
    void numberobj_09_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("TapTempo/number_obj-6"), 1, this->_currentTime);
    }
    
    void numberobj_09_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_09_value;
    }
    
    void numberobj_09_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_09_value_set(preset["value"]);
    }
    
    void listgroup_01_checkOutput() {
        const Index isize = (const Index)(this->listgroup_01_size);
    
        while (this->listgroup_01_grouped->length >= isize) {
            list tmp = list();
    
            for (Index i = 0; i < isize; i++) {
                tmp->push(this->listgroup_01_grouped[(Index)i]);
            }
    
            this->listgroup_01_out_set(tmp);
            this->listgroup_01_grouped->splice(0, isize);
        }
    }
    
    void numberobj_10_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("TapTempo/number_obj-70"), 1, this->_currentTime);
    }
    
    void numberobj_10_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_10_value;
    }
    
    void numberobj_10_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_10_value_set(preset["value"]);
    }
    
    bool stackprotect_check() {
        this->stackprotect_count++;
    
        if (this->stackprotect_count > 128) {
            console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
            return true;
        }
    
        return false;
    }
    
    void updateTime(MillisecondTime time) {
        this->_currentTime = time;
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));
    
        if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
            this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;
    
        if (this->sampleOffsetIntoNextAudioBuffer < 0)
            this->sampleOffsetIntoNextAudioBuffer = 0;
    }
    
    void assign_defaults()
    {
        numberobj_05_value = 0;
        numberobj_05_value_setter(numberobj_05_value);
        expr_02_in1 = 0;
        expr_02_in2 = 1;
        expr_02_out1 = 0;
        gate_01_onoff = 1;
        gate_01_out1_number = 0;
        numberobj_06_value = 0;
        numberobj_06_value_setter(numberobj_06_value);
        expr_03_in1 = 0;
        expr_03_in2 = 0.125;
        expr_03_in3 = 8;
        expr_03_out1 = 0;
        numberobj_07_value = 0;
        numberobj_07_value_setter(numberobj_07_value);
        numberobj_08_value = 0;
        numberobj_08_value_setter(numberobj_08_value);
        expr_04_in1 = 0;
        expr_04_in2 = 1000;
        expr_04_out1 = 0;
        scale_02_inlow = 0.125;
        scale_02_inhigh = 8;
        scale_02_outlow = 0;
        scale_02_outhigh = 100;
        scale_02_power = 0.5;
        numberobj_09_value = 0;
        numberobj_09_value_setter(numberobj_09_value);
        listgroup_01_size = 2;
        split_01_input = 0;
        split_01_min = 0;
        split_01_max = 2000;
        numberobj_10_value = 1;
        numberobj_10_value_setter(numberobj_10_value);
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        numberobj_05_currentFormat = 6;
        numberobj_05_lastValue = 0;
        numberobj_06_currentFormat = 6;
        numberobj_06_lastValue = 0;
        numberobj_07_currentFormat = 6;
        numberobj_07_lastValue = 0;
        numberobj_08_currentFormat = 6;
        numberobj_08_lastValue = 0;
        numberobj_09_currentFormat = 6;
        numberobj_09_lastValue = 0;
        timer_01_starttime = 0;
        timer_01_elapsed = 0;
        numberobj_10_currentFormat = 6;
        numberobj_10_lastValue = 0;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number numberobj_05_value;
        number expr_02_in1;
        number expr_02_in2;
        number expr_02_out1;
        number gate_01_onoff;
        number gate_01_out1_number;
        list gate_01_out1_list;
        number numberobj_06_value;
        list listmedian_01_input;
        number expr_03_in1;
        number expr_03_in2;
        number expr_03_in3;
        number expr_03_out1;
        number numberobj_07_value;
        number numberobj_08_value;
        number expr_04_in1;
        number expr_04_in2;
        number expr_04_out1;
        list scale_02_input;
        number scale_02_inlow;
        number scale_02_inhigh;
        number scale_02_outlow;
        number scale_02_outhigh;
        number scale_02_power;
        list scale_02_out;
        number numberobj_09_value;
        number listgroup_01_size;
        number split_01_input;
        number split_01_min;
        number split_01_max;
        number numberobj_10_value;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        Int numberobj_05_currentFormat;
        number numberobj_05_lastValue;
        Int numberobj_06_currentFormat;
        number numberobj_06_lastValue;
        Int numberobj_07_currentFormat;
        number numberobj_07_lastValue;
        Int numberobj_08_currentFormat;
        number numberobj_08_lastValue;
        Int numberobj_09_currentFormat;
        number numberobj_09_lastValue;
        list listgroup_01_grouped;
        number timer_01_starttime;
        number timer_01_elapsed;
        Int numberobj_10_currentFormat;
        number numberobj_10_lastValue;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

rnbomatic()
{
}

~rnbomatic()
{
    delete this->p_01;
    delete this->p_02;
    delete this->p_03;
    delete this->p_04;
}

rnbomatic* getTopLevelPatcher() {
    return this;
}

void cancelClockEvents()
{
    getEngine()->flushClockEvents(this, 760652352, false);
    getEngine()->flushClockEvents(this, -798469121, false);
    getEngine()->flushClockEvents(this, 2098551528, false);
    getEngine()->flushClockEvents(this, -1494586265, false);
    getEngine()->flushClockEvents(this, 1396722025, false);
}

template <typename T> void listquicksort(T& arr, T& sortindices, Int l, Int h, bool ascending) {
    if (l < h) {
        Int p = (Int)(this->listpartition(arr, sortindices, l, h, ascending));
        this->listquicksort(arr, sortindices, l, p - 1, ascending);
        this->listquicksort(arr, sortindices, p + 1, h, ascending);
    }
}

template <typename T> Int listpartition(T& arr, T& sortindices, Int l, Int h, bool ascending) {
    number x = arr[(Index)h];
    Int i = (Int)(l - 1);

    for (Int j = (Int)(l); j <= h - 1; j++) {
        bool asc = (bool)((bool)(ascending) && arr[(Index)j] <= x);
        bool desc = (bool)((bool)(!(bool)(ascending)) && arr[(Index)j] >= x);

        if ((bool)(asc) || (bool)(desc)) {
            i++;
            this->listswapelements(arr, i, j);
            this->listswapelements(sortindices, i, j);
        }
    }

    i++;
    this->listswapelements(arr, i, h);
    this->listswapelements(sortindices, i, h);
    return i;
}

template <typename T> void listswapelements(T& arr, Int a, Int b) {
    auto tmp = arr[(Index)a];
    arr[(Index)a] = arr[(Index)b];
    arr[(Index)b] = tmp;
}

inline number linearinterp(number frac, number x, number y) {
    return x + (y - x) * frac;
}

inline number cubicinterp(number a, number w, number x, number y, number z) {
    number a2 = a * a;
    number f0 = z - y - w + x;
    number f1 = w - x - f0;
    number f2 = y - w;
    number f3 = x;
    return f0 * a * a2 + f1 * a2 + f2 * a + f3;
}

inline number splineinterp(number a, number w, number x, number y, number z) {
    number a2 = a * a;
    number f0 = -0.5 * w + 1.5 * x - 1.5 * y + 0.5 * z;
    number f1 = w - 2.5 * x + 2 * y - 0.5 * z;
    number f2 = -0.5 * w + 0.5 * y;
    return f0 * a * a2 + f1 * a2 + f2 * a + x;
}

inline number cosT8(number r) {
    number t84 = 56.0;
    number t83 = 1680.0;
    number t82 = 20160.0;
    number t81 = 2.4801587302e-05;
    number t73 = 42.0;
    number t72 = 840.0;
    number t71 = 1.9841269841e-04;

    if (r < 0.785398163397448309615660845819875721 && r > -0.785398163397448309615660845819875721) {
        number rr = r * r;
        return 1.0 - rr * t81 * (t82 - rr * (t83 - rr * (t84 - rr)));
    } else if (r > 0.0) {
        r -= 1.57079632679489661923132169163975144;
        number rr = r * r;
        return -r * (1.0 - t71 * rr * (t72 - rr * (t73 - rr)));
    } else {
        r += 1.57079632679489661923132169163975144;
        number rr = r * r;
        return r * (1.0 - t71 * rr * (t72 - rr * (t73 - rr)));
    }
}

inline number cosineinterp(number frac, number x, number y) {
    number a2 = (1.0 - this->cosT8(frac * 3.14159265358979323846)) / (number)2.0;
    return x * (1.0 - a2) + y * a2;
}

number mstosamps(MillisecondTime ms) {
    return ms * this->sr * 0.001;
}

number samplerate() {
    return this->sr;
}

Index vectorsize() {
    return this->vs;
}

number maximum(number x, number y) {
    return (x < y ? y : x);
}

inline number safediv(number num, number denom) {
    return (denom == 0.0 ? 0.0 : num / denom);
}

number wrap(number x, number low, number high) {
    number lo;
    number hi;

    if (low == high)
        return low;

    if (low > high) {
        hi = low;
        lo = high;
    } else {
        lo = low;
        hi = high;
    }

    number range = hi - lo;

    if (x >= lo && x < hi)
        return x;

    if (range <= 0.000000001)
        return lo;

    long numWraps = (long)(rnbo_trunc((x - lo) / range));
    numWraps = numWraps - ((x < lo ? 1 : 0));
    number result = x - range * numWraps;

    if (result >= hi)
        return result - range;
    else
        return result;
}

number safepow(number base, number exponent) {
    return fixnan(rnbo_pow(base, exponent));
}

SampleIndex currentsampletime() {
    return this->audioProcessSampleCount + this->sampleOffsetIntoNextAudioBuffer;
}

number scale(
    number x,
    number lowin,
    number hiin,
    number lowout,
    number highout,
    number pow
) {
    number inscale = this->safediv(1., hiin - lowin);
    number outdiff = highout - lowout;
    number value = (x - lowin) * inscale;

    if (pow != 1) {
        if (value > 0)
            value = this->safepow(value, pow);
        else
            value = -this->safepow(-value, pow);
    }

    value = value * outdiff + lowout;
    return value;
}

ParameterValue fromnormalized(ParameterIndex index, ParameterValue normalizedValue) {
    return this->convertFromNormalizedParameterValue(index, normalizedValue);
}

MillisecondTime currenttime() {
    return this->_currentTime;
}

number tempo() {
    return this->getTopLevelPatcher()->globaltransport_getTempo();
}

number mstobeats(number ms) {
    return ms * this->tempo() * 0.008 / (number)480;
}

MillisecondTime sampstoms(number samps) {
    return samps * 1000 / this->sr;
}

ParameterIndex getParameterIndexForID(ConstCharPointer paramid) const {
    if (!stringCompare(paramid, "rate")) {
        return 0;
    }

    if (!stringCompare(paramid, "depth")) {
        return 1;
    }

    if (!stringCompare(paramid, "spread")) {
        return 2;
    }

    if (!stringCompare(paramid, "number_obj-41/value")) {
        return 3;
    }

    if (!stringCompare(paramid, "number_obj-60/value")) {
        return 4;
    }

    if (!stringCompare(paramid, "number_obj-64/value")) {
        return 5;
    }

    if (!stringCompare(paramid, "number_obj-55/value")) {
        return 6;
    }

    if (!stringCompare(paramid, "number_obj-37/value")) {
        return 7;
    }

    return INVALID_INDEX;
}

Index getNumMidiInputPorts() const {
    return 1;
}

void processMidiEvent(MillisecondTime time, int port, ConstByteArray data, Index length) {
    this->updateTime(time);
    this->ctlin_01_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
    this->ctlin_02_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
    this->ctlin_03_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
}

Index getNumMidiOutputPorts() const {
    return 0;
}

void process(
    SampleValue ** inputs,
    Index numInputs,
    SampleValue ** outputs,
    Index numOutputs,
    Index n
) {
    this->vs = n;
    this->updateTime(this->getEngine()->getCurrentTime());
    SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
    SampleValue * out2 = (numOutputs >= 2 && outputs[1] ? outputs[1] : this->dummyBuffer);
    SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
    SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
    SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
    this->p_04_perform(n);
    this->p_03_perform(in3, this->signals[0], n);
    this->scopetilde_03_perform(this->signals[0], this->zeroBuffer, n);
    this->triangle_tilde_01_perform(this->signals[0], this->triangle_tilde_01_duty, this->signals[1], n);
    this->scopetilde_02_perform(this->signals[1], this->zeroBuffer, n);
    this->line_02_perform(this->signals[2], n);
    this->dspexpr_11_perform(this->signals[0], this->signals[2], this->signals[3], n);
    this->triangle_tilde_02_perform(this->signals[3], this->triangle_tilde_02_duty, this->signals[2], n);
    this->scopetilde_01_perform(this->signals[2], this->zeroBuffer, n);
    this->line_03_perform(this->signals[3], n);
    this->dspexpr_10_perform(this->signals[2], this->signals[3], this->signals[0], n);

    this->slide_tilde_02_perform(
        this->signals[0],
        this->slide_tilde_02_up,
        this->slide_tilde_02_down,
        this->signals[2],
        n
    );

    this->dspexpr_09_perform(this->signals[2], this->signals[0], n);
    this->mstosamps_tilde_02_perform(this->signals[0], this->signals[4], n);
    this->numbertilde_07_perform(this->signals[2], this->dummyBuffer, n);
    this->dspexpr_08_perform(this->signals[1], this->signals[3], this->signals[2], n);

    this->slide_tilde_01_perform(
        this->signals[2],
        this->slide_tilde_01_up,
        this->slide_tilde_01_down,
        this->signals[1],
        n
    );

    this->dspexpr_07_perform(this->signals[1], this->signals[2], n);
    this->mstosamps_tilde_01_perform(this->signals[2], this->signals[5], n);
    this->numbertilde_06_perform(this->signals[1], this->dummyBuffer, n);

    this->scale_tilde_05_perform(
        this->signals[3],
        this->scale_tilde_05_lowin,
        this->scale_tilde_05_hiin,
        this->scale_tilde_05_lowout,
        this->scale_tilde_05_highout,
        this->scale_tilde_05_pow,
        this->signals[1],
        n
    );

    this->numbertilde_08_perform(this->signals[1], this->dummyBuffer, n);
    this->dspexpr_12_perform(this->signals[3], this->signals[6], n);

    this->p_02_perform(
        in1,
        in2,
        this->signals[2],
        this->signals[0],
        this->signals[6],
        this->signals[3],
        this->signals[7],
        n
    );

    this->p_01_perform(
        this->signals[3],
        this->signals[7],
        this->signals[2],
        this->signals[0],
        this->signals[6],
        this->signals[8],
        this->signals[9],
        n
    );

    this->delaytilde_01_perform(this->signals[5], this->signals[8], this->signals[6], n);
    this->dspexpr_04_perform(in1, this->signals[6], this->signals[5], n);
    this->dspexpr_03_perform(this->signals[5], this->signals[1], out1, n);
    this->delaytilde_02_perform(this->signals[4], this->signals[9], this->signals[5], n);
    this->dspexpr_06_perform(in2, this->signals[5], this->signals[4], n);
    this->dspexpr_05_perform(this->signals[4], this->signals[1], out2, n);
    this->stackprotect_perform(n);
    this->globaltransport_advance();
    this->audioProcessSampleCount += this->vs;
}

void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
    if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
        Index i;

        for (i = 0; i < 10; i++) {
            this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
        }

        this->globaltransport_tempo = resizeSignal(this->globaltransport_tempo, this->maxvs, maxBlockSize);
        this->globaltransport_state = resizeSignal(this->globaltransport_state, this->maxvs, maxBlockSize);
        this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
        this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
        this->didAllocateSignals = true;
    }

    const bool sampleRateChanged = sampleRate != this->sr;
    const bool maxvsChanged = maxBlockSize != this->maxvs;
    const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;

    if (sampleRateChanged || maxvsChanged) {
        this->vs = maxBlockSize;
        this->maxvs = maxBlockSize;
        this->sr = sampleRate;
        this->invsr = 1 / sampleRate;
    }

    this->scopetilde_03_dspsetup(forceDSPSetup);
    this->scopetilde_02_dspsetup(forceDSPSetup);
    this->scopetilde_01_dspsetup(forceDSPSetup);
    this->numbertilde_07_dspsetup(forceDSPSetup);
    this->numbertilde_06_dspsetup(forceDSPSetup);
    this->numbertilde_08_dspsetup(forceDSPSetup);
    this->delaytilde_01_dspsetup(forceDSPSetup);
    this->delaytilde_02_dspsetup(forceDSPSetup);
    this->globaltransport_dspsetup(forceDSPSetup);
    this->p_01->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_02->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_03->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_04->prepareToProcess(sampleRate, maxBlockSize, force);

    if (sampleRateChanged)
        this->onSampleRateChanged(sampleRate);
}

void setProbingTarget(MessageTag id) {
    switch (id) {
    default:
        this->setProbingIndex(-1);
        break;
    }
}

void setProbingIndex(ProbingIndex ) {}

Index getProbingChannels(MessageTag outletId) const {
    RNBO_UNUSED(outletId);
    return 0;
}

DataRef* getDataRef(DataRefIndex index)  {
    switch (index) {
    case 0:
        return addressOf(this->delaytilde_01_del_bufferobj);
        break;
    case 1:
        return addressOf(this->delaytilde_02_del_bufferobj);
        break;
    default:
        return nullptr;
    }
}

DataRefIndex getNumDataRefs() const {
    return 2;
}

void fillDataRef(DataRefIndex , DataRef& ) {}

void zeroDataRef(DataRef& ref) {
    ref->setZero();
}

void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
    this->updateTime(time);

    if (index == 0) {
        this->delaytilde_01_del_buffer = new Float64Buffer(this->delaytilde_01_del_bufferobj);
    }

    if (index == 1) {
        this->delaytilde_02_del_buffer = new Float64Buffer(this->delaytilde_02_del_bufferobj);
    }

    this->p_01->processDataViewUpdate(index, time);
    this->p_02->processDataViewUpdate(index, time);
    this->p_03->processDataViewUpdate(index, time);
    this->p_04->processDataViewUpdate(index, time);
}

void initialize() {
    this->delaytilde_01_del_bufferobj = initDataRef("delaytilde_01_del_bufferobj", true, nullptr);
    this->delaytilde_02_del_bufferobj = initDataRef("delaytilde_02_del_bufferobj", true, nullptr);
    this->assign_defaults();
    this->setState();
    this->delaytilde_01_del_bufferobj->setIndex(0);
    this->delaytilde_01_del_buffer = new Float64Buffer(this->delaytilde_01_del_bufferobj);
    this->delaytilde_02_del_bufferobj->setIndex(1);
    this->delaytilde_02_del_buffer = new Float64Buffer(this->delaytilde_02_del_bufferobj);
    this->initializeObjects();
    this->allocateDataRefs();
    this->startup();
}

Index getIsMuted()  {
    return this->isMuted;
}

void setIsMuted(Index v)  {
    this->isMuted = v;
}

Index getPatcherSerial() const {
    return 0;
}

void getState(PatcherStateInterface& ) {}

void setState() {
    this->p_01 = new RNBOSubpatcher_13();
    this->p_01->setEngineAndPatcher(this->getEngine(), this);
    this->p_01->initialize();
    this->p_01->setParameterOffset(this->getParameterOffset(this->p_01));
    this->p_02 = new RNBOSubpatcher_14();
    this->p_02->setEngineAndPatcher(this->getEngine(), this);
    this->p_02->initialize();
    this->p_02->setParameterOffset(this->getParameterOffset(this->p_02));
    this->p_03 = new RNBOSubpatcher_15();
    this->p_03->setEngineAndPatcher(this->getEngine(), this);
    this->p_03->initialize();
    this->p_03->setParameterOffset(this->getParameterOffset(this->p_03));
    this->p_04 = new RNBOSubpatcher_16();
    this->p_04->setEngineAndPatcher(this->getEngine(), this);
    this->p_04->initialize();
    this->p_04->setParameterOffset(this->getParameterOffset(this->p_04));
}

void getPreset(PatcherStateInterface& preset) {
    preset["__presetid"] = "rnbo";
    this->param_01_getPresetValue(getSubState(preset, "rate"));
    this->param_02_getPresetValue(getSubState(preset, "depth"));
    this->param_03_getPresetValue(getSubState(preset, "spread"));
    this->p_01->getPreset(getSubState(getSubState(preset, "__sps"), "LowCut~"));
    this->p_02->getPreset(getSubState(getSubState(preset, "__sps"), "HiCut~"));
    this->p_03->getPreset(getSubState(getSubState(preset, "__sps"), "LFO~"));
    this->p_04->getPreset(getSubState(getSubState(preset, "__sps"), "TapTempo"));
}

void setPreset(MillisecondTime time, PatcherStateInterface& preset) {
    this->updateTime(time);
    this->param_01_setPresetValue(getSubState(preset, "rate"));
    this->param_02_setPresetValue(getSubState(preset, "depth"));
    this->param_03_setPresetValue(getSubState(preset, "spread"));
}

void processTempoEvent(MillisecondTime time, Tempo tempo) {
    this->updateTime(time);

    if (this->globaltransport_setTempo(tempo, false)) {
        this->p_01->processTempoEvent(time, tempo);
        this->p_02->processTempoEvent(time, tempo);
        this->p_03->processTempoEvent(time, tempo);
        this->p_04->processTempoEvent(time, tempo);
    }
}

void processTransportEvent(MillisecondTime time, TransportState state) {
    this->updateTime(time);

    if (this->globaltransport_setState(state, false)) {
        this->p_01->processTransportEvent(time, state);
        this->p_02->processTransportEvent(time, state);
        this->p_03->processTransportEvent(time, state);
        this->p_04->processTransportEvent(time, state);
    }
}

void processBeatTimeEvent(MillisecondTime time, BeatTime beattime) {
    this->updateTime(time);

    if (this->globaltransport_setBeatTime(beattime, false)) {
        this->p_01->processBeatTimeEvent(time, beattime);
        this->p_02->processBeatTimeEvent(time, beattime);
        this->p_03->processBeatTimeEvent(time, beattime);
        this->p_04->processBeatTimeEvent(time, beattime);
    }
}

void onSampleRateChanged(double ) {}

void processTimeSignatureEvent(MillisecondTime time, int numerator, int denominator) {
    this->updateTime(time);

    if (this->globaltransport_setTimeSignature(numerator, denominator, false)) {
        this->p_01->processTimeSignatureEvent(time, numerator, denominator);
        this->p_02->processTimeSignatureEvent(time, numerator, denominator);
        this->p_03->processTimeSignatureEvent(time, numerator, denominator);
        this->p_04->processTimeSignatureEvent(time, numerator, denominator);
    }
}

void setParameterValue(ParameterIndex index, ParameterValue v, MillisecondTime time) {
    this->updateTime(time);

    switch (index) {
    case 0:
        this->param_01_value_set(v);
        break;
    case 1:
        this->param_02_value_set(v);
        break;
    case 2:
        this->param_03_value_set(v);
        break;
    case 3:
        this->numberobj_11_value_set(v);
        break;
    case 4:
        this->numberobj_12_value_set(v);
        break;
    case 5:
        this->numberobj_13_value_set(v);
        break;
    case 6:
        this->numberobj_14_value_set(v);
        break;
    case 7:
        this->numberobj_15_value_set(v);
        break;
    default:
        index -= 8;

        if (index < this->p_01->getNumParameters())
            this->p_01->setParameterValue(index, v, time);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            this->p_02->setParameterValue(index, v, time);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03->getNumParameters())
            this->p_03->setParameterValue(index, v, time);

        index -= this->p_03->getNumParameters();

        if (index < this->p_04->getNumParameters())
            this->p_04->setParameterValue(index, v, time);

        break;
    }
}

void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
    this->setParameterValue(index, value, time);
}

void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
    this->setParameterValueNormalized(index, value, time);
}

ParameterValue getParameterValue(ParameterIndex index)  {
    switch (index) {
    case 0:
        return this->param_01_value;
    case 1:
        return this->param_02_value;
    case 2:
        return this->param_03_value;
    case 3:
        return this->numberobj_11_value;
    case 4:
        return this->numberobj_12_value;
    case 5:
        return this->numberobj_13_value;
    case 6:
        return this->numberobj_14_value;
    case 7:
        return this->numberobj_15_value;
    default:
        index -= 8;

        if (index < this->p_01->getNumParameters())
            return this->p_01->getParameterValue(index);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->getParameterValue(index);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03->getNumParameters())
            return this->p_03->getParameterValue(index);

        index -= this->p_03->getNumParameters();

        if (index < this->p_04->getNumParameters())
            return this->p_04->getParameterValue(index);

        return 0;
    }
}

ParameterIndex getNumSignalInParameters() const {
    return 0;
}

ParameterIndex getNumSignalOutParameters() const {
    return 0;
}

ParameterIndex getNumParameters() const {
    return 8 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters();
}

ConstCharPointer getParameterName(ParameterIndex index) const {
    switch (index) {
    case 0:
        return "rate";
    case 1:
        return "depth";
    case 2:
        return "spread";
    case 3:
        return "numberobj_11_value";
    case 4:
        return "numberobj_12_value";
    case 5:
        return "numberobj_13_value";
    case 6:
        return "numberobj_14_value";
    case 7:
        return "numberobj_15_value";
    default:
        index -= 8;

        if (index < this->p_01->getNumParameters())
            return this->p_01->getParameterName(index);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->getParameterName(index);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03->getNumParameters())
            return this->p_03->getParameterName(index);

        index -= this->p_03->getNumParameters();

        if (index < this->p_04->getNumParameters())
            return this->p_04->getParameterName(index);

        return "bogus";
    }
}

ConstCharPointer getParameterId(ParameterIndex index) const {
    switch (index) {
    case 0:
        return "rate";
    case 1:
        return "depth";
    case 2:
        return "spread";
    case 3:
        return "number_obj-41/value";
    case 4:
        return "number_obj-60/value";
    case 5:
        return "number_obj-64/value";
    case 6:
        return "number_obj-55/value";
    case 7:
        return "number_obj-37/value";
    default:
        index -= 8;

        if (index < this->p_01->getNumParameters())
            return this->p_01->getParameterId(index);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->getParameterId(index);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03->getNumParameters())
            return this->p_03->getParameterId(index);

        index -= this->p_03->getNumParameters();

        if (index < this->p_04->getNumParameters())
            return this->p_04->getParameterId(index);

        return "bogus";
    }
}

void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
    {
        switch (index) {
        case 0:
            info->type = ParameterTypeNumber;
            info->initialValue = 50;
            info->min = 0;
            info->max = 100;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "Rate";
            info->unit = "%";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
        case 1:
            info->type = ParameterTypeNumber;
            info->initialValue = 50;
            info->min = 0;
            info->max = 100;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "Depth";
            info->unit = "%";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
        case 2:
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = 0;
            info->max = 100;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "Spread";
            info->unit = "%";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
        case 3:
            info->type = ParameterTypeNumber;
            info->initialValue = 50;
            info->min = 0;
            info->max = 100;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = false;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
        case 4:
            info->type = ParameterTypeNumber;
            info->initialValue = 3.7201824282796907;
            info->min = 0;
            info->max = 1;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = false;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
        case 5:
            info->type = ParameterTypeNumber;
            info->initialValue = 80;
            info->min = 0;
            info->max = 100;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = false;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
        case 6:
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = 0;
            info->max = 1;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = false;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
        case 7:
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = 0;
            info->max = 100;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = false;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
        default:
            index -= 8;

            if (index < this->p_01->getNumParameters())
                this->p_01->getParameterInfo(index, info);

            index -= this->p_01->getNumParameters();

            if (index < this->p_02->getNumParameters())
                this->p_02->getParameterInfo(index, info);

            index -= this->p_02->getNumParameters();

            if (index < this->p_03->getNumParameters())
                this->p_03->getParameterInfo(index, info);

            index -= this->p_03->getNumParameters();

            if (index < this->p_04->getNumParameters())
                this->p_04->getParameterInfo(index, info);

            break;
        }
    }
}

void sendParameter(ParameterIndex index, bool ignoreValue) {
    this->getEngine()->notifyParameterValueChanged(index, (ignoreValue ? 0 : this->getParameterValue(index)), ignoreValue);
}

ParameterIndex getParameterOffset(BaseInterface* subpatcher) const {
    if (subpatcher == this->p_01)
        return 8;

    if (subpatcher == this->p_02)
        return 8 + this->p_01->getNumParameters();

    if (subpatcher == this->p_03)
        return 8 + this->p_01->getNumParameters() + this->p_02->getNumParameters();

    if (subpatcher == this->p_04)
        return 8 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters();

    return 0;
}

ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
    if (steps == 1) {
        if (normalizedValue > 0) {
            normalizedValue = 1.;
        }
    } else {
        ParameterValue oneStep = (number)1. / (steps - 1);
        ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
        normalizedValue = numberOfSteps * oneStep;
    }

    return normalizedValue;
}

ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
    switch (index) {
    case 4:
    case 6:
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));
            ParameterValue normalizedValue = (value - 0) / (1 - 0);
            return normalizedValue;
        }
    case 0:
    case 1:
    case 2:
    case 3:
    case 5:
    case 7:
        {
            value = (value < 0 ? 0 : (value > 100 ? 100 : value));
            ParameterValue normalizedValue = (value - 0) / (100 - 0);
            return normalizedValue;
        }
    default:
        index -= 8;

        if (index < this->p_01->getNumParameters())
            return this->p_01->convertToNormalizedParameterValue(index, value);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->convertToNormalizedParameterValue(index, value);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03->getNumParameters())
            return this->p_03->convertToNormalizedParameterValue(index, value);

        index -= this->p_03->getNumParameters();

        if (index < this->p_04->getNumParameters())
            return this->p_04->convertToNormalizedParameterValue(index, value);

        return value;
    }
}

ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
    value = (value < 0 ? 0 : (value > 1 ? 1 : value));

    switch (index) {
    case 4:
    case 6:
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                return 0 + value * (1 - 0);
            }
        }
    case 0:
    case 1:
    case 2:
    case 3:
    case 5:
    case 7:
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                return 0 + value * (100 - 0);
            }
        }
    default:
        index -= 8;

        if (index < this->p_01->getNumParameters())
            return this->p_01->convertFromNormalizedParameterValue(index, value);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->convertFromNormalizedParameterValue(index, value);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03->getNumParameters())
            return this->p_03->convertFromNormalizedParameterValue(index, value);

        index -= this->p_03->getNumParameters();

        if (index < this->p_04->getNumParameters())
            return this->p_04->convertFromNormalizedParameterValue(index, value);

        return value;
    }
}

ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
    switch (index) {
    case 0:
        return this->param_01_value_constrain(value);
    case 1:
        return this->param_02_value_constrain(value);
    case 2:
        return this->param_03_value_constrain(value);
    default:
        index -= 8;

        if (index < this->p_01->getNumParameters())
            return this->p_01->constrainParameterValue(index, value);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->constrainParameterValue(index, value);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03->getNumParameters())
            return this->p_03->constrainParameterValue(index, value);

        index -= this->p_03->getNumParameters();

        if (index < this->p_04->getNumParameters())
            return this->p_04->constrainParameterValue(index, value);

        return value;
    }
}

void scheduleParamInit(ParameterIndex index, Index order) {
    this->paramInitIndices->push(index);
    this->paramInitOrder->push(order);
}

void processParamInitEvents() {
    this->listquicksort(
        this->paramInitOrder,
        this->paramInitIndices,
        0,
        (int)(this->paramInitOrder->length - 1),
        true
    );

    for (Index i = 0; i < this->paramInitOrder->length; i++) {
        this->getEngine()->scheduleParameterChange(
            this->paramInitIndices[i],
            this->getParameterValue(this->paramInitIndices[i]),
            0
        );
    }
}

void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
    RNBO_UNUSED(hasValue);
    this->updateTime(time);

    switch (index) {
    case 760652352:
        this->line_02_target_bang();
        break;
    case -798469121:
        this->line_03_target_bang();
        break;
    case 2098551528:
        this->numbertilde_07_value_set(value);
        break;
    case -1494586265:
        this->numbertilde_06_value_set(value);
        break;
    case 1396722025:
        this->numbertilde_08_value_set(value);
        break;
    }
}

void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}

void processOutletEvent(
    EngineLink* sender,
    OutletIndex index,
    ParameterValue value,
    MillisecondTime time
) {
    this->updateTime(time);
    this->processOutletAtCurrentTime(sender, index, value);
}

void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
    this->updateTime(time);

    switch (tag) {
    case TAG("format"):
        if (TAG("number_obj-41") == objectId)
            this->numberobj_11_format_set(payload);

        if (TAG("number_obj-60") == objectId)
            this->numberobj_12_format_set(payload);

        if (TAG("number_obj-64") == objectId)
            this->numberobj_13_format_set(payload);

        if (TAG("number_obj-55") == objectId)
            this->numberobj_14_format_set(payload);

        if (TAG("number_obj-37") == objectId)
            this->numberobj_15_format_set(payload);

        break;
    case TAG("sig"):
        if (TAG("number~_obj-32") == objectId)
            this->numbertilde_06_sig_number_set(payload);

        if (TAG("number~_obj-31") == objectId)
            this->numbertilde_07_sig_number_set(payload);

        if (TAG("number~_obj-30") == objectId)
            this->numbertilde_08_sig_number_set(payload);

        break;
    case TAG("mode"):
        if (TAG("number~_obj-32") == objectId)
            this->numbertilde_06_mode_set(payload);

        if (TAG("number~_obj-31") == objectId)
            this->numbertilde_07_mode_set(payload);

        if (TAG("number~_obj-30") == objectId)
            this->numbertilde_08_mode_set(payload);

        break;
    }

    this->p_01->processNumMessage(tag, objectId, time, payload);
    this->p_02->processNumMessage(tag, objectId, time, payload);
    this->p_03->processNumMessage(tag, objectId, time, payload);
    this->p_04->processNumMessage(tag, objectId, time, payload);
}

void processListMessage(
    MessageTag tag,
    MessageTag objectId,
    MillisecondTime time,
    const list& payload
) {
    this->updateTime(time);

    switch (tag) {
    case TAG("sig"):
        if (TAG("number~_obj-32") == objectId)
            this->numbertilde_06_sig_list_set(payload);

        if (TAG("number~_obj-31") == objectId)
            this->numbertilde_07_sig_list_set(payload);

        if (TAG("number~_obj-30") == objectId)
            this->numbertilde_08_sig_list_set(payload);

        break;
    }

    this->p_01->processListMessage(tag, objectId, time, payload);
    this->p_02->processListMessage(tag, objectId, time, payload);
    this->p_03->processListMessage(tag, objectId, time, payload);
    this->p_04->processListMessage(tag, objectId, time, payload);
}

void processBangMessage(MessageTag tag, MessageTag objectId, MillisecondTime time) {
    RNBO_UNUSED(objectId);
    this->updateTime(time);
    this->p_01->processBangMessage(tag, objectId, time);
    this->p_02->processBangMessage(tag, objectId, time);
    this->p_03->processBangMessage(tag, objectId, time);
    this->p_04->processBangMessage(tag, objectId, time);
}

MessageTagInfo resolveTag(MessageTag tag) const {
    switch (tag) {
    case TAG("setup"):
        return "setup";
    case TAG("scope~_obj-98"):
        return "scope~_obj-98";
    case TAG("monitor"):
        return "monitor";
    case TAG("scope~_obj-85"):
        return "scope~_obj-85";
    case TAG("scope~_obj-86"):
        return "scope~_obj-86";
    case TAG("number_obj-41"):
        return "number_obj-41";
    case TAG("number~_obj-32"):
        return "number~_obj-32";
    case TAG("assign"):
        return "assign";
    case TAG("number~_obj-31"):
        return "number~_obj-31";
    case TAG("number_obj-60"):
        return "number_obj-60";
    case TAG("number_obj-64"):
        return "number_obj-64";
    case TAG("number_obj-55"):
        return "number_obj-55";
    case TAG("number_obj-37"):
        return "number_obj-37";
    case TAG("number~_obj-30"):
        return "number~_obj-30";
    case TAG("format"):
        return "format";
    case TAG("sig"):
        return "sig";
    case TAG("mode"):
        return "mode";
    }

    auto subpatchResult_0 = this->p_01->resolveTag(tag);

    if (subpatchResult_0)
        return subpatchResult_0;

    auto subpatchResult_1 = this->p_02->resolveTag(tag);

    if (subpatchResult_1)
        return subpatchResult_1;

    auto subpatchResult_2 = this->p_03->resolveTag(tag);

    if (subpatchResult_2)
        return subpatchResult_2;

    auto subpatchResult_3 = this->p_04->resolveTag(tag);

    if (subpatchResult_3)
        return subpatchResult_3;

    return "";
}

MessageIndex getNumMessages() const {
    return 3;
}

const MessageInfo& getMessageInfo(MessageIndex index) const {
    switch (index) {
    case 0:
        static const MessageInfo r0 = {
            "lfo-mode",
            Inport
        };

        return r0;
    case 1:
        static const MessageInfo r1 = {
            "tap-tempo",
            Inport
        };

        return r1;
    case 2:
        static const MessageInfo r2 = {
            "tap-mult",
            Inport
        };

        return r2;
    }

    return NullMessageInfo;
}

protected:

void param_01_value_set(number v) {
    v = this->param_01_value_constrain(v);
    this->param_01_value = v;
    this->sendParameter(0, false);

    if (this->param_01_value != this->param_01_lastValue) {
        this->getEngine()->presetTouched();
        this->param_01_lastValue = this->param_01_value;
    }

    this->numberobj_11_value_set(v);
}

void param_02_value_set(number v) {
    v = this->param_02_value_constrain(v);
    this->param_02_value = v;
    this->sendParameter(1, false);

    if (this->param_02_value != this->param_02_lastValue) {
        this->getEngine()->presetTouched();
        this->param_02_lastValue = this->param_02_value;
    }

    this->numberobj_13_value_set(v);
}

void param_03_value_set(number v) {
    v = this->param_03_value_constrain(v);
    this->param_03_value = v;
    this->sendParameter(2, false);

    if (this->param_03_value != this->param_03_lastValue) {
        this->getEngine()->presetTouched();
        this->param_03_lastValue = this->param_03_value;
    }

    this->numberobj_15_value_set(v);
}

void numberobj_11_format_set(number v) {
    if (v == 0) {
        this->numberobj_11_currentFormat = 0;
    } else if (v == 1) {
        this->numberobj_11_currentFormat = 1;
    } else if (v == 2) {
        this->numberobj_11_currentFormat = 2;
    } else if (v == 3) {
        this->numberobj_11_currentFormat = 3;
    } else if (v == 4) {
        this->numberobj_11_currentFormat = 4;
    } else if (v == 5) {
        this->numberobj_11_currentFormat = 5;
    } else if (v == 6) {
        this->numberobj_11_currentFormat = 6;
    }
}

void numbertilde_06_sig_number_set(number v) {
    this->numbertilde_06_outValue = v;
}

void numbertilde_06_sig_list_set(const list& v) {
    this->numbertilde_06_outValue = v[0];
}

void numbertilde_06_mode_set(number v) {
    if (v == 1) {
        this->numbertilde_06_currentMode = 0;
    } else if (v == 2) {
        this->numbertilde_06_currentMode = 1;
    }
}

void numbertilde_07_sig_number_set(number v) {
    this->numbertilde_07_outValue = v;
}

void numbertilde_07_sig_list_set(const list& v) {
    this->numbertilde_07_outValue = v[0];
}

void numbertilde_07_mode_set(number v) {
    if (v == 1) {
        this->numbertilde_07_currentMode = 0;
    } else if (v == 2) {
        this->numbertilde_07_currentMode = 1;
    }
}

void numberobj_12_format_set(number v) {
    if (v == 0) {
        this->numberobj_12_currentFormat = 0;
    } else if (v == 1) {
        this->numberobj_12_currentFormat = 1;
    } else if (v == 2) {
        this->numberobj_12_currentFormat = 2;
    } else if (v == 3) {
        this->numberobj_12_currentFormat = 3;
    } else if (v == 4) {
        this->numberobj_12_currentFormat = 4;
    } else if (v == 5) {
        this->numberobj_12_currentFormat = 5;
    } else if (v == 6) {
        this->numberobj_12_currentFormat = 6;
    }
}

void numberobj_13_format_set(number v) {
    if (v == 0) {
        this->numberobj_13_currentFormat = 0;
    } else if (v == 1) {
        this->numberobj_13_currentFormat = 1;
    } else if (v == 2) {
        this->numberobj_13_currentFormat = 2;
    } else if (v == 3) {
        this->numberobj_13_currentFormat = 3;
    } else if (v == 4) {
        this->numberobj_13_currentFormat = 4;
    } else if (v == 5) {
        this->numberobj_13_currentFormat = 5;
    } else if (v == 6) {
        this->numberobj_13_currentFormat = 6;
    }
}

void numberobj_14_format_set(number v) {
    if (v == 0) {
        this->numberobj_14_currentFormat = 0;
    } else if (v == 1) {
        this->numberobj_14_currentFormat = 1;
    } else if (v == 2) {
        this->numberobj_14_currentFormat = 2;
    } else if (v == 3) {
        this->numberobj_14_currentFormat = 3;
    } else if (v == 4) {
        this->numberobj_14_currentFormat = 4;
    } else if (v == 5) {
        this->numberobj_14_currentFormat = 5;
    } else if (v == 6) {
        this->numberobj_14_currentFormat = 6;
    }
}

void numberobj_15_format_set(number v) {
    if (v == 0) {
        this->numberobj_15_currentFormat = 0;
    } else if (v == 1) {
        this->numberobj_15_currentFormat = 1;
    } else if (v == 2) {
        this->numberobj_15_currentFormat = 2;
    } else if (v == 3) {
        this->numberobj_15_currentFormat = 3;
    } else if (v == 4) {
        this->numberobj_15_currentFormat = 4;
    } else if (v == 5) {
        this->numberobj_15_currentFormat = 5;
    } else if (v == 6) {
        this->numberobj_15_currentFormat = 6;
    }
}

void numbertilde_08_sig_number_set(number v) {
    this->numbertilde_08_outValue = v;
}

void numbertilde_08_sig_list_set(const list& v) {
    this->numbertilde_08_outValue = v[0];
}

void numbertilde_08_mode_set(number v) {
    if (v == 1) {
        this->numbertilde_08_currentMode = 0;
    } else if (v == 2) {
        this->numbertilde_08_currentMode = 1;
    }
}

void line_02_target_bang() {}

void line_03_target_bang() {}

void numbertilde_07_value_set(number ) {}

void numbertilde_06_value_set(number ) {}

void numbertilde_08_value_set(number ) {}

number msToSamps(MillisecondTime ms, number sampleRate) {
    return ms * sampleRate * 0.001;
}

MillisecondTime sampsToMs(SampleIndex samps) {
    return samps * (this->invsr * 1000);
}

Index getMaxBlockSize() const {
    return this->maxvs;
}

number getSampleRate() const {
    return this->sr;
}

bool hasFixedVectorSize() const {
    return false;
}

Index getNumInputChannels() const {
    return 3;
}

Index getNumOutputChannels() const {
    return 2;
}

void allocateDataRefs() {
    this->p_01->allocateDataRefs();
    this->p_02->allocateDataRefs();
    this->p_03->allocateDataRefs();
    this->p_04->allocateDataRefs();
    this->delaytilde_01_del_buffer = this->delaytilde_01_del_buffer->allocateIfNeeded();

    if (this->delaytilde_01_del_bufferobj->hasRequestedSize()) {
        if (this->delaytilde_01_del_bufferobj->wantsFill())
            this->zeroDataRef(this->delaytilde_01_del_bufferobj);

        this->getEngine()->sendDataRefUpdated(0);
    }

    this->delaytilde_02_del_buffer = this->delaytilde_02_del_buffer->allocateIfNeeded();

    if (this->delaytilde_02_del_bufferobj->hasRequestedSize()) {
        if (this->delaytilde_02_del_bufferobj->wantsFill())
            this->zeroDataRef(this->delaytilde_02_del_bufferobj);

        this->getEngine()->sendDataRefUpdated(1);
    }
}

void initializeObjects() {
    this->delaytilde_01_del_init();
    this->delaytilde_02_del_init();
    this->numberobj_11_init();
    this->numbertilde_06_init();
    this->numbertilde_07_init();
    this->numberobj_12_init();
    this->numberobj_13_init();
    this->numberobj_14_init();
    this->numberobj_15_init();
    this->numbertilde_08_init();
    this->p_01->initializeObjects();
    this->p_02->initializeObjects();
    this->p_03->initializeObjects();
    this->p_04->initializeObjects();
}

void sendOutlet(OutletIndex index, ParameterValue value) {
    this->getEngine()->sendOutlet(this, index, value);
}

void startup() {
    this->updateTime(this->getEngine()->getCurrentTime());
    this->p_01->startup();
    this->p_02->startup();
    this->p_03->startup();
    this->p_04->startup();

    {
        this->scheduleParamInit(0, 1);
    }

    {
        this->scheduleParamInit(1, 2);
    }

    {
        this->scheduleParamInit(2, 3);
    }

    this->processParamInitEvents();
}

static number param_01_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void p_03_in1_number_set(number v) {
    this->p_03->updateTime(this->_currentTime);
    this->p_03->eventinlet_01_out1_number_set(v);
}

void numberobj_11_output_set(number v) {
    this->p_03_in1_number_set(v);
}

void numberobj_11_value_set(number v) {
    this->numberobj_11_value_setter(v);
    v = this->numberobj_11_value;
    this->sendParameter(3, false);
    this->numberobj_11_output_set(v);
}

static number param_02_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void line_03_segments_set(const list& v) {
    this->line_03_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        SampleIndex currentTime = this->currentsampletime();
        number lastRampValue = this->line_03_currentValue;
        number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

        for (Index i = 0; i < this->line_03_activeRamps->length; i += 3) {
            rampEnd = this->line_03_activeRamps[(Index)(i + 2)];

            if (rampEnd > currentTime) {
                this->line_03_activeRamps[(Index)(i + 2)] = currentTime;
                number diff = rampEnd - currentTime;
                number valueDiff = diff * this->line_03_activeRamps[(Index)(i + 1)];
                lastRampValue = this->line_03_activeRamps[(Index)i] - valueDiff;
                this->line_03_activeRamps[(Index)i] = lastRampValue;
                this->line_03_activeRamps->length = i + 3;
                rampEnd = currentTime;
            } else {
                lastRampValue = this->line_03_activeRamps[(Index)i];
            }
        }

        if (rampEnd < currentTime) {
            this->line_03_activeRamps->push(lastRampValue);
            this->line_03_activeRamps->push(0);
            this->line_03_activeRamps->push(currentTime);
        }

        number lastRampEnd = currentTime;

        for (Index i = 0; i < v->length; i += 2) {
            number destinationValue = v[(Index)i];
            number inc = 0;
            number rampTimeInSamples;

            if (v->length > i + 1) {
                rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);
            } else {
                rampTimeInSamples = this->mstosamps(this->line_03_time);
            }

            if (rampTimeInSamples <= 0)
                rampTimeInSamples = 1;

            inc = (destinationValue - lastRampValue) / rampTimeInSamples;
            lastRampEnd += rampTimeInSamples;
            this->line_03_activeRamps->push(destinationValue);
            this->line_03_activeRamps->push(inc);
            this->line_03_activeRamps->push(lastRampEnd);
            lastRampValue = destinationValue;
        }
    }
}

void numberobj_12_output_set(number v) {
    {
        list converted = {v};
        this->line_03_segments_set(converted);
    }
}

void numberobj_12_value_set(number v) {
    this->numberobj_12_value_setter(v);
    v = this->numberobj_12_value;
    this->sendParameter(4, false);
    this->numberobj_12_output_set(v);
}

void scale_03_out_set(const list& v) {
    this->scale_03_out = jsCreateListCopy(v);

    {
        number converted = (v->length > 0 ? v[0] : 0);
        this->numberobj_12_value_set(converted);
    }
}

void scale_03_input_set(const list& v) {
    this->scale_03_input = jsCreateListCopy(v);
    list tmp = {};

    for (Index i = 0; i < v->length; i++) {
        tmp->push(this->scale(
            v[(Index)i],
            this->scale_03_inlow,
            this->scale_03_inhigh,
            this->scale_03_outlow,
            this->scale_03_outhigh,
            this->scale_03_power
        ));
    }

    this->scale_03_out_set(tmp);
}

void numberobj_13_output_set(number v) {
    {
        list converted = {v};
        this->scale_03_input_set(converted);
    }
}

void numberobj_13_value_set(number v) {
    this->numberobj_13_value_setter(v);
    v = this->numberobj_13_value;
    this->sendParameter(5, false);
    this->numberobj_13_output_set(v);
}

static number param_03_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void line_02_segments_set(const list& v) {
    this->line_02_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        SampleIndex currentTime = this->currentsampletime();
        number lastRampValue = this->line_02_currentValue;
        number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

        for (Index i = 0; i < this->line_02_activeRamps->length; i += 3) {
            rampEnd = this->line_02_activeRamps[(Index)(i + 2)];

            if (rampEnd > currentTime) {
                this->line_02_activeRamps[(Index)(i + 2)] = currentTime;
                number diff = rampEnd - currentTime;
                number valueDiff = diff * this->line_02_activeRamps[(Index)(i + 1)];
                lastRampValue = this->line_02_activeRamps[(Index)i] - valueDiff;
                this->line_02_activeRamps[(Index)i] = lastRampValue;
                this->line_02_activeRamps->length = i + 3;
                rampEnd = currentTime;
            } else {
                lastRampValue = this->line_02_activeRamps[(Index)i];
            }
        }

        if (rampEnd < currentTime) {
            this->line_02_activeRamps->push(lastRampValue);
            this->line_02_activeRamps->push(0);
            this->line_02_activeRamps->push(currentTime);
        }

        number lastRampEnd = currentTime;

        for (Index i = 0; i < v->length; i += 2) {
            number destinationValue = v[(Index)i];
            number inc = 0;
            number rampTimeInSamples;

            if (v->length > i + 1) {
                rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);
            } else {
                rampTimeInSamples = this->mstosamps(this->line_02_time);
            }

            if (rampTimeInSamples <= 0)
                rampTimeInSamples = 1;

            inc = (destinationValue - lastRampValue) / rampTimeInSamples;
            lastRampEnd += rampTimeInSamples;
            this->line_02_activeRamps->push(destinationValue);
            this->line_02_activeRamps->push(inc);
            this->line_02_activeRamps->push(lastRampEnd);
            lastRampValue = destinationValue;
        }
    }
}

void numberobj_14_output_set(number v) {
    {
        list converted = {v};
        this->line_02_segments_set(converted);
    }
}

void numberobj_14_value_set(number v) {
    this->numberobj_14_value_setter(v);
    v = this->numberobj_14_value;
    this->sendParameter(6, false);
    this->numberobj_14_output_set(v);
}

void scale_04_out_set(const list& v) {
    this->scale_04_out = jsCreateListCopy(v);

    {
        number converted = (v->length > 0 ? v[0] : 0);
        this->numberobj_14_value_set(converted);
    }
}

void scale_04_input_set(const list& v) {
    this->scale_04_input = jsCreateListCopy(v);
    list tmp = {};

    for (Index i = 0; i < v->length; i++) {
        tmp->push(this->scale(
            v[(Index)i],
            this->scale_04_inlow,
            this->scale_04_inhigh,
            this->scale_04_outlow,
            this->scale_04_outhigh,
            this->scale_04_power
        ));
    }

    this->scale_04_out_set(tmp);
}

void numberobj_15_output_set(number v) {
    {
        list converted = {v};
        this->scale_04_input_set(converted);
    }
}

void numberobj_15_value_set(number v) {
    this->numberobj_15_value_setter(v);
    v = this->numberobj_15_value;
    this->sendParameter(7, false);
    this->numberobj_15_output_set(v);
}

void ctlin_01_outchannel_set(number ) {}

void ctlin_01_outcontroller_set(number ) {}

void fromnormalized_01_output_set(number v) {
    this->param_01_value_set(v);
}

void fromnormalized_01_input_set(number v) {
    this->fromnormalized_01_output_set(this->fromnormalized(0, v));
}

void expr_05_out1_set(number v) {
    this->expr_05_out1 = v;
    this->fromnormalized_01_input_set(this->expr_05_out1);
}

void expr_05_in1_set(number in1) {
    this->expr_05_in1 = in1;
    this->expr_05_out1_set(this->expr_05_in1 * this->expr_05_in2);//#map:expr_05:1
}

void ctlin_01_value_set(number v) {
    this->expr_05_in1_set(v);
}

void ctlin_01_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(length);
    RNBO_UNUSED(port);

    if (status == 0xB0 && (channel == this->ctlin_01_channel || this->ctlin_01_channel == -1) && (data[1] == this->ctlin_01_controller || this->ctlin_01_controller == -1)) {
        this->ctlin_01_outchannel_set(channel);
        this->ctlin_01_outcontroller_set(data[1]);
        this->ctlin_01_value_set(data[2]);
        this->ctlin_01_status = 0;
    }
}

void ctlin_02_outchannel_set(number ) {}

void ctlin_02_outcontroller_set(number ) {}

void fromnormalized_02_output_set(number v) {
    this->param_02_value_set(v);
}

void fromnormalized_02_input_set(number v) {
    this->fromnormalized_02_output_set(this->fromnormalized(1, v));
}

void expr_06_out1_set(number v) {
    this->expr_06_out1 = v;
    this->fromnormalized_02_input_set(this->expr_06_out1);
}

void expr_06_in1_set(number in1) {
    this->expr_06_in1 = in1;
    this->expr_06_out1_set(this->expr_06_in1 * this->expr_06_in2);//#map:expr_06:1
}

void ctlin_02_value_set(number v) {
    this->expr_06_in1_set(v);
}

void ctlin_02_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(length);
    RNBO_UNUSED(port);

    if (status == 0xB0 && (channel == this->ctlin_02_channel || this->ctlin_02_channel == -1) && (data[1] == this->ctlin_02_controller || this->ctlin_02_controller == -1)) {
        this->ctlin_02_outchannel_set(channel);
        this->ctlin_02_outcontroller_set(data[1]);
        this->ctlin_02_value_set(data[2]);
        this->ctlin_02_status = 0;
    }
}

void ctlin_03_outchannel_set(number ) {}

void ctlin_03_outcontroller_set(number ) {}

void fromnormalized_03_output_set(number v) {
    this->param_03_value_set(v);
}

void fromnormalized_03_input_set(number v) {
    this->fromnormalized_03_output_set(this->fromnormalized(2, v));
}

void expr_07_out1_set(number v) {
    this->expr_07_out1 = v;
    this->fromnormalized_03_input_set(this->expr_07_out1);
}

void expr_07_in1_set(number in1) {
    this->expr_07_in1 = in1;
    this->expr_07_out1_set(this->expr_07_in1 * this->expr_07_in2);//#map:expr_07:1
}

void ctlin_03_value_set(number v) {
    this->expr_07_in1_set(v);
}

void ctlin_03_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(length);
    RNBO_UNUSED(port);

    if (status == 0xB0 && (channel == this->ctlin_03_channel || this->ctlin_03_channel == -1) && (data[1] == this->ctlin_03_controller || this->ctlin_03_controller == -1)) {
        this->ctlin_03_outchannel_set(channel);
        this->ctlin_03_outcontroller_set(data[1]);
        this->ctlin_03_value_set(data[2]);
        this->ctlin_03_status = 0;
    }
}

void p_04_out1_number_set(number v) {
    this->param_01_value_set(v);
}

void p_03_in2_list_set(const list& v) {
    this->p_03->updateTime(this->_currentTime);
    this->p_03->eventinlet_02_out1_list_set(v);
}

void p_04_out2_list_set(const list& v) {
    this->p_03_in2_list_set(v);
}

void p_04_perform(Index n) {
    // subpatcher: TapTempo
    this->p_04->process(nullptr, 0, nullptr, 0, n);
}

void p_03_perform(Sample * in3, Sample * out1, Index n) {
    // subpatcher: LFO~
    SampleArray<1> ins = {in3};

    SampleArray<1> outs = {out1};
    this->p_03->process(ins, 1, outs, 1, n);
}

void scopetilde_03_perform(const Sample * x, const Sample * y, Index n) {
    auto __scopetilde_03_ysign = this->scopetilde_03_ysign;
    auto __scopetilde_03_ymonitorvalue = this->scopetilde_03_ymonitorvalue;
    auto __scopetilde_03_xsign = this->scopetilde_03_xsign;
    auto __scopetilde_03_xmonitorvalue = this->scopetilde_03_xmonitorvalue;
    auto __scopetilde_03_mode = this->scopetilde_03_mode;

    for (Index i = 0; i < n; i++) {
        number xval = x[(Index)i];
        number yval = y[(Index)i];

        if (__scopetilde_03_mode == 1) {
            number xabsval = rnbo_abs(xval);

            if (xabsval > __scopetilde_03_xmonitorvalue) {
                __scopetilde_03_xmonitorvalue = xabsval;
                __scopetilde_03_xsign = (xval < 0 ? -1 : 1);
            }

            number yabsval = rnbo_abs(yval);

            if (yabsval > __scopetilde_03_ymonitorvalue) {
                __scopetilde_03_ymonitorvalue = yabsval;
                __scopetilde_03_ysign = (yval < 0 ? -1 : 1);
            }
        } else {
            __scopetilde_03_xmonitorvalue = xval;
            __scopetilde_03_xsign = 1;
            __scopetilde_03_ymonitorvalue = yval;
            __scopetilde_03_ysign = 1;
        }

        this->scopetilde_03_effectiveCount--;

        if (this->scopetilde_03_effectiveCount <= 0) {
            this->scopetilde_03_updateEffectiveCount();
            this->scopetilde_03_monitorbuffer->push(__scopetilde_03_xmonitorvalue * __scopetilde_03_xsign);

            if (__scopetilde_03_mode == 1)
                __scopetilde_03_xmonitorvalue = 0;

            if (this->scopetilde_03_monitorbuffer->length >= 128 * (1 + 0)) {
                this->getEngine()->sendListMessage(
                    TAG("monitor"),
                    TAG("scope~_obj-86"),
                    this->scopetilde_03_monitorbuffer,
                    this->_currentTime
                );;

                this->scopetilde_03_monitorbuffer->length = 0;
            }
        }
    }

    this->scopetilde_03_xmonitorvalue = __scopetilde_03_xmonitorvalue;
    this->scopetilde_03_xsign = __scopetilde_03_xsign;
    this->scopetilde_03_ymonitorvalue = __scopetilde_03_ymonitorvalue;
    this->scopetilde_03_ysign = __scopetilde_03_ysign;
}

void triangle_tilde_01_perform(const Sample * phase, number duty, Sample * out1, Index n) {
    RNBO_UNUSED(duty);
    Index i;

    for (i = 0; i < n; i++) {
        number p1 = 0.5;
        number wrappedPhase = this->wrap(phase[(Index)i], 0., 1.);
        p1 = (p1 > 1. ? 1. : (p1 < 0. ? 0. : p1));

        if (wrappedPhase < p1) {
            out1[(Index)i] = wrappedPhase / p1;
            continue;
        } else {
            out1[(Index)i] = (p1 == 1. ? wrappedPhase : 1. - (wrappedPhase - p1) / (1. - p1));
            continue;
        }
    }
}

void scopetilde_02_perform(const Sample * x, const Sample * y, Index n) {
    auto __scopetilde_02_ysign = this->scopetilde_02_ysign;
    auto __scopetilde_02_ymonitorvalue = this->scopetilde_02_ymonitorvalue;
    auto __scopetilde_02_xsign = this->scopetilde_02_xsign;
    auto __scopetilde_02_xmonitorvalue = this->scopetilde_02_xmonitorvalue;
    auto __scopetilde_02_mode = this->scopetilde_02_mode;

    for (Index i = 0; i < n; i++) {
        number xval = x[(Index)i];
        number yval = y[(Index)i];

        if (__scopetilde_02_mode == 1) {
            number xabsval = rnbo_abs(xval);

            if (xabsval > __scopetilde_02_xmonitorvalue) {
                __scopetilde_02_xmonitorvalue = xabsval;
                __scopetilde_02_xsign = (xval < 0 ? -1 : 1);
            }

            number yabsval = rnbo_abs(yval);

            if (yabsval > __scopetilde_02_ymonitorvalue) {
                __scopetilde_02_ymonitorvalue = yabsval;
                __scopetilde_02_ysign = (yval < 0 ? -1 : 1);
            }
        } else {
            __scopetilde_02_xmonitorvalue = xval;
            __scopetilde_02_xsign = 1;
            __scopetilde_02_ymonitorvalue = yval;
            __scopetilde_02_ysign = 1;
        }

        this->scopetilde_02_effectiveCount--;

        if (this->scopetilde_02_effectiveCount <= 0) {
            this->scopetilde_02_updateEffectiveCount();
            this->scopetilde_02_monitorbuffer->push(__scopetilde_02_xmonitorvalue * __scopetilde_02_xsign);

            if (__scopetilde_02_mode == 1)
                __scopetilde_02_xmonitorvalue = 0;

            if (this->scopetilde_02_monitorbuffer->length >= 128 * (1 + 0)) {
                this->getEngine()->sendListMessage(
                    TAG("monitor"),
                    TAG("scope~_obj-85"),
                    this->scopetilde_02_monitorbuffer,
                    this->_currentTime
                );;

                this->scopetilde_02_monitorbuffer->length = 0;
            }
        }
    }

    this->scopetilde_02_xmonitorvalue = __scopetilde_02_xmonitorvalue;
    this->scopetilde_02_xsign = __scopetilde_02_xsign;
    this->scopetilde_02_ymonitorvalue = __scopetilde_02_ymonitorvalue;
    this->scopetilde_02_ysign = __scopetilde_02_ysign;
}

void line_02_perform(Sample * out, Index n) {
    auto __line_02_currentValue = this->line_02_currentValue;
    Index i = 0;

    if ((bool)(this->line_02_activeRamps->length)) {
        while ((bool)(this->line_02_activeRamps->length) && i < n) {
            number destinationValue = this->line_02_activeRamps[0];
            number inc = this->line_02_activeRamps[1];
            number rampTimeInSamples = this->line_02_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __line_02_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->line_02_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->line_02_activeRamps->length))) this->getEngine()->scheduleClockEventWithValue(
                    this,
                    760652352,
                    this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                    0
                );;
            }

            __line_02_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __line_02_currentValue;
        i++;
    }

    this->line_02_currentValue = __line_02_currentValue;
}

void dspexpr_11_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void triangle_tilde_02_perform(const Sample * phase, number duty, Sample * out1, Index n) {
    RNBO_UNUSED(duty);
    Index i;

    for (i = 0; i < n; i++) {
        number p1 = 0.5;
        number wrappedPhase = this->wrap(phase[(Index)i], 0., 1.);
        p1 = (p1 > 1. ? 1. : (p1 < 0. ? 0. : p1));

        if (wrappedPhase < p1) {
            out1[(Index)i] = wrappedPhase / p1;
            continue;
        } else {
            out1[(Index)i] = (p1 == 1. ? wrappedPhase : 1. - (wrappedPhase - p1) / (1. - p1));
            continue;
        }
    }
}

void scopetilde_01_perform(const Sample * x, const Sample * y, Index n) {
    auto __scopetilde_01_ysign = this->scopetilde_01_ysign;
    auto __scopetilde_01_ymonitorvalue = this->scopetilde_01_ymonitorvalue;
    auto __scopetilde_01_xsign = this->scopetilde_01_xsign;
    auto __scopetilde_01_xmonitorvalue = this->scopetilde_01_xmonitorvalue;
    auto __scopetilde_01_mode = this->scopetilde_01_mode;

    for (Index i = 0; i < n; i++) {
        number xval = x[(Index)i];
        number yval = y[(Index)i];

        if (__scopetilde_01_mode == 1) {
            number xabsval = rnbo_abs(xval);

            if (xabsval > __scopetilde_01_xmonitorvalue) {
                __scopetilde_01_xmonitorvalue = xabsval;
                __scopetilde_01_xsign = (xval < 0 ? -1 : 1);
            }

            number yabsval = rnbo_abs(yval);

            if (yabsval > __scopetilde_01_ymonitorvalue) {
                __scopetilde_01_ymonitorvalue = yabsval;
                __scopetilde_01_ysign = (yval < 0 ? -1 : 1);
            }
        } else {
            __scopetilde_01_xmonitorvalue = xval;
            __scopetilde_01_xsign = 1;
            __scopetilde_01_ymonitorvalue = yval;
            __scopetilde_01_ysign = 1;
        }

        this->scopetilde_01_effectiveCount--;

        if (this->scopetilde_01_effectiveCount <= 0) {
            this->scopetilde_01_updateEffectiveCount();
            this->scopetilde_01_monitorbuffer->push(__scopetilde_01_xmonitorvalue * __scopetilde_01_xsign);

            if (__scopetilde_01_mode == 1)
                __scopetilde_01_xmonitorvalue = 0;

            if (this->scopetilde_01_monitorbuffer->length >= 128 * (1 + 0)) {
                this->getEngine()->sendListMessage(
                    TAG("monitor"),
                    TAG("scope~_obj-98"),
                    this->scopetilde_01_monitorbuffer,
                    this->_currentTime
                );;

                this->scopetilde_01_monitorbuffer->length = 0;
            }
        }
    }

    this->scopetilde_01_xmonitorvalue = __scopetilde_01_xmonitorvalue;
    this->scopetilde_01_xsign = __scopetilde_01_xsign;
    this->scopetilde_01_ymonitorvalue = __scopetilde_01_ymonitorvalue;
    this->scopetilde_01_ysign = __scopetilde_01_ysign;
}

void line_03_perform(Sample * out, Index n) {
    auto __line_03_currentValue = this->line_03_currentValue;
    Index i = 0;

    if ((bool)(this->line_03_activeRamps->length)) {
        while ((bool)(this->line_03_activeRamps->length) && i < n) {
            number destinationValue = this->line_03_activeRamps[0];
            number inc = this->line_03_activeRamps[1];
            number rampTimeInSamples = this->line_03_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __line_03_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->line_03_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->line_03_activeRamps->length))) this->getEngine()->scheduleClockEventWithValue(
                    this,
                    -798469121,
                    this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                    0
                );;
            }

            __line_03_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __line_03_currentValue;
        i++;
    }

    this->line_03_currentValue = __line_03_currentValue;
}

void dspexpr_10_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void slide_tilde_02_perform(const Sample * x, number up, number down, Sample * out1, Index n) {
    RNBO_UNUSED(down);
    RNBO_UNUSED(up);
    auto __slide_tilde_02_prev = this->slide_tilde_02_prev;
    number iup = this->safediv(1., this->maximum(1., rnbo_abs(1000)));
    number idown = this->safediv(1., this->maximum(1., rnbo_abs(1000)));
    Index i;

    for (i = 0; i < n; i++) {
        number temp = x[(Index)i] - __slide_tilde_02_prev;
        __slide_tilde_02_prev = __slide_tilde_02_prev + ((x[(Index)i] > __slide_tilde_02_prev ? iup : idown)) * temp;
        out1[(Index)i] = __slide_tilde_02_prev;
    }

    this->slide_tilde_02_prev = __slide_tilde_02_prev;
}

void dspexpr_09_perform(const Sample * in1, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i];//#map:_###_obj_###_:1
    }
}

void mstosamps_tilde_02_perform(const Sample * ms, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = ms[(Index)i] * this->sr * 0.001;
    }
}

void numbertilde_07_perform(const Sample * input_signal, Sample * output, Index n) {
    auto __numbertilde_07_currentIntervalInSamples = this->numbertilde_07_currentIntervalInSamples;
    auto __numbertilde_07_lastValue = this->numbertilde_07_lastValue;
    auto __numbertilde_07_currentInterval = this->numbertilde_07_currentInterval;
    auto __numbertilde_07_rampInSamples = this->numbertilde_07_rampInSamples;
    auto __numbertilde_07_outValue = this->numbertilde_07_outValue;
    auto __numbertilde_07_currentMode = this->numbertilde_07_currentMode;
    number monitorvalue = 0;

    for (Index i = 0; i < n; i++) {
        monitorvalue += input_signal[(Index)i];

        if (__numbertilde_07_currentMode == 0) {
            output[(Index)i] = this->numbertilde_07_smooth_next(
                __numbertilde_07_outValue,
                __numbertilde_07_rampInSamples,
                __numbertilde_07_rampInSamples
            );
        } else {
            output[(Index)i] = input_signal[(Index)i];
        }
    }

    __numbertilde_07_currentInterval -= n;
    monitorvalue /= n;

    if (monitorvalue != __numbertilde_07_lastValue && __numbertilde_07_currentInterval <= 0) {
        __numbertilde_07_currentInterval = __numbertilde_07_currentIntervalInSamples;

        this->getEngine()->scheduleClockEventWithValue(
            this,
            2098551528,
            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
            monitorvalue
        );;

        __numbertilde_07_lastValue = monitorvalue;
        this->getEngine()->sendListMessage(TAG("monitor"), TAG("number~_obj-31"), {monitorvalue}, this->_currentTime);;
    }

    this->numbertilde_07_currentInterval = __numbertilde_07_currentInterval;
    this->numbertilde_07_lastValue = __numbertilde_07_lastValue;
}

void dspexpr_08_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void slide_tilde_01_perform(const Sample * x, number up, number down, Sample * out1, Index n) {
    RNBO_UNUSED(down);
    RNBO_UNUSED(up);
    auto __slide_tilde_01_prev = this->slide_tilde_01_prev;
    number iup = this->safediv(1., this->maximum(1., rnbo_abs(1000)));
    number idown = this->safediv(1., this->maximum(1., rnbo_abs(1000)));
    Index i;

    for (i = 0; i < n; i++) {
        number temp = x[(Index)i] - __slide_tilde_01_prev;
        __slide_tilde_01_prev = __slide_tilde_01_prev + ((x[(Index)i] > __slide_tilde_01_prev ? iup : idown)) * temp;
        out1[(Index)i] = __slide_tilde_01_prev;
    }

    this->slide_tilde_01_prev = __slide_tilde_01_prev;
}

void dspexpr_07_perform(const Sample * in1, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i];//#map:_###_obj_###_:1
    }
}

void mstosamps_tilde_01_perform(const Sample * ms, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = ms[(Index)i] * this->sr * 0.001;
    }
}

void numbertilde_06_perform(const Sample * input_signal, Sample * output, Index n) {
    auto __numbertilde_06_currentIntervalInSamples = this->numbertilde_06_currentIntervalInSamples;
    auto __numbertilde_06_lastValue = this->numbertilde_06_lastValue;
    auto __numbertilde_06_currentInterval = this->numbertilde_06_currentInterval;
    auto __numbertilde_06_rampInSamples = this->numbertilde_06_rampInSamples;
    auto __numbertilde_06_outValue = this->numbertilde_06_outValue;
    auto __numbertilde_06_currentMode = this->numbertilde_06_currentMode;
    number monitorvalue = 0;

    for (Index i = 0; i < n; i++) {
        monitorvalue += input_signal[(Index)i];

        if (__numbertilde_06_currentMode == 0) {
            output[(Index)i] = this->numbertilde_06_smooth_next(
                __numbertilde_06_outValue,
                __numbertilde_06_rampInSamples,
                __numbertilde_06_rampInSamples
            );
        } else {
            output[(Index)i] = input_signal[(Index)i];
        }
    }

    __numbertilde_06_currentInterval -= n;
    monitorvalue /= n;

    if (monitorvalue != __numbertilde_06_lastValue && __numbertilde_06_currentInterval <= 0) {
        __numbertilde_06_currentInterval = __numbertilde_06_currentIntervalInSamples;

        this->getEngine()->scheduleClockEventWithValue(
            this,
            -1494586265,
            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
            monitorvalue
        );;

        __numbertilde_06_lastValue = monitorvalue;
        this->getEngine()->sendListMessage(TAG("monitor"), TAG("number~_obj-32"), {monitorvalue}, this->_currentTime);;
    }

    this->numbertilde_06_currentInterval = __numbertilde_06_currentInterval;
    this->numbertilde_06_lastValue = __numbertilde_06_lastValue;
}

void scale_tilde_05_perform(
    const Sample * x,
    number lowin,
    number hiin,
    number lowout,
    number highout,
    number pow,
    Sample * out1,
    Index n
) {
    RNBO_UNUSED(pow);
    RNBO_UNUSED(highout);
    RNBO_UNUSED(lowout);
    RNBO_UNUSED(hiin);
    RNBO_UNUSED(lowin);
    number inscale = this->safediv(1., 5 - 0);
    number outdiff = 1 - 0.7;
    Index i;

    for (i = 0; i < n; i++) {
        number value = (x[(Index)i] - 0) * inscale;
        value = value * outdiff + 0.7;
        out1[(Index)i] = value;
    }
}

void numbertilde_08_perform(const Sample * input_signal, Sample * output, Index n) {
    auto __numbertilde_08_currentIntervalInSamples = this->numbertilde_08_currentIntervalInSamples;
    auto __numbertilde_08_lastValue = this->numbertilde_08_lastValue;
    auto __numbertilde_08_currentInterval = this->numbertilde_08_currentInterval;
    auto __numbertilde_08_rampInSamples = this->numbertilde_08_rampInSamples;
    auto __numbertilde_08_outValue = this->numbertilde_08_outValue;
    auto __numbertilde_08_currentMode = this->numbertilde_08_currentMode;
    number monitorvalue = 0;

    for (Index i = 0; i < n; i++) {
        monitorvalue += input_signal[(Index)i];

        if (__numbertilde_08_currentMode == 0) {
            output[(Index)i] = this->numbertilde_08_smooth_next(
                __numbertilde_08_outValue,
                __numbertilde_08_rampInSamples,
                __numbertilde_08_rampInSamples
            );
        } else {
            output[(Index)i] = input_signal[(Index)i];
        }
    }

    __numbertilde_08_currentInterval -= n;
    monitorvalue /= n;

    if (monitorvalue != __numbertilde_08_lastValue && __numbertilde_08_currentInterval <= 0) {
        __numbertilde_08_currentInterval = __numbertilde_08_currentIntervalInSamples;

        this->getEngine()->scheduleClockEventWithValue(
            this,
            1396722025,
            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
            monitorvalue
        );;

        __numbertilde_08_lastValue = monitorvalue;
        this->getEngine()->sendListMessage(TAG("monitor"), TAG("number~_obj-30"), {monitorvalue}, this->_currentTime);;
    }

    this->numbertilde_08_currentInterval = __numbertilde_08_currentInterval;
    this->numbertilde_08_lastValue = __numbertilde_08_lastValue;
}

void dspexpr_12_perform(const Sample * in1, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i];//#map:_###_obj_###_:1
    }
}

void p_02_perform(
    Sample * in1,
    Sample * in2,
    Sample * in3,
    Sample * in4,
    Sample * in5,
    Sample * out1,
    Sample * out2,
    Index n
) {
    // subpatcher: HiCut~
    SampleArray<5> ins = {in1, in2, in3, in4, in5};

    SampleArray<2> outs = {out1, out2};
    this->p_02->process(ins, 5, outs, 2, n);
}

void p_01_perform(
    Sample * in1,
    Sample * in2,
    Sample * in3,
    Sample * in4,
    Sample * in5,
    Sample * out1,
    Sample * out2,
    Index n
) {
    // subpatcher: LowCut~
    SampleArray<5> ins = {in1, in2, in3, in4, in5};

    SampleArray<2> outs = {out1, out2};
    this->p_01->process(ins, 5, outs, 2, n);
}

void delaytilde_01_perform(const Sample * delay, const Sample * input, Sample * output, Index n) {
    auto __delaytilde_01_crossfadeDelay = this->delaytilde_01_crossfadeDelay;
    auto __delaytilde_01_rampInSamples = this->delaytilde_01_rampInSamples;
    auto __delaytilde_01_ramp = this->delaytilde_01_ramp;
    auto __delaytilde_01_lastDelay = this->delaytilde_01_lastDelay;

    for (Index i = 0; i < n; i++) {
        if (__delaytilde_01_lastDelay == -1) {
            __delaytilde_01_lastDelay = delay[(Index)i];
        }

        if (__delaytilde_01_ramp > 0) {
            number factor = __delaytilde_01_ramp / __delaytilde_01_rampInSamples;
            output[(Index)i] = this->delaytilde_01_del_read(__delaytilde_01_crossfadeDelay, 0) * factor + this->delaytilde_01_del_read(__delaytilde_01_lastDelay, 0) * (1. - factor);
            __delaytilde_01_ramp--;
        } else {
            number effectiveDelay = delay[(Index)i];

            if (effectiveDelay != __delaytilde_01_lastDelay) {
                __delaytilde_01_ramp = __delaytilde_01_rampInSamples;
                __delaytilde_01_crossfadeDelay = __delaytilde_01_lastDelay;
                __delaytilde_01_lastDelay = effectiveDelay;
                output[(Index)i] = this->delaytilde_01_del_read(__delaytilde_01_crossfadeDelay, 0);
                __delaytilde_01_ramp--;
            } else {
                output[(Index)i] = this->delaytilde_01_del_read(effectiveDelay, 0);
            }
        }

        this->delaytilde_01_del_write(input[(Index)i]);
        this->delaytilde_01_del_step();
    }

    this->delaytilde_01_lastDelay = __delaytilde_01_lastDelay;
    this->delaytilde_01_ramp = __delaytilde_01_ramp;
    this->delaytilde_01_crossfadeDelay = __delaytilde_01_crossfadeDelay;
}

void dspexpr_04_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void dspexpr_03_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void delaytilde_02_perform(const Sample * delay, const Sample * input, Sample * output, Index n) {
    auto __delaytilde_02_crossfadeDelay = this->delaytilde_02_crossfadeDelay;
    auto __delaytilde_02_rampInSamples = this->delaytilde_02_rampInSamples;
    auto __delaytilde_02_ramp = this->delaytilde_02_ramp;
    auto __delaytilde_02_lastDelay = this->delaytilde_02_lastDelay;

    for (Index i = 0; i < n; i++) {
        if (__delaytilde_02_lastDelay == -1) {
            __delaytilde_02_lastDelay = delay[(Index)i];
        }

        if (__delaytilde_02_ramp > 0) {
            number factor = __delaytilde_02_ramp / __delaytilde_02_rampInSamples;
            output[(Index)i] = this->delaytilde_02_del_read(__delaytilde_02_crossfadeDelay, 0) * factor + this->delaytilde_02_del_read(__delaytilde_02_lastDelay, 0) * (1. - factor);
            __delaytilde_02_ramp--;
        } else {
            number effectiveDelay = delay[(Index)i];

            if (effectiveDelay != __delaytilde_02_lastDelay) {
                __delaytilde_02_ramp = __delaytilde_02_rampInSamples;
                __delaytilde_02_crossfadeDelay = __delaytilde_02_lastDelay;
                __delaytilde_02_lastDelay = effectiveDelay;
                output[(Index)i] = this->delaytilde_02_del_read(__delaytilde_02_crossfadeDelay, 0);
                __delaytilde_02_ramp--;
            } else {
                output[(Index)i] = this->delaytilde_02_del_read(effectiveDelay, 0);
            }
        }

        this->delaytilde_02_del_write(input[(Index)i]);
        this->delaytilde_02_del_step();
    }

    this->delaytilde_02_lastDelay = __delaytilde_02_lastDelay;
    this->delaytilde_02_ramp = __delaytilde_02_ramp;
    this->delaytilde_02_crossfadeDelay = __delaytilde_02_crossfadeDelay;
}

void dspexpr_06_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void dspexpr_05_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void stackprotect_perform(Index n) {
    RNBO_UNUSED(n);
    auto __stackprotect_count = this->stackprotect_count;
    __stackprotect_count = 0;
    this->stackprotect_count = __stackprotect_count;
}

void numberobj_11_value_setter(number v) {
    number localvalue = v;

    {
        if (localvalue < 0)
            localvalue = 0;
    }

    {
        if (localvalue > 100)
            localvalue = 100;
    }

    if (this->numberobj_11_currentFormat != 6) {
        localvalue = rnbo_trunc(localvalue);
    }

    this->numberobj_11_value = localvalue;
}

void numberobj_12_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_12_currentFormat != 6) {
        localvalue = rnbo_trunc(localvalue);
    }

    this->numberobj_12_value = localvalue;
}

void numberobj_13_value_setter(number v) {
    number localvalue = v;

    {
        if (localvalue < 0)
            localvalue = 0;
    }

    {
        if (localvalue > 100)
            localvalue = 100;
    }

    if (this->numberobj_13_currentFormat != 6) {
        localvalue = rnbo_trunc(localvalue);
    }

    this->numberobj_13_value = localvalue;
}

void numberobj_14_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_14_currentFormat != 6) {
        localvalue = rnbo_trunc(localvalue);
    }

    this->numberobj_14_value = localvalue;
}

void numberobj_15_value_setter(number v) {
    number localvalue = v;

    {
        if (localvalue < 0)
            localvalue = 0;
    }

    {
        if (localvalue > 100)
            localvalue = 100;
    }

    if (this->numberobj_15_currentFormat != 6) {
        localvalue = rnbo_trunc(localvalue);
    }

    this->numberobj_15_value = localvalue;
}

void delaytilde_01_del_step() {
    this->delaytilde_01_del_reader++;

    if (this->delaytilde_01_del_reader >= (int)(this->delaytilde_01_del_buffer->getSize()))
        this->delaytilde_01_del_reader = 0;
}

number delaytilde_01_del_read(number size, Int interp) {
    if (interp == 0) {
        number r = (int)(this->delaytilde_01_del_buffer->getSize()) + this->delaytilde_01_del_reader - ((size > this->delaytilde_01_del__maxdelay ? this->delaytilde_01_del__maxdelay : (size < (this->delaytilde_01_del_reader != this->delaytilde_01_del_writer) ? this->delaytilde_01_del_reader != this->delaytilde_01_del_writer : size)));
        long index1 = (long)(rnbo_floor(r));
        number frac = r - index1;
        long index2 = (long)(index1 + 1);

        return this->linearinterp(frac, this->delaytilde_01_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_01_del_wrap))
        ), this->delaytilde_01_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_01_del_wrap))
        ));
    } else if (interp == 1) {
        number r = (int)(this->delaytilde_01_del_buffer->getSize()) + this->delaytilde_01_del_reader - ((size > this->delaytilde_01_del__maxdelay ? this->delaytilde_01_del__maxdelay : (size < (1 + this->delaytilde_01_del_reader != this->delaytilde_01_del_writer) ? 1 + this->delaytilde_01_del_reader != this->delaytilde_01_del_writer : size)));
        long index1 = (long)(rnbo_floor(r));
        number frac = r - index1;
        Index index2 = (Index)(index1 + 1);
        Index index3 = (Index)(index2 + 1);
        Index index4 = (Index)(index3 + 1);

        return this->cubicinterp(frac, this->delaytilde_01_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_01_del_wrap))
        ), this->delaytilde_01_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_01_del_wrap))
        ), this->delaytilde_01_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index3 & (UBinOpInt)this->delaytilde_01_del_wrap))
        ), this->delaytilde_01_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index4 & (UBinOpInt)this->delaytilde_01_del_wrap))
        ));
    } else if (interp == 2) {
        number r = (int)(this->delaytilde_01_del_buffer->getSize()) + this->delaytilde_01_del_reader - ((size > this->delaytilde_01_del__maxdelay ? this->delaytilde_01_del__maxdelay : (size < (1 + this->delaytilde_01_del_reader != this->delaytilde_01_del_writer) ? 1 + this->delaytilde_01_del_reader != this->delaytilde_01_del_writer : size)));
        long index1 = (long)(rnbo_floor(r));
        number frac = r - index1;
        Index index2 = (Index)(index1 + 1);
        Index index3 = (Index)(index2 + 1);
        Index index4 = (Index)(index3 + 1);

        return this->splineinterp(frac, this->delaytilde_01_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_01_del_wrap))
        ), this->delaytilde_01_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_01_del_wrap))
        ), this->delaytilde_01_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index3 & (UBinOpInt)this->delaytilde_01_del_wrap))
        ), this->delaytilde_01_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index4 & (UBinOpInt)this->delaytilde_01_del_wrap))
        ));
    } else if (interp == 3) {
        number r = (int)(this->delaytilde_01_del_buffer->getSize()) + this->delaytilde_01_del_reader - ((size > this->delaytilde_01_del__maxdelay ? this->delaytilde_01_del__maxdelay : (size < (this->delaytilde_01_del_reader != this->delaytilde_01_del_writer) ? this->delaytilde_01_del_reader != this->delaytilde_01_del_writer : size)));
        long index1 = (long)(rnbo_floor(r));
        number frac = r - index1;
        Index index2 = (Index)(index1 + 1);

        return this->cosineinterp(frac, this->delaytilde_01_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_01_del_wrap))
        ), this->delaytilde_01_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_01_del_wrap))
        ));
    }

    number r = (int)(this->delaytilde_01_del_buffer->getSize()) + this->delaytilde_01_del_reader - ((size > this->delaytilde_01_del__maxdelay ? this->delaytilde_01_del__maxdelay : (size < (this->delaytilde_01_del_reader != this->delaytilde_01_del_writer) ? this->delaytilde_01_del_reader != this->delaytilde_01_del_writer : size)));
    long index1 = (long)(rnbo_floor(r));

    return this->delaytilde_01_del_buffer->getSample(
        0,
        (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_01_del_wrap))
    );
}

void delaytilde_01_del_write(number v) {
    this->delaytilde_01_del_writer = this->delaytilde_01_del_reader;
    this->delaytilde_01_del_buffer[(Index)this->delaytilde_01_del_writer] = v;
}

number delaytilde_01_del_next(number v, int size) {
    number effectiveSize = (size == -1 ? this->delaytilde_01_del__maxdelay : size);
    number val = this->delaytilde_01_del_read(effectiveSize, 0);
    this->delaytilde_01_del_write(v);
    this->delaytilde_01_del_step();
    return val;
}

array<Index, 2> delaytilde_01_del_calcSizeInSamples() {
    number sizeInSamples = 0;
    Index allocatedSizeInSamples = 0;

    {
        sizeInSamples = this->delaytilde_01_del_evaluateSizeExpr(this->samplerate(), this->vectorsize());
        this->delaytilde_01_del_sizemode = 0;
    }

    sizeInSamples = rnbo_floor(sizeInSamples);
    sizeInSamples = this->maximum(sizeInSamples, 2);
    allocatedSizeInSamples = (Index)(sizeInSamples);
    allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
    return {sizeInSamples, allocatedSizeInSamples};
}

void delaytilde_01_del_init() {
    auto result = this->delaytilde_01_del_calcSizeInSamples();
    this->delaytilde_01_del__maxdelay = result[0];
    Index requestedSizeInSamples = (Index)(result[1]);
    this->delaytilde_01_del_buffer->requestSize(requestedSizeInSamples, 1);
    this->delaytilde_01_del_wrap = requestedSizeInSamples - 1;
}

void delaytilde_01_del_clear() {
    this->delaytilde_01_del_buffer->setZero();
}

void delaytilde_01_del_reset() {
    auto result = this->delaytilde_01_del_calcSizeInSamples();
    this->delaytilde_01_del__maxdelay = result[0];
    Index allocatedSizeInSamples = (Index)(result[1]);
    this->delaytilde_01_del_buffer->setSize(allocatedSizeInSamples);
    updateDataRef(this, this->delaytilde_01_del_buffer);
    this->delaytilde_01_del_wrap = this->delaytilde_01_del_buffer->getSize() - 1;
    this->delaytilde_01_del_clear();

    if (this->delaytilde_01_del_reader >= this->delaytilde_01_del__maxdelay || this->delaytilde_01_del_writer >= this->delaytilde_01_del__maxdelay) {
        this->delaytilde_01_del_reader = 0;
        this->delaytilde_01_del_writer = 0;
    }
}

void delaytilde_01_del_dspsetup() {
    this->delaytilde_01_del_reset();
}

number delaytilde_01_del_evaluateSizeExpr(number samplerate, number vectorsize) {
    RNBO_UNUSED(vectorsize);
    RNBO_UNUSED(samplerate);
    return 1920;
}

number delaytilde_01_del_size() {
    return this->delaytilde_01_del__maxdelay;
}

void delaytilde_01_dspsetup(bool force) {
    if ((bool)(this->delaytilde_01_setupDone) && (bool)(!(bool)(force)))
        return;

    this->delaytilde_01_rampInSamples = (long)(this->mstosamps(0));
    this->delaytilde_01_lastDelay = -1;
    this->delaytilde_01_setupDone = true;
    this->delaytilde_01_del_dspsetup();
}

void scopetilde_01_updateEffectiveCount() {
    number effectiveCount = 256 * 1 + 256 * 0;
    this->scopetilde_01_effectiveCount = this->maximum(effectiveCount, 256);
}

void scopetilde_01_dspsetup(bool force) {
    if ((bool)(this->scopetilde_01_setupDone) && (bool)(!(bool)(force)))
        return;

    {
        this->scopetilde_01_mode = 1;
    }

    this->getEngine()->sendListMessage(
        TAG("setup"),
        TAG("scope~_obj-98"),
        {1, 1, this->samplerate(), 0, 1, 0, 0, 128, this->scopetilde_01_mode},
        this->_currentTime
    );;

    this->scopetilde_01_updateEffectiveCount();
    this->scopetilde_01_setupDone = true;
}

void scopetilde_02_updateEffectiveCount() {
    number effectiveCount = 256 * 1 + 256 * 0;
    this->scopetilde_02_effectiveCount = this->maximum(effectiveCount, 256);
}

void scopetilde_02_dspsetup(bool force) {
    if ((bool)(this->scopetilde_02_setupDone) && (bool)(!(bool)(force)))
        return;

    {
        this->scopetilde_02_mode = 1;
    }

    this->getEngine()->sendListMessage(
        TAG("setup"),
        TAG("scope~_obj-85"),
        {1, 1, this->samplerate(), 0, 1, 0, 0, 128, this->scopetilde_02_mode},
        this->_currentTime
    );;

    this->scopetilde_02_updateEffectiveCount();
    this->scopetilde_02_setupDone = true;
}

void scopetilde_03_updateEffectiveCount() {
    number effectiveCount = 256 * 1 + 256 * 0;
    this->scopetilde_03_effectiveCount = this->maximum(effectiveCount, 256);
}

void scopetilde_03_dspsetup(bool force) {
    if ((bool)(this->scopetilde_03_setupDone) && (bool)(!(bool)(force)))
        return;

    {
        this->scopetilde_03_mode = 1;
    }

    this->getEngine()->sendListMessage(
        TAG("setup"),
        TAG("scope~_obj-86"),
        {1, 1, this->samplerate(), 0, 1, 0, 0, 128, this->scopetilde_03_mode},
        this->_currentTime
    );;

    this->scopetilde_03_updateEffectiveCount();
    this->scopetilde_03_setupDone = true;
}

void delaytilde_02_del_step() {
    this->delaytilde_02_del_reader++;

    if (this->delaytilde_02_del_reader >= (int)(this->delaytilde_02_del_buffer->getSize()))
        this->delaytilde_02_del_reader = 0;
}

number delaytilde_02_del_read(number size, Int interp) {
    if (interp == 0) {
        number r = (int)(this->delaytilde_02_del_buffer->getSize()) + this->delaytilde_02_del_reader - ((size > this->delaytilde_02_del__maxdelay ? this->delaytilde_02_del__maxdelay : (size < (this->delaytilde_02_del_reader != this->delaytilde_02_del_writer) ? this->delaytilde_02_del_reader != this->delaytilde_02_del_writer : size)));
        long index1 = (long)(rnbo_floor(r));
        number frac = r - index1;
        long index2 = (long)(index1 + 1);

        return this->linearinterp(frac, this->delaytilde_02_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_02_del_wrap))
        ), this->delaytilde_02_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_02_del_wrap))
        ));
    } else if (interp == 1) {
        number r = (int)(this->delaytilde_02_del_buffer->getSize()) + this->delaytilde_02_del_reader - ((size > this->delaytilde_02_del__maxdelay ? this->delaytilde_02_del__maxdelay : (size < (1 + this->delaytilde_02_del_reader != this->delaytilde_02_del_writer) ? 1 + this->delaytilde_02_del_reader != this->delaytilde_02_del_writer : size)));
        long index1 = (long)(rnbo_floor(r));
        number frac = r - index1;
        Index index2 = (Index)(index1 + 1);
        Index index3 = (Index)(index2 + 1);
        Index index4 = (Index)(index3 + 1);

        return this->cubicinterp(frac, this->delaytilde_02_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_02_del_wrap))
        ), this->delaytilde_02_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_02_del_wrap))
        ), this->delaytilde_02_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index3 & (UBinOpInt)this->delaytilde_02_del_wrap))
        ), this->delaytilde_02_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index4 & (UBinOpInt)this->delaytilde_02_del_wrap))
        ));
    } else if (interp == 2) {
        number r = (int)(this->delaytilde_02_del_buffer->getSize()) + this->delaytilde_02_del_reader - ((size > this->delaytilde_02_del__maxdelay ? this->delaytilde_02_del__maxdelay : (size < (1 + this->delaytilde_02_del_reader != this->delaytilde_02_del_writer) ? 1 + this->delaytilde_02_del_reader != this->delaytilde_02_del_writer : size)));
        long index1 = (long)(rnbo_floor(r));
        number frac = r - index1;
        Index index2 = (Index)(index1 + 1);
        Index index3 = (Index)(index2 + 1);
        Index index4 = (Index)(index3 + 1);

        return this->splineinterp(frac, this->delaytilde_02_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_02_del_wrap))
        ), this->delaytilde_02_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_02_del_wrap))
        ), this->delaytilde_02_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index3 & (UBinOpInt)this->delaytilde_02_del_wrap))
        ), this->delaytilde_02_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index4 & (UBinOpInt)this->delaytilde_02_del_wrap))
        ));
    } else if (interp == 3) {
        number r = (int)(this->delaytilde_02_del_buffer->getSize()) + this->delaytilde_02_del_reader - ((size > this->delaytilde_02_del__maxdelay ? this->delaytilde_02_del__maxdelay : (size < (this->delaytilde_02_del_reader != this->delaytilde_02_del_writer) ? this->delaytilde_02_del_reader != this->delaytilde_02_del_writer : size)));
        long index1 = (long)(rnbo_floor(r));
        number frac = r - index1;
        Index index2 = (Index)(index1 + 1);

        return this->cosineinterp(frac, this->delaytilde_02_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_02_del_wrap))
        ), this->delaytilde_02_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_02_del_wrap))
        ));
    }

    number r = (int)(this->delaytilde_02_del_buffer->getSize()) + this->delaytilde_02_del_reader - ((size > this->delaytilde_02_del__maxdelay ? this->delaytilde_02_del__maxdelay : (size < (this->delaytilde_02_del_reader != this->delaytilde_02_del_writer) ? this->delaytilde_02_del_reader != this->delaytilde_02_del_writer : size)));
    long index1 = (long)(rnbo_floor(r));

    return this->delaytilde_02_del_buffer->getSample(
        0,
        (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_02_del_wrap))
    );
}

void delaytilde_02_del_write(number v) {
    this->delaytilde_02_del_writer = this->delaytilde_02_del_reader;
    this->delaytilde_02_del_buffer[(Index)this->delaytilde_02_del_writer] = v;
}

number delaytilde_02_del_next(number v, int size) {
    number effectiveSize = (size == -1 ? this->delaytilde_02_del__maxdelay : size);
    number val = this->delaytilde_02_del_read(effectiveSize, 0);
    this->delaytilde_02_del_write(v);
    this->delaytilde_02_del_step();
    return val;
}

array<Index, 2> delaytilde_02_del_calcSizeInSamples() {
    number sizeInSamples = 0;
    Index allocatedSizeInSamples = 0;

    {
        sizeInSamples = this->delaytilde_02_del_evaluateSizeExpr(this->samplerate(), this->vectorsize());
        this->delaytilde_02_del_sizemode = 0;
    }

    sizeInSamples = rnbo_floor(sizeInSamples);
    sizeInSamples = this->maximum(sizeInSamples, 2);
    allocatedSizeInSamples = (Index)(sizeInSamples);
    allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
    return {sizeInSamples, allocatedSizeInSamples};
}

void delaytilde_02_del_init() {
    auto result = this->delaytilde_02_del_calcSizeInSamples();
    this->delaytilde_02_del__maxdelay = result[0];
    Index requestedSizeInSamples = (Index)(result[1]);
    this->delaytilde_02_del_buffer->requestSize(requestedSizeInSamples, 1);
    this->delaytilde_02_del_wrap = requestedSizeInSamples - 1;
}

void delaytilde_02_del_clear() {
    this->delaytilde_02_del_buffer->setZero();
}

void delaytilde_02_del_reset() {
    auto result = this->delaytilde_02_del_calcSizeInSamples();
    this->delaytilde_02_del__maxdelay = result[0];
    Index allocatedSizeInSamples = (Index)(result[1]);
    this->delaytilde_02_del_buffer->setSize(allocatedSizeInSamples);
    updateDataRef(this, this->delaytilde_02_del_buffer);
    this->delaytilde_02_del_wrap = this->delaytilde_02_del_buffer->getSize() - 1;
    this->delaytilde_02_del_clear();

    if (this->delaytilde_02_del_reader >= this->delaytilde_02_del__maxdelay || this->delaytilde_02_del_writer >= this->delaytilde_02_del__maxdelay) {
        this->delaytilde_02_del_reader = 0;
        this->delaytilde_02_del_writer = 0;
    }
}

void delaytilde_02_del_dspsetup() {
    this->delaytilde_02_del_reset();
}

number delaytilde_02_del_evaluateSizeExpr(number samplerate, number vectorsize) {
    RNBO_UNUSED(vectorsize);
    RNBO_UNUSED(samplerate);
    return 1920;
}

number delaytilde_02_del_size() {
    return this->delaytilde_02_del__maxdelay;
}

void delaytilde_02_dspsetup(bool force) {
    if ((bool)(this->delaytilde_02_setupDone) && (bool)(!(bool)(force)))
        return;

    this->delaytilde_02_rampInSamples = (long)(this->mstosamps(0));
    this->delaytilde_02_lastDelay = -1;
    this->delaytilde_02_setupDone = true;
    this->delaytilde_02_del_dspsetup();
}

void numberobj_11_init() {
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-41"), 1, this->_currentTime);
}

void numberobj_11_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_11_value;
}

void numberobj_11_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_11_value_set(preset["value"]);
}

void param_01_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_01_value;
}

void param_01_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_01_value_set(preset["value"]);
}

number numbertilde_06_smooth_d_next(number x) {
    number temp = (number)(x - this->numbertilde_06_smooth_d_prev);
    this->numbertilde_06_smooth_d_prev = x;
    return temp;
}

void numbertilde_06_smooth_d_dspsetup() {
    this->numbertilde_06_smooth_d_reset();
}

void numbertilde_06_smooth_d_reset() {
    this->numbertilde_06_smooth_d_prev = 0;
}

number numbertilde_06_smooth_next(number x, number up, number down) {
    if (this->numbertilde_06_smooth_d_next(x) != 0.) {
        if (x > this->numbertilde_06_smooth_prev) {
            number _up = up;

            if (_up < 1)
                _up = 1;

            this->numbertilde_06_smooth_index = _up;
            this->numbertilde_06_smooth_increment = (x - this->numbertilde_06_smooth_prev) / _up;
        } else if (x < this->numbertilde_06_smooth_prev) {
            number _down = down;

            if (_down < 1)
                _down = 1;

            this->numbertilde_06_smooth_index = _down;
            this->numbertilde_06_smooth_increment = (x - this->numbertilde_06_smooth_prev) / _down;
        }
    }

    if (this->numbertilde_06_smooth_index > 0) {
        this->numbertilde_06_smooth_prev += this->numbertilde_06_smooth_increment;
        this->numbertilde_06_smooth_index -= 1;
    } else {
        this->numbertilde_06_smooth_prev = x;
    }

    return this->numbertilde_06_smooth_prev;
}

void numbertilde_06_smooth_reset() {
    this->numbertilde_06_smooth_prev = 0;
    this->numbertilde_06_smooth_index = 0;
    this->numbertilde_06_smooth_increment = 0;
    this->numbertilde_06_smooth_d_reset();
}

void numbertilde_06_init() {
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number~_obj-32"), 1, this->_currentTime);
}

void numbertilde_06_dspsetup(bool force) {
    if ((bool)(this->numbertilde_06_setupDone) && (bool)(!(bool)(force)))
        return;

    this->numbertilde_06_currentIntervalInSamples = this->mstosamps(100);
    this->numbertilde_06_currentInterval = this->numbertilde_06_currentIntervalInSamples;
    this->numbertilde_06_rampInSamples = this->mstosamps(this->numbertilde_06_ramp);
    this->numbertilde_06_setupDone = true;
    this->numbertilde_06_smooth_d_dspsetup();
}

number numbertilde_07_smooth_d_next(number x) {
    number temp = (number)(x - this->numbertilde_07_smooth_d_prev);
    this->numbertilde_07_smooth_d_prev = x;
    return temp;
}

void numbertilde_07_smooth_d_dspsetup() {
    this->numbertilde_07_smooth_d_reset();
}

void numbertilde_07_smooth_d_reset() {
    this->numbertilde_07_smooth_d_prev = 0;
}

number numbertilde_07_smooth_next(number x, number up, number down) {
    if (this->numbertilde_07_smooth_d_next(x) != 0.) {
        if (x > this->numbertilde_07_smooth_prev) {
            number _up = up;

            if (_up < 1)
                _up = 1;

            this->numbertilde_07_smooth_index = _up;
            this->numbertilde_07_smooth_increment = (x - this->numbertilde_07_smooth_prev) / _up;
        } else if (x < this->numbertilde_07_smooth_prev) {
            number _down = down;

            if (_down < 1)
                _down = 1;

            this->numbertilde_07_smooth_index = _down;
            this->numbertilde_07_smooth_increment = (x - this->numbertilde_07_smooth_prev) / _down;
        }
    }

    if (this->numbertilde_07_smooth_index > 0) {
        this->numbertilde_07_smooth_prev += this->numbertilde_07_smooth_increment;
        this->numbertilde_07_smooth_index -= 1;
    } else {
        this->numbertilde_07_smooth_prev = x;
    }

    return this->numbertilde_07_smooth_prev;
}

void numbertilde_07_smooth_reset() {
    this->numbertilde_07_smooth_prev = 0;
    this->numbertilde_07_smooth_index = 0;
    this->numbertilde_07_smooth_increment = 0;
    this->numbertilde_07_smooth_d_reset();
}

void numbertilde_07_init() {
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number~_obj-31"), 1, this->_currentTime);
}

void numbertilde_07_dspsetup(bool force) {
    if ((bool)(this->numbertilde_07_setupDone) && (bool)(!(bool)(force)))
        return;

    this->numbertilde_07_currentIntervalInSamples = this->mstosamps(100);
    this->numbertilde_07_currentInterval = this->numbertilde_07_currentIntervalInSamples;
    this->numbertilde_07_rampInSamples = this->mstosamps(this->numbertilde_07_ramp);
    this->numbertilde_07_setupDone = true;
    this->numbertilde_07_smooth_d_dspsetup();
}

void numberobj_12_init() {
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-60"), 1, this->_currentTime);
}

void numberobj_12_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_12_value;
}

void numberobj_12_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_12_value_set(preset["value"]);
}

void numberobj_13_init() {
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-64"), 1, this->_currentTime);
}

void numberobj_13_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_13_value;
}

void numberobj_13_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_13_value_set(preset["value"]);
}

void param_02_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_02_value;
}

void param_02_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_02_value_set(preset["value"]);
}

void numberobj_14_init() {
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-55"), 1, this->_currentTime);
}

void numberobj_14_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_14_value;
}

void numberobj_14_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_14_value_set(preset["value"]);
}

void param_03_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_03_value;
}

void param_03_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_03_value_set(preset["value"]);
}

void numberobj_15_init() {
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-37"), 1, this->_currentTime);
}

void numberobj_15_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_15_value;
}

void numberobj_15_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_15_value_set(preset["value"]);
}

number numbertilde_08_smooth_d_next(number x) {
    number temp = (number)(x - this->numbertilde_08_smooth_d_prev);
    this->numbertilde_08_smooth_d_prev = x;
    return temp;
}

void numbertilde_08_smooth_d_dspsetup() {
    this->numbertilde_08_smooth_d_reset();
}

void numbertilde_08_smooth_d_reset() {
    this->numbertilde_08_smooth_d_prev = 0;
}

number numbertilde_08_smooth_next(number x, number up, number down) {
    if (this->numbertilde_08_smooth_d_next(x) != 0.) {
        if (x > this->numbertilde_08_smooth_prev) {
            number _up = up;

            if (_up < 1)
                _up = 1;

            this->numbertilde_08_smooth_index = _up;
            this->numbertilde_08_smooth_increment = (x - this->numbertilde_08_smooth_prev) / _up;
        } else if (x < this->numbertilde_08_smooth_prev) {
            number _down = down;

            if (_down < 1)
                _down = 1;

            this->numbertilde_08_smooth_index = _down;
            this->numbertilde_08_smooth_increment = (x - this->numbertilde_08_smooth_prev) / _down;
        }
    }

    if (this->numbertilde_08_smooth_index > 0) {
        this->numbertilde_08_smooth_prev += this->numbertilde_08_smooth_increment;
        this->numbertilde_08_smooth_index -= 1;
    } else {
        this->numbertilde_08_smooth_prev = x;
    }

    return this->numbertilde_08_smooth_prev;
}

void numbertilde_08_smooth_reset() {
    this->numbertilde_08_smooth_prev = 0;
    this->numbertilde_08_smooth_index = 0;
    this->numbertilde_08_smooth_increment = 0;
    this->numbertilde_08_smooth_d_reset();
}

void numbertilde_08_init() {
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number~_obj-30"), 1, this->_currentTime);
}

void numbertilde_08_dspsetup(bool force) {
    if ((bool)(this->numbertilde_08_setupDone) && (bool)(!(bool)(force)))
        return;

    this->numbertilde_08_currentIntervalInSamples = this->mstosamps(100);
    this->numbertilde_08_currentInterval = this->numbertilde_08_currentIntervalInSamples;
    this->numbertilde_08_rampInSamples = this->mstosamps(this->numbertilde_08_ramp);
    this->numbertilde_08_setupDone = true;
    this->numbertilde_08_smooth_d_dspsetup();
}

number globaltransport_getTempoAtSample(SampleIndex sampleOffset) {
    RNBO_UNUSED(sampleOffset);
    return (this->vs > 0 ? this->globaltransport_tempo[(Index)sampleOffset] : this->globaltransport_lastTempo);
}

number globaltransport_getTempo() {
    return this->globaltransport_getTempoAtSample(this->sampleOffsetIntoNextAudioBuffer);
}

number globaltransport_getStateAtSample(SampleIndex sampleOffset) {
    RNBO_UNUSED(sampleOffset);
    return (this->vs > 0 ? this->globaltransport_state[(Index)sampleOffset] : this->globaltransport_lastState);
}

number globaltransport_getState() {
    return this->globaltransport_getStateAtSample(this->sampleOffsetIntoNextAudioBuffer);
}

number globaltransport_getBeatTimeAtMsTime(MillisecondTime time) {
    number i = 2;

    while (i < this->globaltransport_beatTimeChanges->length && this->globaltransport_beatTimeChanges[(Index)(i + 1)] <= time) {
        i += 2;
    }

    i -= 2;
    number beatTimeBase = this->globaltransport_beatTimeChanges[(Index)i];

    if (this->globaltransport_getState() == 0)
        return beatTimeBase;

    number beatTimeBaseMsTime = this->globaltransport_beatTimeChanges[(Index)(i + 1)];
    number diff = time - beatTimeBaseMsTime;
    return beatTimeBase + this->mstobeats(diff);
}

bool globaltransport_setTempo(number tempo, bool notify) {
    if ((bool)(notify)) {
        this->processTempoEvent(this->currenttime(), tempo);
        this->globaltransport_notify = true;
    } else if (this->globaltransport_getTempo() != tempo) {
        MillisecondTime ct = this->currenttime();
        this->globaltransport_beatTimeChanges->push(this->globaltransport_getBeatTimeAtMsTime(ct));
        this->globaltransport_beatTimeChanges->push(ct);

        fillSignal(
            this->globaltransport_tempo,
            this->vs,
            tempo,
            (Index)(this->sampleOffsetIntoNextAudioBuffer)
        );

        this->globaltransport_lastTempo = tempo;
        this->globaltransport_tempoNeedsReset = true;
        return true;
    }

    return false;
}

number globaltransport_getBeatTime() {
    return this->globaltransport_getBeatTimeAtMsTime(this->currenttime());
}

bool globaltransport_setState(number state, bool notify) {
    if ((bool)(notify)) {
        this->processTransportEvent(this->currenttime(), TransportState(state));
        this->globaltransport_notify = true;
    } else if (this->globaltransport_getState() != state) {
        fillSignal(
            this->globaltransport_state,
            this->vs,
            state,
            (Index)(this->sampleOffsetIntoNextAudioBuffer)
        );

        this->globaltransport_lastState = TransportState(state);
        this->globaltransport_stateNeedsReset = true;

        if (state == 0) {
            this->globaltransport_beatTimeChanges->push(this->globaltransport_getBeatTime());
            this->globaltransport_beatTimeChanges->push(this->currenttime());
        }

        return true;
    }

    return false;
}

bool globaltransport_setBeatTime(number beattime, bool notify) {
    if ((bool)(notify)) {
        this->processBeatTimeEvent(this->currenttime(), beattime);
        this->globaltransport_notify = true;
        return false;
    } else {
        bool beatTimeHasChanged = false;
        float oldBeatTime = (float)(this->globaltransport_getBeatTime());
        float newBeatTime = (float)(beattime);

        if (oldBeatTime != newBeatTime) {
            beatTimeHasChanged = true;
        }

        this->globaltransport_beatTimeChanges->push(beattime);
        this->globaltransport_beatTimeChanges->push(this->currenttime());
        return beatTimeHasChanged;
    }
}

number globaltransport_getBeatTimeAtSample(SampleIndex sampleOffset) {
    MillisecondTime msOffset = this->sampstoms(sampleOffset);
    return this->globaltransport_getBeatTimeAtMsTime(this->currenttime() + msOffset);
}

array<number, 2> globaltransport_getTimeSignatureAtMsTime(MillisecondTime time) {
    number i = 3;

    while (i < this->globaltransport_timeSignatureChanges->length && this->globaltransport_timeSignatureChanges[(Index)(i + 2)] <= time) {
        i += 3;
    }

    i -= 3;

    return {
        this->globaltransport_timeSignatureChanges[(Index)i],
        this->globaltransport_timeSignatureChanges[(Index)(i + 1)]
    };
}

array<number, 2> globaltransport_getTimeSignature() {
    return this->globaltransport_getTimeSignatureAtMsTime(this->currenttime());
}

array<number, 2> globaltransport_getTimeSignatureAtSample(SampleIndex sampleOffset) {
    MillisecondTime msOffset = this->sampstoms(sampleOffset);
    return this->globaltransport_getTimeSignatureAtMsTime(this->currenttime() + msOffset);
}

bool globaltransport_setTimeSignature(number numerator, number denominator, bool notify) {
    if ((bool)(notify)) {
        this->processTimeSignatureEvent(this->currenttime(), (int)(numerator), (int)(denominator));
        this->globaltransport_notify = true;
    } else {
        array<number, 2> currentSig = this->globaltransport_getTimeSignature();

        if (currentSig[0] != numerator || currentSig[1] != denominator) {
            this->globaltransport_timeSignatureChanges->push(numerator);
            this->globaltransport_timeSignatureChanges->push(denominator);
            this->globaltransport_timeSignatureChanges->push(this->currenttime());
            return true;
        }
    }

    return false;
}

void globaltransport_advance() {
    if ((bool)(this->globaltransport_tempoNeedsReset)) {
        fillSignal(this->globaltransport_tempo, this->vs, this->globaltransport_lastTempo);
        this->globaltransport_tempoNeedsReset = false;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendTempoEvent(this->globaltransport_lastTempo);
        }
    }

    if ((bool)(this->globaltransport_stateNeedsReset)) {
        fillSignal(this->globaltransport_state, this->vs, this->globaltransport_lastState);
        this->globaltransport_stateNeedsReset = false;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendTransportEvent(TransportState(this->globaltransport_lastState));
        }
    }

    if (this->globaltransport_beatTimeChanges->length > 2) {
        this->globaltransport_beatTimeChanges[0] = this->globaltransport_beatTimeChanges[(Index)(this->globaltransport_beatTimeChanges->length - 2)];
        this->globaltransport_beatTimeChanges[1] = this->globaltransport_beatTimeChanges[(Index)(this->globaltransport_beatTimeChanges->length - 1)];
        this->globaltransport_beatTimeChanges->length = 2;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendBeatTimeEvent(this->globaltransport_beatTimeChanges[0]);
        }
    }

    if (this->globaltransport_timeSignatureChanges->length > 3) {
        this->globaltransport_timeSignatureChanges[0] = this->globaltransport_timeSignatureChanges[(Index)(this->globaltransport_timeSignatureChanges->length - 3)];
        this->globaltransport_timeSignatureChanges[1] = this->globaltransport_timeSignatureChanges[(Index)(this->globaltransport_timeSignatureChanges->length - 2)];
        this->globaltransport_timeSignatureChanges[2] = this->globaltransport_timeSignatureChanges[(Index)(this->globaltransport_timeSignatureChanges->length - 1)];
        this->globaltransport_timeSignatureChanges->length = 3;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendTimeSignatureEvent(
                (int)(this->globaltransport_timeSignatureChanges[0]),
                (int)(this->globaltransport_timeSignatureChanges[1])
            );
        }
    }

    this->globaltransport_notify = false;
}

void globaltransport_dspsetup(bool force) {
    if ((bool)(this->globaltransport_setupDone) && (bool)(!(bool)(force)))
        return;

    fillSignal(this->globaltransport_tempo, this->vs, this->globaltransport_lastTempo);
    this->globaltransport_tempoNeedsReset = false;
    fillSignal(this->globaltransport_state, this->vs, this->globaltransport_lastState);
    this->globaltransport_stateNeedsReset = false;
    this->globaltransport_setupDone = true;
}

bool stackprotect_check() {
    this->stackprotect_count++;

    if (this->stackprotect_count > 128) {
        console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
        return true;
    }

    return false;
}

void updateTime(MillisecondTime time) {
    this->_currentTime = time;
    this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));

    if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;

    if (this->sampleOffsetIntoNextAudioBuffer < 0)
        this->sampleOffsetIntoNextAudioBuffer = 0;
}

void assign_defaults()
{
    dspexpr_03_in1 = 0;
    dspexpr_03_in2 = 0;
    dspexpr_04_in1 = 0;
    dspexpr_04_in2 = 0;
    dspexpr_05_in1 = 0;
    dspexpr_05_in2 = 0;
    dspexpr_06_in1 = 0;
    dspexpr_06_in2 = 0;
    p_01_target = 0;
    p_02_target = 0;
    delaytilde_01_delay = 0;
    delaytilde_02_delay = 0;
    dspexpr_07_in1 = 0;
    p_03_target = 0;
    slide_tilde_01_x = 0;
    slide_tilde_01_up = 1000;
    slide_tilde_01_down = 1000;
    p_04_target = 0;
    mstosamps_tilde_01_ms = 0;
    dspexpr_08_in1 = 0;
    dspexpr_08_in2 = 5;
    numberobj_11_value = 50;
    numberobj_11_value_setter(numberobj_11_value);
    param_01_value = 50;
    triangle_tilde_01_phase = 0;
    triangle_tilde_01_duty = 0.5;
    numbertilde_06_input_number = 0;
    numbertilde_06_ramp = 0;
    dspexpr_09_in1 = 0;
    slide_tilde_02_x = 0;
    slide_tilde_02_up = 1000;
    slide_tilde_02_down = 1000;
    mstosamps_tilde_02_ms = 0;
    dspexpr_10_in1 = 0;
    dspexpr_10_in2 = 5;
    dspexpr_11_in1 = 0;
    dspexpr_11_in2 = 0;
    triangle_tilde_02_phase = 0;
    triangle_tilde_02_duty = 0.5;
    numbertilde_07_input_number = 0;
    numbertilde_07_ramp = 0;
    dspexpr_12_in1 = 0;
    scale_tilde_05_x = 0;
    scale_tilde_05_lowin = 0;
    scale_tilde_05_hiin = 5;
    scale_tilde_05_lowout = 0.7;
    scale_tilde_05_highout = 1;
    scale_tilde_05_pow = 1;
    line_02_time = 10;
    numberobj_12_value = 3.7201824282796907;
    numberobj_12_value_setter(numberobj_12_value);
    scale_03_inlow = 0;
    scale_03_inhigh = 100;
    scale_03_outlow = 0;
    scale_03_outhigh = 5;
    scale_03_power = 1.325;
    numberobj_13_value = 80;
    numberobj_13_value_setter(numberobj_13_value);
    param_02_value = 50;
    numberobj_14_value = 0;
    numberobj_14_value_setter(numberobj_14_value);
    scale_04_inlow = 0;
    scale_04_inhigh = 100;
    scale_04_outlow = 0;
    scale_04_outhigh = 0.5;
    scale_04_power = 2;
    param_03_value = 0;
    numberobj_15_value = 0;
    numberobj_15_value_setter(numberobj_15_value);
    line_03_time = 10;
    numbertilde_08_input_number = 0;
    numbertilde_08_ramp = 0;
    ctlin_01_input = 0;
    ctlin_01_controller = 1;
    ctlin_01_channel = -1;
    expr_05_in1 = 0;
    expr_05_in2 = 0.007874015748;
    expr_05_out1 = 0;
    ctlin_02_input = 0;
    ctlin_02_controller = 2;
    ctlin_02_channel = -1;
    expr_06_in1 = 0;
    expr_06_in2 = 0.007874015748;
    expr_06_out1 = 0;
    ctlin_03_input = 0;
    ctlin_03_controller = 3;
    ctlin_03_channel = -1;
    expr_07_in1 = 0;
    expr_07_in2 = 0.007874015748;
    expr_07_out1 = 0;
    _currentTime = 0;
    audioProcessSampleCount = 0;
    sampleOffsetIntoNextAudioBuffer = 0;
    zeroBuffer = nullptr;
    dummyBuffer = nullptr;
    signals[0] = nullptr;
    signals[1] = nullptr;
    signals[2] = nullptr;
    signals[3] = nullptr;
    signals[4] = nullptr;
    signals[5] = nullptr;
    signals[6] = nullptr;
    signals[7] = nullptr;
    signals[8] = nullptr;
    signals[9] = nullptr;
    didAllocateSignals = 0;
    vs = 0;
    maxvs = 0;
    sr = 44100;
    invsr = 0.00002267573696;
    delaytilde_01_lastDelay = -1;
    delaytilde_01_crossfadeDelay = 0;
    delaytilde_01_ramp = 0;
    delaytilde_01_rampInSamples = 0;
    delaytilde_01_del__maxdelay = 0;
    delaytilde_01_del_sizemode = 0;
    delaytilde_01_del_wrap = 0;
    delaytilde_01_del_reader = 0;
    delaytilde_01_del_writer = 0;
    delaytilde_01_setupDone = false;
    scopetilde_01_lastValue = 0;
    scopetilde_01_effectiveCount = 256;
    scopetilde_01_xsign = 1;
    scopetilde_01_ysign = 1;
    scopetilde_01_mode = 0;
    scopetilde_01_setupDone = false;
    scopetilde_02_lastValue = 0;
    scopetilde_02_effectiveCount = 256;
    scopetilde_02_xsign = 1;
    scopetilde_02_ysign = 1;
    scopetilde_02_mode = 0;
    scopetilde_02_setupDone = false;
    scopetilde_03_lastValue = 0;
    scopetilde_03_effectiveCount = 256;
    scopetilde_03_xsign = 1;
    scopetilde_03_ysign = 1;
    scopetilde_03_mode = 0;
    scopetilde_03_setupDone = false;
    delaytilde_02_lastDelay = -1;
    delaytilde_02_crossfadeDelay = 0;
    delaytilde_02_ramp = 0;
    delaytilde_02_rampInSamples = 0;
    delaytilde_02_del__maxdelay = 0;
    delaytilde_02_del_sizemode = 0;
    delaytilde_02_del_wrap = 0;
    delaytilde_02_del_reader = 0;
    delaytilde_02_del_writer = 0;
    delaytilde_02_setupDone = false;
    slide_tilde_01_prev = 0;
    numberobj_11_currentFormat = 6;
    numberobj_11_lastValue = 0;
    param_01_lastValue = 0;
    numbertilde_06_currentInterval = 0;
    numbertilde_06_currentIntervalInSamples = 0;
    numbertilde_06_lastValue = 0;
    numbertilde_06_outValue = 0;
    numbertilde_06_rampInSamples = 0;
    numbertilde_06_currentMode = 1;
    numbertilde_06_smooth_d_prev = 0;
    numbertilde_06_smooth_prev = 0;
    numbertilde_06_smooth_index = 0;
    numbertilde_06_smooth_increment = 0;
    numbertilde_06_setupDone = false;
    slide_tilde_02_prev = 0;
    numbertilde_07_currentInterval = 0;
    numbertilde_07_currentIntervalInSamples = 0;
    numbertilde_07_lastValue = 0;
    numbertilde_07_outValue = 0;
    numbertilde_07_rampInSamples = 0;
    numbertilde_07_currentMode = 1;
    numbertilde_07_smooth_d_prev = 0;
    numbertilde_07_smooth_prev = 0;
    numbertilde_07_smooth_index = 0;
    numbertilde_07_smooth_increment = 0;
    numbertilde_07_setupDone = false;
    line_02_currentValue = 0;
    numberobj_12_currentFormat = 6;
    numberobj_12_lastValue = 0;
    numberobj_13_currentFormat = 6;
    numberobj_13_lastValue = 0;
    param_02_lastValue = 0;
    numberobj_14_currentFormat = 6;
    numberobj_14_lastValue = 0;
    param_03_lastValue = 0;
    numberobj_15_currentFormat = 6;
    numberobj_15_lastValue = 0;
    line_03_currentValue = 0;
    numbertilde_08_currentInterval = 0;
    numbertilde_08_currentIntervalInSamples = 0;
    numbertilde_08_lastValue = 0;
    numbertilde_08_outValue = 0;
    numbertilde_08_rampInSamples = 0;
    numbertilde_08_currentMode = 1;
    numbertilde_08_smooth_d_prev = 0;
    numbertilde_08_smooth_prev = 0;
    numbertilde_08_smooth_index = 0;
    numbertilde_08_smooth_increment = 0;
    numbertilde_08_setupDone = false;
    ctlin_01_status = 0;
    ctlin_01_byte1 = -1;
    ctlin_01_inchan = 0;
    ctlin_02_status = 0;
    ctlin_02_byte1 = -1;
    ctlin_02_inchan = 0;
    ctlin_03_status = 0;
    ctlin_03_byte1 = -1;
    ctlin_03_inchan = 0;
    globaltransport_tempo = nullptr;
    globaltransport_tempoNeedsReset = false;
    globaltransport_lastTempo = 120;
    globaltransport_state = nullptr;
    globaltransport_stateNeedsReset = false;
    globaltransport_lastState = 0;
    globaltransport_beatTimeChanges = { 0, 0 };
    globaltransport_timeSignatureChanges = { 4, 4, 0 };
    globaltransport_notify = false;
    globaltransport_setupDone = false;
    stackprotect_count = 0;
    _voiceIndex = 0;
    _noteNumber = 0;
    isMuted = 1;
}

// member variables

    number dspexpr_03_in1;
    number dspexpr_03_in2;
    number dspexpr_04_in1;
    number dspexpr_04_in2;
    number dspexpr_05_in1;
    number dspexpr_05_in2;
    number dspexpr_06_in1;
    number dspexpr_06_in2;
    number p_01_target;
    number p_02_target;
    number delaytilde_01_delay;
    number delaytilde_02_delay;
    number dspexpr_07_in1;
    number p_03_target;
    number slide_tilde_01_x;
    number slide_tilde_01_up;
    number slide_tilde_01_down;
    number p_04_target;
    number mstosamps_tilde_01_ms;
    number dspexpr_08_in1;
    number dspexpr_08_in2;
    number numberobj_11_value;
    number param_01_value;
    number triangle_tilde_01_phase;
    number triangle_tilde_01_duty;
    number numbertilde_06_input_number;
    number numbertilde_06_ramp;
    number dspexpr_09_in1;
    number slide_tilde_02_x;
    number slide_tilde_02_up;
    number slide_tilde_02_down;
    number mstosamps_tilde_02_ms;
    number dspexpr_10_in1;
    number dspexpr_10_in2;
    number dspexpr_11_in1;
    number dspexpr_11_in2;
    number triangle_tilde_02_phase;
    number triangle_tilde_02_duty;
    number numbertilde_07_input_number;
    number numbertilde_07_ramp;
    number dspexpr_12_in1;
    number scale_tilde_05_x;
    number scale_tilde_05_lowin;
    number scale_tilde_05_hiin;
    number scale_tilde_05_lowout;
    number scale_tilde_05_highout;
    number scale_tilde_05_pow;
    list line_02_segments;
    number line_02_time;
    number numberobj_12_value;
    list scale_03_input;
    number scale_03_inlow;
    number scale_03_inhigh;
    number scale_03_outlow;
    number scale_03_outhigh;
    number scale_03_power;
    list scale_03_out;
    number numberobj_13_value;
    number param_02_value;
    number numberobj_14_value;
    list scale_04_input;
    number scale_04_inlow;
    number scale_04_inhigh;
    number scale_04_outlow;
    number scale_04_outhigh;
    number scale_04_power;
    list scale_04_out;
    number param_03_value;
    number numberobj_15_value;
    list line_03_segments;
    number line_03_time;
    number numbertilde_08_input_number;
    number numbertilde_08_ramp;
    number ctlin_01_input;
    number ctlin_01_controller;
    number ctlin_01_channel;
    number expr_05_in1;
    number expr_05_in2;
    number expr_05_out1;
    number ctlin_02_input;
    number ctlin_02_controller;
    number ctlin_02_channel;
    number expr_06_in1;
    number expr_06_in2;
    number expr_06_out1;
    number ctlin_03_input;
    number ctlin_03_controller;
    number ctlin_03_channel;
    number expr_07_in1;
    number expr_07_in2;
    number expr_07_out1;
    MillisecondTime _currentTime;
    SampleIndex audioProcessSampleCount;
    SampleIndex sampleOffsetIntoNextAudioBuffer;
    signal zeroBuffer;
    signal dummyBuffer;
    SampleValue * signals[10];
    bool didAllocateSignals;
    Index vs;
    Index maxvs;
    number sr;
    number invsr;
    number delaytilde_01_lastDelay;
    number delaytilde_01_crossfadeDelay;
    number delaytilde_01_ramp;
    long delaytilde_01_rampInSamples;
    Float64BufferRef delaytilde_01_del_buffer;
    Index delaytilde_01_del__maxdelay;
    Int delaytilde_01_del_sizemode;
    Index delaytilde_01_del_wrap;
    Int delaytilde_01_del_reader;
    Int delaytilde_01_del_writer;
    bool delaytilde_01_setupDone;
    number scopetilde_01_lastValue;
    number scopetilde_01_effectiveCount;
    number scopetilde_01_xmonitorvalue;
    number scopetilde_01_ymonitorvalue;
    list scopetilde_01_monitorbuffer;
    number scopetilde_01_xsign;
    number scopetilde_01_ysign;
    Int scopetilde_01_mode;
    bool scopetilde_01_setupDone;
    number scopetilde_02_lastValue;
    number scopetilde_02_effectiveCount;
    number scopetilde_02_xmonitorvalue;
    number scopetilde_02_ymonitorvalue;
    list scopetilde_02_monitorbuffer;
    number scopetilde_02_xsign;
    number scopetilde_02_ysign;
    Int scopetilde_02_mode;
    bool scopetilde_02_setupDone;
    number scopetilde_03_lastValue;
    number scopetilde_03_effectiveCount;
    number scopetilde_03_xmonitorvalue;
    number scopetilde_03_ymonitorvalue;
    list scopetilde_03_monitorbuffer;
    number scopetilde_03_xsign;
    number scopetilde_03_ysign;
    Int scopetilde_03_mode;
    bool scopetilde_03_setupDone;
    number delaytilde_02_lastDelay;
    number delaytilde_02_crossfadeDelay;
    number delaytilde_02_ramp;
    long delaytilde_02_rampInSamples;
    Float64BufferRef delaytilde_02_del_buffer;
    Index delaytilde_02_del__maxdelay;
    Int delaytilde_02_del_sizemode;
    Index delaytilde_02_del_wrap;
    Int delaytilde_02_del_reader;
    Int delaytilde_02_del_writer;
    bool delaytilde_02_setupDone;
    number slide_tilde_01_prev;
    Int numberobj_11_currentFormat;
    number numberobj_11_lastValue;
    number param_01_lastValue;
    SampleIndex numbertilde_06_currentInterval;
    SampleIndex numbertilde_06_currentIntervalInSamples;
    number numbertilde_06_lastValue;
    number numbertilde_06_outValue;
    number numbertilde_06_rampInSamples;
    Int numbertilde_06_currentMode;
    number numbertilde_06_smooth_d_prev;
    number numbertilde_06_smooth_prev;
    number numbertilde_06_smooth_index;
    number numbertilde_06_smooth_increment;
    bool numbertilde_06_setupDone;
    number slide_tilde_02_prev;
    SampleIndex numbertilde_07_currentInterval;
    SampleIndex numbertilde_07_currentIntervalInSamples;
    number numbertilde_07_lastValue;
    number numbertilde_07_outValue;
    number numbertilde_07_rampInSamples;
    Int numbertilde_07_currentMode;
    number numbertilde_07_smooth_d_prev;
    number numbertilde_07_smooth_prev;
    number numbertilde_07_smooth_index;
    number numbertilde_07_smooth_increment;
    bool numbertilde_07_setupDone;
    list line_02_activeRamps;
    number line_02_currentValue;
    Int numberobj_12_currentFormat;
    number numberobj_12_lastValue;
    Int numberobj_13_currentFormat;
    number numberobj_13_lastValue;
    number param_02_lastValue;
    Int numberobj_14_currentFormat;
    number numberobj_14_lastValue;
    number param_03_lastValue;
    Int numberobj_15_currentFormat;
    number numberobj_15_lastValue;
    list line_03_activeRamps;
    number line_03_currentValue;
    SampleIndex numbertilde_08_currentInterval;
    SampleIndex numbertilde_08_currentIntervalInSamples;
    number numbertilde_08_lastValue;
    number numbertilde_08_outValue;
    number numbertilde_08_rampInSamples;
    Int numbertilde_08_currentMode;
    number numbertilde_08_smooth_d_prev;
    number numbertilde_08_smooth_prev;
    number numbertilde_08_smooth_index;
    number numbertilde_08_smooth_increment;
    bool numbertilde_08_setupDone;
    int ctlin_01_status;
    int ctlin_01_byte1;
    int ctlin_01_inchan;
    int ctlin_02_status;
    int ctlin_02_byte1;
    int ctlin_02_inchan;
    int ctlin_03_status;
    int ctlin_03_byte1;
    int ctlin_03_inchan;
    signal globaltransport_tempo;
    bool globaltransport_tempoNeedsReset;
    number globaltransport_lastTempo;
    signal globaltransport_state;
    bool globaltransport_stateNeedsReset;
    number globaltransport_lastState;
    list globaltransport_beatTimeChanges;
    list globaltransport_timeSignatureChanges;
    bool globaltransport_notify;
    bool globaltransport_setupDone;
    number stackprotect_count;
    DataRef delaytilde_01_del_bufferobj;
    DataRef delaytilde_02_del_bufferobj;
    Index _voiceIndex;
    Int _noteNumber;
    Index isMuted;
    indexlist paramInitIndices;
    indexlist paramInitOrder;
    RNBOSubpatcher_13* p_01;
    RNBOSubpatcher_14* p_02;
    RNBOSubpatcher_15* p_03;
    RNBOSubpatcher_16* p_04;

};

PatcherInterface* creaternbomatic()
{
    return new rnbomatic();
}

#ifndef RNBO_NO_PATCHERFACTORY

extern "C" PatcherFactoryFunctionPtr GetPatcherFactoryFunction(PlatformInterface* platformInterface)
#else

extern "C" PatcherFactoryFunctionPtr rnbomaticFactoryFunction(PlatformInterface* platformInterface)
#endif

{
    Platform::set(platformInterface);
    return creaternbomatic;
}

} // end RNBO namespace

