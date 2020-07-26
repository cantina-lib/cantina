//
// Created by piptouque on 23/04/2020.
//

#ifndef LIB_CANTINA_HPP
#define LIB_CANTINA_HPP

#pragma once

#include <cant/common/config.hpp>
#include <cant/common/memory.hpp>
#include <cant/common/types.hpp>

#include <cant/pan/pan.hpp>
#include <cant/track/track.hpp>
#include <cant/shift/shift.hpp>

namespace cant
{
    class Cantina
    {
    private:
        UPtr<midi::MidiMachine> _mach;
        UPtr<track::PitchTracker> _tracker;
        /*
         * Cantina needs an unconstrained shifter for its main function,
         * so a mixed-domain pitch-shifter can't be used.
         */
        UPtr<shift::TimeDomainPitchShifter> _shifter;

    private:
        void update(const sample_m *in, sizeint blockSize);
    public:
        explicit Cantina(sizeint numberHarmonics,
                int_m sampleRate,
                midi::byte_m channelId=0
                        );
        CANT_NODISCARD sizeint getNumberHarmonics() const;
        void setController(const std::string &type, midi::byte_m channelId, midi::byte_m controllerId);
        void receiveNote(sizeint iVoice, const midi::MidiNoteInputData& noteData);
        void receiveControl(const midi::MidiControlData &controlData);


        /* @brief: assumes numberHarmonics() elements in outVoices */
        void perform(const sample_m* in, sample_m* outSeed, sample_m** outHarmonics, sizeint blockSize);
    };

}


#endif //LIB_CANTINA_HPP
