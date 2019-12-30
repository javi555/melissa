
#ifndef WAYPOINT_ZMQ_SERIALIZER_H
#define WAYPOINT_ZMQ_SERIALIZER_H

#include <cereal/cereal.hpp>

#include <cereal/types/vector.hpp>

#include "waypoint.h"

namespace cereal {
template<class Archive>
void serialize(Archive & archive, mls::Waypoint & event)
{
    archive(CEREAL_NVP(event.seq),
            CEREAL_NVP(event.x),
            CEREAL_NVP(event.y),
            CEREAL_NVP(event.z));
}
}
#endif // WAYPOINT_ZMQ_SERIALIZER_H
