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
        CANT_EXPLICIT Cantina(size_u numberHarmonics,
                              type_i sampleRate,
                              pan::id_u8 channelId=0
        );
        CANT_NODISCARD size_u getNumberHarmonics() const;
        void setController(const std::string &type, pan::id_u8 channel, const Stream <pan::id_u8> &controllerIds);
        void receiveNote(const pan::MidiNoteData<pan::tone_u8, pan::vel_u8>& noteData);
        void receiveControl(const pan::MidiControlData &controlData);


        /* @brief: assumes numberHarmonics() elements in outVoices */
        void perform(const sample_f* in, sample_f* outSeed, sample_f** outHarmonics, size_u blockSize);
    private:
        /** -- methods -- **/
        void update(const sample_f *in, size_u blockSize);

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
