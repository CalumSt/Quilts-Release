#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <vector>
#include "WavetableOscillator.h"

class WavetableSynth {
public:
  void prepareToPlay(double sampleRate);
  void processBlock(juce::AudioBuffer<float>& buffer,
                    juce::MidiBuffer& midiMessages);

  double sampleRate;

private:
  void initialiseOscillators();
  void handleMidiEvent(const juce::MidiMessage& midiEvent);
  float midiNoteNumberToFrequency(int midiNoteNumber);
  void render(juce::AudioBuffer<float>& buffer, int startSample, int endSample);

  std::vector<WavetableOscillator> oscillators;
  std::vector<float> generateSineWaveTable();
};