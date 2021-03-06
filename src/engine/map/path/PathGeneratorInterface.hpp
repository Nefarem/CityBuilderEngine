#ifndef PATHGENERATORINTERFACE_HPP
#define PATHGENERATORINTERFACE_HPP

#include <functional>
#include <QtCore/QSharedPointer>
#include <QtCore/QWeakPointer>

class AbstractStaticElement;
class PathInterface;
class Tile;

using TargetFetcher = std::function<QWeakPointer<AbstractStaticElement>(const Tile&)>;

class PathGeneratorInterface
{
    public:
        virtual ~PathGeneratorInterface() {};

        /**
         * @brief Generate a path for a wandering character.
         */
        virtual QSharedPointer<PathInterface> generateWanderingPath(
            const Tile& origin,
            const int wanderingCredits
        ) const = 0;

        /**
         * @brief Generate the shortest path from origin to target.
         */
        virtual QSharedPointer<PathInterface> generateShortestPathTo(
            const Tile& origin,
            const Tile& destination
        ) const = 0;

        /**
         * @brief Generate the shortest path from origin to target by using roads only.
         */
        virtual QSharedPointer<PathInterface> generateShortestRoadPathTo(
            const Tile& origin,
            const Tile& destination
        ) const = 0;

        /**
         * @brief Generate the shortest path from origin to targe, prefering roads if available.
         *
         * If the `restrictedToRoads` attribute is `true` and no road path is found, then no path will be returned.
         */
        virtual QSharedPointer<PathInterface> generatePreferedShortestPathTo(
            const Tile& origin,
            const Tile& destination,
            bool restrictedToRoads
        ) const = 0;

        /**
         * @brief Generate shortest path to the closest location having a target by using the given target fetcher.
         */
        virtual QSharedPointer<PathInterface> generateShortestPathToClosestMatch(
            const Tile& origin,
            TargetFetcher getTarget
        ) const = 0;
};

#endif // PATHGENERATORINTERFACE_HPP
