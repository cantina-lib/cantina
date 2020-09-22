//
// Created by piptouque on 23/04/2020.
//

#ifndef CANTINA_CAN_CANTINA_HPP
#define CANTINA_CAN_CANTINA_HPP

#pragma once

#include <cant/common/memory.hpp>
#include <cant/common/types.hpp>
#include <cant/common/option.hpp>

#include <cant/pan/common/types.hpp>
#include <cant/time/common/types.hpp>

#include <cant/cant_forward.hpp>

#include <cant/common/macro.hpp>
CANTINA_CANT_NAMESPACE_BEGIN

    class Cantina
    {
    public:
        CANT_EXPLICIT Cantina(size_u numberHarmonics,
                              type_i sampleRate,
                              pan::id_u8 channel
        );

        /** -- methods -- **/
        void update();
        /* @brief: assumes numberHarmonics() elements in outVoices */
        void perform(const sample_f* in, sample_f* outSeed, sample_f** outHarmonics, size_u blockSize);

        void setController(
                const std::string &type,
                pan::id_u8 channel,
                const Stream <pan::id_u8> &controllerIds
                );
        void setCustomClock(time::AbsoluteTimeGetter currentTimeGetter);

        Optional <size_u> receiveNote(const pan::MidiNoteInputData& noteData);
        void              receiveControl(const pan::MidiControlData &controlData);

        CANT_NODISCARD const pan::MidiNoteOutput& getProcessedVoice(size_u voice) const;


        CANT_NODISCARD size_u getNumberHarmonics() const;
    private:

        /** -- fields -- **/
        UPtr<pan::Pantoufle> m_pantoufle;
        UPtr<PitchTracker> m_tracker;
        /*
         * Cantina needs an unconstrained shifter for its main function,
         * so a mixed-domain pitch-shifter can't be used.
         */
        UPtr<TimeDomainPitchShifter> m_shifter;
    };

CANTINA_CANT_NAMESPACE_END
#include <cant/common/undef_macro.hpp>

#endif //CANTINA_CAN_CANTINA_HPP
