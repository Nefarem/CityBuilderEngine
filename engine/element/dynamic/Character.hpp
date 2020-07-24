#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <QtCore/QObject>
#include <QtCore/QPointer>

#include "engine/element/static/ProcessableBuilding.hpp"
#include "engine/map/MapCoordinates.hpp"
#include "engine/processing/AbstractProcessable.hpp"

class CycleDate;
class CharacterInformation;
class Map;
class MotionHandler;

/**
 * @brief A character on the map.
 *
 * A character is a dynamic element that can move (e.g. change its coordinates through time). It do not have any size,
 * so they do not cover any area. They are just represented by some coordinates and should be perceived as mathematical
 * points.
 *
 * Characters are always issued from a building. This building (the `issuer`) does not strictly own the character:
 * all the characters belong to the map. Furthermore, a charater can be kept alive even if the issuer has been
 * destroyed.
 *
 * If a character is granted wandering credits, it will use them to wander around (see MotionHandler for more details).
 * Otherwise, the character won't move until a traget is assigned to it.
 */
class Character : public QObject, public AbstractProcessable
{
        Q_OBJECT

    private:
        const CharacterInformation* conf;
        QPointer<ProcessableBuilding> issuer;///< The issuer building.
        QPointer<ProcessableBuilding> target;///< The target building.
        MotionHandler* motionHandler;///< A helper that will handle character's motion.

    public:
        Character(
            QObject* parent,
            const Map* map,
            const CharacterInformation* conf,
            ProcessableBuilding* issuer,
            int wanderingCredits = 0
        );

        void assignTarget(ProcessableBuilding* target);

        const CharacterInformation* getConf() const;

        /**
         * @brief The current location of the character.
         */
        const MapCoordinates& getCurrentLocation() const;

        /**
         * @brief Get the issuer.
         */
        ProcessableBuilding* getIssuer() const;

        /**
         * @brief Make the charater move.
         */
        virtual void process(const CycleDate& date) override;
};

#endif // CHARACTER_HPP