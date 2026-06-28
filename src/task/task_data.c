/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Create task data from data and clearer
*/
#include "prophecy.h"

inline prTaskData prTask_data(
    void *data,
    prClearer clearer
)
{
    return (prTaskData){
        .data = data,
        .clearer = clearer,
    };
}
