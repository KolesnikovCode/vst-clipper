/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VstclipperAudioProcessor::VstclipperAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

VstclipperAudioProcessor::~VstclipperAudioProcessor()
{
}

//==============================================================================
const juce::String VstclipperAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VstclipperAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VstclipperAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool VstclipperAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double VstclipperAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VstclipperAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int VstclipperAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VstclipperAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String VstclipperAudioProcessor::getProgramName (int index)
{
    return {};
}

void VstclipperAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void VstclipperAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void VstclipperAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VstclipperAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void VstclipperAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
 
        // ya ebal mne toje 7
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float inputSample = buffer.getSample(channel, sample) * inputGainValue;
            float outputSample;
            
            // Process audio
            if (isHardClippingMode.load())
            {
                outputSample = hardClipSample(buffer.getSample(channel, sample) * inputGainValue, thresholdGainValue);
                
                channelData[sample] = outputSample;
            } else
            {
                outputSample = softClipSample(buffer.getSample(channel, sample) * inputGainValue, thresholdGainValue);
                
                channelData[sample] = outputSample;
            }
            
            // Set currentGainReduction
            float reduction = inputSample - outputSample;
                                                                                                           
            if (reduction > 0.0f)
            {
                currentGainReduction = reduction;
            }
        }
    }
}

//==============================================================================
bool VstclipperAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* VstclipperAudioProcessor::createEditor()
{
    return new VstclipperAudioProcessorEditor (*this);
}

//==============================================================================
void VstclipperAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // TODO: restore settings
}

void VstclipperAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // TODO: serialize settings
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VstclipperAudioProcessor();
}
