#ifndef TARGETEDPATH_HPP
#define TARGETEDPATH_HPP

#include <QtCore/QList>

#include "src/engine/element/static/path/PathInterface.hpp"
#include "src/engine/map/MapCoordinates.hpp"

class MapDetailsInterface;

class TargetedPath : public PathInterface
{
    private:
        const MapDetailsInterface& mapDetails;
        const bool restrictedToRoads;
        QList<MapCoordinates> path;
        bool obsolete;

    public:
        TargetedPath(
            const MapDetailsInterface& mapDetails,
            const bool restrictedToRoads,
            const QList<MapCoordinates>& path
        );

        virtual bool isObsolete() const override;

        virtual bool isCompleted() const override;

        virtual MapCoordinates getNextTargetCoordinates() override;

        const QList<MapCoordinates>& toCoordinatesList() const;
};

#endif // TARGETEDPATH_HPP