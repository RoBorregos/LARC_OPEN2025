/*
 * @file trigger_list.hpp
 * @date 03/04/2025
 * @author Dana E. Torres Estrada
 *
 * @brief Defines the TriggerList enumeration, which represents different triggers for state transitions.
 */

#ifndef TRIGGERLIST_H
#define TRIGGERLIST_H

enum class TriggerList { //lo ideal seria hacer un list por cada sistema
    //para el sábado
    FINISH_SETUP,       // INIT -> CALC_TRACK
    CALCULATED_TRACK,    // CALC_TRACK -> FOLLOW_TRACK
    REACHED_TREE,       // FOLLOW_TRACK -> FETCH_BEAN
    BEANS_FOUND,        // FETCH_BEAN -> PICK_BEAN
    BEANS_STORED,       // PICK_BEAN -> CALC_TRACK
    REACHED_EXIT        // DROP_BEAN -> FINISH
};
//OBSTACLE va en nav
#endif // TRIGGERLIST_H
