#include "WavetableSynth.h"

void WavetableSynth::prepareToPlay(double sampleRate){

    this->sampleRate = sampleRate;
  initialiseOscillators();

};
//=========================================================================
void WavetableSynth::processBlock(juce::AudioBuffer<float>& buffer,
                                  juce::MidiBuffer& midiMessages){

  auto currentSample = 0;
  // Loop over midi messages
  for (const auto midiMessage : midiMessages)
  {
    const auto midiEvent = midiMessage.getMessage();
    // need to know when the midi event happened as an int
    const auto midiEventSample = static_cast<int>(midiEvent.getTimeStamp());

    // render the sound in the specified interval
    render(buffer, currentSample, midiEventSample);

    // process the midiEvent
    handleMidiEvent(midiEvent);

    // update the current sample
    currentSample = midiEventSample;
  }

  // process to the end of the block
  render(buffer, currentSample, buffer.getNumSamples());
};

//=========================================================================
void WavetableSynth::initialiseOscillators() {
  constexpr auto OSCILLATOR_COUNT = 128; // one oscillator per midi note
  const auto waveTable = generateSineWaveTable();

  // clear them first just incase
  oscillators.clear();

  for (auto oscillatorIndex = 0; oscillatorIndex < OSCILLATOR_COUNT;
       ++oscillatorIndex) {
    oscillators.emplace_back(waveTable, sampleRate);
  }
}

//=========================================================================
void WavetableSynth::render(juce::AudioBuffer<float>& buffer,
                            int startSample,
                            int endSample) {
    // get pointer to first channel
  auto* firstChannel = buffer.getWritePointer(0);

  // iterate over oscillators:

  for (auto& oscillator : oscillators) {
      // ask if active
    if (oscillator.isPlaying()) {
        // give samples in a specificed range
      for (auto& sample = startSample; sample < endSample; ++sample) {
          firstChannel[sample] += oscillator.getSample();
      }
    }
  }

  for (auto channel = 1; channel < buffer.getNumChannels(); ++channel) {
    std::copy(firstChannel + startSample, firstChannel + endSample,
              buffer.getWritePointer(channel) + startSample);
  }
}

//=========================================================================
void WavetableSynth::handleMidiEvent(const juce::MidiMessage& midiEvent){
  if (midiEvent.isNoteOn()) {

    // convert midi to a note
    const auto oscillatorId = midiEvent.getNoteNumber();
    const auto frequency = midiNoteNumberToFrequency(oscillatorId);
    oscillators[oscillatorId].setFrequency(frequency);

  } else if (midiEvent.isNoteOff()) {
    const auto oscillatorId = midiEvent.getNoteNumber();
    oscillators[oscillatorId].stop();

  } else if (midiEvent.isAllNotesOff()) {

      for (auto& oscillator : oscillators) {
        oscillator.stop();
      }
  };
};

//=========================================================================
float WavetableSynth::midiNoteNumberToFrequency(int midiNoteNumber) {
    // System constants are upper case
  constexpr auto A4_FREQUENCY = 440.f;
  constexpr auto A4_NOTENUMBER = 69.f;
  constexpr auto SEMITONES_PER_OCTAVE = 12.f;
  return A4_FREQUENCY * std::powf(2.f, (midiNoteNumber - A4_NOTENUMBER) /
                                           SEMITONES_PER_OCTAVE);
}

//=========================================================================
std::vector<float> WavetableSynth::generateSineWaveTable() {

  constexpr auto WAVETABLE_LENGTH = 64;

  std::vector<float> sineWaveTable(WAVETABLE_LENGTH);

  const auto PI = std::atanf(1.f) * 4; // hehe

  // loop over the wavetable

  for (auto wavetableIndex = 0; wavetableIndex < WAVETABLE_LENGTH;
       ++wavetableIndex) {
  
      sineWaveTable[wavetableIndex] =
        std::sinf(2 * PI * static_cast<float>(wavetableIndex) /
                  static_cast<float>(WAVETABLE_LENGTH));
  }

  return sineWaveTable;

}

