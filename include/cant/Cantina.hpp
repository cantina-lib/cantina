//
// Created by piptouque on 23/04/2020.
//

#ifndef LIB_CANTINA_HPP
#define LIB_CANTINA_HPP

#pragma once

#include <cant/common/config.hpp>
#include <cant/common/memory.hpp>
#include <cant/common/types.hpp>

#include <cant/pan/common/types.hpp>

#include <cant/pan/Pantoufle.hpp>
#include <cant/track/PitchTracker.hpp>
#include <cant/shift/TimeDomainPitchShifter.hpp>


namespace cant
{

    class Cantina
    {
    private:
        UPtr<pan::Pantoufle> _pan;
        UPtr<track::PitchTracker> _tracker;
        /*
         * Cantina needs an unconstrained shifter for its main function,
         * so a mixed-domain pitch-shifter can't be used.
         */
        UPtr<shift::TimeDomainPitchShifter> _shifter;

    private:
        void update(const sample_m *in, size_m blockSize);
    public:
        CANT_EXPLICIT Cantina(size_m numberHarmonics,
                              int_m sampleRate,
                              pan::byte_m channelId=0
                        );
        CANT_NODISCARD size_m getNumberHarmonics() const;
        void setController(const std::string &type, pan::byte_m channel, pan::byte_m controllerId);
        void receiveNote(size_m iVoice, const pan::MidiNoteInputData& noteData);
        void receiveControl(const pan::MidiControlData &controlData);


        /* @brief: assumes numberHarmonics() elements in outVoices */
        void perform(const sample_m* in, sample_m* outSeed, sample_m** outHarmonics, size_m blockSize);
    };

}


#endif //LIB_CANTINA_HPP
