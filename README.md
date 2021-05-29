
## Cantina

### Info

Library for processing signal input through a vocoder controlled by MIDI Input 
(carrier frequency and velocity, modulated by MIDI controllers and envelopes).


### Build

#### State:

Working on Linux. Have issues linking to FFTW on Windows.

#### Dependencies:

	Pantoufle  (submodule)
	FFTW       (included)
	SoundTouch (included)

##### On Debian:

	sudo apt install libsndfile-dev vamp-plugin-sdk

### To do

#### Features

##### Pitch shifting

- [ ] Set pitch of pitch sifter outside of the apply() routine.
- [ ] Make it vary incrementally with a Slider.

##### Misc

- [ ] Output freq / volume info of seed as MIDI note?
- [ ] Move responsibility of MIDI tone <-> frequency conversion to Eurêka when ready.
- [ ] Add support of phase-based pitch shifting with RubberBand.
- [ ] Implement mixed pitch shifting for semi-static pitch shifting to harmonics.

#### Fixes

- [ ] Audio signal frets when starting and stopping note. Testing revealed that it was not obviously related to velocity
  changes.

###### ~ tut-tut-tut-tut-tulut-tut ~
