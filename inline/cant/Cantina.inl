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
CANTINA_CANT_NAMESPACE_BEGIN

    CANT_INLINE
    void
    Cantina::
    update()
    {
        CANTINA_TRY_RETHROW({
                m_pantoufle->update();
        })
    }

    CANT_INLINE
    size_u
    Cantina::
    getNumberHarmonics() const
    {
        return m_pantoufle->getNumberVoices();
    }

    CANT_INLINE
    Optional<size_u>
    Cantina::
    receiveNote(const CANTINA_PAN_NAMESPACE::MidiNoteInputData& noteData)
    {
        CANTINA_TRY_RETHROW({
                                    return m_pantoufle->receiveInputNoteData(noteData);
                            })
    }

    CANT_INLINE
    void
    Cantina::
    receiveControl(const CANTINA_PAN_NAMESPACE::MidiControlInputData &controlData)
    {
        CANTINA_TRY_RETHROW({
                                    m_pantoufle->receiveRawControlData(controlData);
                            })
    }

    CANT_INLINE
    const CANTINA_PAN_NAMESPACE::MidiNoteOutput&
    Cantina::
    getProcessedVoice(size_u voice) const
    {
        return m_pantoufle->getProcessedVoice(voice);
    }

    CANT_INLINE
    void
    Cantina::
    setCurrentTimeGetter(CANTINA_PAN_NAMESPACE::MidiTimer::CurrentTimeGetter currentTimeGetter)
    {
        m_pantoufle->setCurrentTimeGetter(std::move(currentTimeGetter));
    }

CANTINA_CANT_NAMESPACE_END
#include <cant/common/undef_macro.hpp>

#endif //CANTINA_TILDE_CANTINA_INL
