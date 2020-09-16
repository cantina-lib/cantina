//
// Created by binabik on 15/09/2020.
//

#ifndef CANTINA_TILDE_CANTINA_INL
#define CANTINA_TILDE_CANTINA_INL

#pragma once

#include <cant/pan/note/MidiNoteData.hpp>
#include <cant/pan/control/MidiControlData.hpp>

#include <cant/pan/Pantoufle.hpp>
#include <cant/track/PitchTracker.hpp>

#include <cant/common/CantinaException.hpp>

#include <cant/common/macro.hpp>
namespace cant
{

    CANT_INLINE
    void
    Cantina::
    update(const sample_f* in, const size_u blockSize)
    {

        CANTINA_TRY_RETHROW({
                m_tracker->update(in, blockSize);
                m_pantoufle->update();
        })
    }

    CANT_NODISCARD CANT_INLINE
    size_u
    Cantina::
    getNumberHarmonics() const
    {
        return m_pantoufle->getNumberVoices();
    }

    CANT_INLINE
    Optional<size_u>
    Cantina::
    receiveNote(const pan::MidiNoteInputData& noteData)
    {
        CANTINA_TRY_RETHROW({
                                    return m_pantoufle->receiveInputNoteData(noteData);
                            })
    }

    CANT_INLINE
    void
    Cantina::
    receiveControl(const pan::MidiControlInputData &controlData)
    {
        CANTINA_TRY_RETHROW({
                                    m_pantoufle->receiveRawControlData(controlData);
                            })
    }

    CANT_NODISCARD CANT_INLINE
    const pan::MidiNoteOutput&
    Cantina::
    getProcessedVoice(size_u voice) const
    {
        return m_pantoufle->getProcessedVoice(voice);
    }
}
#include <cant/common/undef_macro.hpp>

#endif //CANTINA_TILDE_CANTINA_INL
