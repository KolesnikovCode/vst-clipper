/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class VstclipperAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    VstclipperAudioProcessor();
    ~VstclipperAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    double inputGainValue;
    double thresholdGainValue;
    std::atomic<bool> isHardClippingMode { false };
    std::atomic<float> currentGainReduction = 0.0f;
    
    // Functions
    float hardClipSample(float sample, float threshold)
    {
        if (sample > threshold)
        {
            return threshold;
        }
        else if (sample < -threshold)
        {
            
            return -threshold;
        }

        return sample;
    }

    float softClipSample(float sample, float threshold)
    {
        if (sample > threshold)
        {
            return threshold + (sample - threshold) / (1.0f + powf((sample - threshold), 2));
        }
        else if (sample < -threshold)
        {
            return -threshold + (sample + threshold) / (1.0f + powf((sample + threshold), 2));
        }

        return sample;
    }

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VstclipperAudioProcessor)
};
