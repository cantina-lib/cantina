//
// Created by piptouque on 23/04/2020.
//

#ifndef LIB_CANTINA_HPP
#define LIB_CANTINA_HPP

#pragma once

#include <cant/common/memory.hpp>
#include <cant/common/types.hpp>

#include <cant/pan/common/types.hpp>

#include <cant/cant_forward.hpp>

#include <cant/common/macro.hpp>
namespace cant
{


    class Cantina
    {
    public:
        CANT_EXPLICIT Cantina(size_m numberHarmonics,
                              int_m sampleRate,
                              pan::byte_m channelId=0
        );
        CANT_NODISCARD size_m getNumberHarmonics() const;
        void setController(const std::string &type, pan::byte_m channel, const Stream <pan::byte_m> &controllerIds);
        void receiveNote(const pan::MidiNoteData<pan::tone_mint, pan::vel_mint>& noteData);
        void receiveControl(const pan::MidiControlData &controlData);


        /* @brief: assumes numberHarmonics() elements in outVoices */
        void perform(const sample_m* in, sample_m* outSeed, sample_m** outHarmonics, size_m blockSize);
    private:
        /** -- methods -- **/
        void update(const sample_m *in, size_m blockSize);

        /** -- fields -- **/
        UPtr<pan::Pantoufle> _pantoufle;
        UPtr<track::PitchTracker> _tracker;
        /*
         * Cantina needs an unconstrained shifter for its main function,
         * so a mixed-domain pitch-shifter can't be used.
         */
        UPtr<shift::TimeDomainPitchShifter> _shifter;

    };

}


#include <cant/common/undef_macro.hpp>
#endif //LIB_CANTINA_HPP
