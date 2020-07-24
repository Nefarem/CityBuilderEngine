#ifndef ROAD_HPP
#define ROAD_HPP

#include "engine/element/static/Building.hpp"

class MapCoordinates;
class StaticElementInformation;

/**
 * @brief A road.
 *
 * Roads are very particular buildings. They constitute a road graph on wich the dynamic elements will move. Check
 * RoadGraph for more information.
 */
class Road : public Building
{
        Q_OBJECT

    public:
        Road(QObject* parent, const StaticElementInformation* conf, const MapCoordinates& coordinates);
};

#endif // ROAD_HPP
