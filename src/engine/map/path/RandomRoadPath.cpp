#include "RandomRoadPath.hpp"

#include "src/engine/map/path/MapDetailsInterface.hpp"
#include "src/engine/random.hpp"



RandomRoadPath::RandomRoadPath(const MapDetailsInterface& mapDetails, const MapCoordinates& initialLocation) :
    mapDetails(mapDetails),
    previousLocation(),
    currentLocation(initialLocation.getRounded())
{

}



MapCoordinates RandomRoadPath::getNextTargetCoordinates()
{
    QList<MapCoordinates> neighbours({
        currentLocation.getNorth(),
        currentLocation.getEast(),
        currentLocation.getSouth(),
        currentLocation.getWest(),
    });
    QList<MapCoordinates> roadNeighbours;
    for (auto neighbour : neighbours) {
        if (mapDetails.isLocationARoad(neighbour)) {
            roadNeighbours.append(neighbour);
        }
    }

    if (roadNeighbours.size() == 0) {
        // Character is on a single road tile.
        return {};
    }
    if (roadNeighbours.size() == 1) {
        // Dead end, must go back.
        return roadNeighbours.first();
    }

    // We remove the previous node to avoid going back on a bifurcation.
    if (previousLocation.isValid()) {
        roadNeighbours.removeOne(previousLocation);
    }

    if (roadNeighbours.size() == 1) {
        // Only one choice left
        return roadNeighbours.first();
    }

    // Choose random.
    previousLocation = currentLocation;
    currentLocation = roadNeighbours.at(randomInt(0, roadNeighbours.size() - 1));

    return currentLocation;
}
