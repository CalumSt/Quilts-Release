Release is a wavetable bass synthesiser, inspired by various Moog synths but more digital processing.

This current build is very barebones, only having the wavetable core.

If you want to build it and test it for yourself, all you need to do is clone the repo, then run the following two commands:

```
cmake -S . -B build 
cmake --build build
```

Or, you can run the first command, and open the generated solution in your IDE of choice, and compile it through that.

Current issues:
  - 'Clicking' on new midi input. This will require a small gain attenuation at the start of each sample.
  - Mono only (Left channel). I'm unsure exactly why this is happening. I believe it's the loop over the first channel and then a separate loop over every remaining channel causing difficulty.
  - No GUI.

Features to add:
  - Full synthesis chain (Envelopes, Gain, Noise, Detune, etc)
  - Ladder filter
  - GUI
  - More waves
  - Restructure Voices - as this is a monophonic synth, we only need a small amount of voices and to be able to put notes into that, rather than having a voice per note.
  - Unit testing
