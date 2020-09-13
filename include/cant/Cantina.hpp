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
                              size_u sampleRate,
                              pan::id_u8 channelId=0
        );
        CANT_NODISCARD size_u getNumberHarmonics() const;
        void setController(const std::string &type, pan::id_u8 channel, const Stream <pan::id_u8> &controllerIds);
        void receiveNote(const pan::MidiNoteInputData& noteData);
        void receiveControl(const pan::MidiControlData &controlData);


        /* @brief: assumes numberHarmonics() elements in outVoices */
        void perform(const sample_f* in, sample_f* outSeed, sample_f** outHarmonics, size_u blockSize);
    private:
        /** -- methods -- **/
        void update(const sample_f *in, size_u blockSize);

        /** -- fields -- **/
        UPtr<pan::Pantoufle> m_pantoufle;
        UPtr<track::PitchTracker> m_tracker;
        /*
         * Cantina needs an unconstrained shifter for its main function,
         * so a mixed-domain pitch-shifter can't be used.
         */
        UPtr<shift::TimeDomainPitchShifter> m_shifter;

    };

}


#include <cant/common/undef_macro.hpp>
#endif //LIB_CANTINA_HPP
