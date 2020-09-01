//
// Created by binabik on 24/08/2020.
//

#ifndef CANTINA_TILDE_CANT_FORWARD_HPP
#define CANTINA_TILDE_CANT_FORWARD_HPP

#pragma once

namespace cant
{
    namespace track { class PitchTracker; }
    namespace shift { class TimeDomainPitchShifter; }

    namespace pan
    {
        class Pantoufle;
        class MidiControlData;
        template <typename T, typename V>
        class MidiNoteData;
        class MidiNoteOutput;
    }
}

#endif //CANTINA_TILDE_CANT_FORWARD_H
