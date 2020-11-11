#ifndef MAP_HPP
#define MAP_HPP

#include <functional>
#include <list>
#include <QtCore/QObject>
#include <QtCore/QSize>

#include "src/engine/map/path/MapDetailsInterface.hpp"
#include "src/engine/map/path/PathGenerator.hpp"
#include "src/engine/map/ElementHandler.hpp"
#include "src/engine/map/MapEntryPoint.hpp"
#include "src/engine/map/MapDetailsCache.hpp"
#include "src/engine/map/MapSearchEngine.hpp"
#include "src/viewer/construction/AreaCheckerInterface.hpp"
#include "src/viewer/construction/RoadPathGeneratorInterface.hpp"

class Building;
class BuildingInformation;
class Character;
class CharacterInformation;
class CityStatus;
class Conf;
class CycleDate;
class ImmigranteGeneratorInterface;
class MapArea;
class MapCoordinates;
class MapLoader;
class NatureElement;
class NatureElementInformation;
class ProcessableBuilding;
class TimeCycleProcessor;

class Map : public QObject, public MapDetailsInterface, public AreaCheckerInterface, public RoadPathGeneratorInterface
{
        Q_OBJECT

    private:
        const Conf* conf;
        QSize size;
        CityStatus* cityStatus;
        TimeCycleProcessor* processor;
        MapDetailsCache mapDetailsCache;
        PathGenerator pathGenerator;
        MapSearchEngine searchEngine;
        ElementHandler elementHandler;
        MapEntryPoint entryPoint;

    public:
        Map(const Conf* conf, const MapLoader& loader);

        /**
         * @brief Return the size of the map in terms on tiles.
         */
        const QSize& getSize() const;

        /**
         * @brief Indicate if the given coordinates are valid
         *
         * The coordinates are valid if they point to an existing cell in the map.
         */
        bool isValidCoordinates(const MapCoordinates& coordinates) const;

        /**
         * @brief Indicate if the given area is valid.
         *
         * The area is valid if it fits on the map.
         */
        bool isValidArea(const MapArea& area) const;

        /**
         * @brief Indicate if the given coordinates are free of any king of static element.
         */
        bool isConstructible(const MapCoordinates& coordinates) const;

        /**
         * @brief Indicate if the given area is free of any kind of static element.
         */
        virtual bool isConstructible(const MapArea& area) const override;

        MapCoordinates getBestEntryPoint(const MapArea& area) const;

        /**
         * @brief Return a const reference to the time cycle processor.
         */
        const TimeCycleProcessor* getProcessor() const;

        /**
         * @brief Return the list of all buildings.
         */
        const std::list<QSharedPointer<Building>>& getBuildings() const;

        /**
         * @brief Return the list of all characters.
         */
        const std::list<QSharedPointer<Character>>& getCharacters() const;

        /**
         * @brief Return the list of all nature elements.
         */
        const std::list<NatureElement*>& getNatureElements() const;

        /**
         * @brief Return the map immigrant generator.
         */
        ImmigrantGeneratorInterface& getImmigrantGenerator();

        /**
         * @brief Get current budget.
         */
        int getCurrentBudget() const;

        /**
         * @brief Get the current population.
         */
        int getCurrentPopulation() const;

        /**
         * @brief Get the current date for display purpose.
         */
        const CycleDate& getCurrentDate() const;

        virtual bool isLocationTraversable(const MapCoordinates& location) const override;

        virtual bool hasRoadAtLocation(const MapCoordinates& location) const override;

        virtual bool canConstructRoadAtLocation(const MapCoordinates& location) const override;

        virtual QList<MapCoordinates> getShortestPathForRoad(
            const MapCoordinates& origin,
            const MapCoordinates& target
        ) const override;

    public slots:
        /**
         * @brief Set (or unset) the pause mode.
         */
        void pause(const bool pause = true);

        /**
         * @brief Set the time-cycle processor speed ratio.
         */
        void setProcessorSpeedRatio(const qreal speedRatio);

        /**
         * @brief Create a building on the map.
         *
         * If the area is not free, the element is not created. For now, we just log with qDebug(). But this fail should
         * be kept silent because it could eventually append in real condition when a static element creation request
         * append while the area get "unfree" during the last time cycle process.
         *
         * @param conf The conf for the new building to create.
         * @param area The location of the element on the map.
         * @throw UnexpectedException Try to create a static element of type None.
         */
        void createBuilding(const BuildingInformation& conf, const MapArea& area);

        /**
         * @brief Update the total population of the given delta.
         */
        void changePopulation(const int populationDelta);

    signals:
        void buildingCreated(QSharedPointer<const Building> building);
        void characterCreated(QSharedPointer<const Character> character);
        void natureElementCreated(NatureElement& natureElement);
        void budgetChanged(const int budget);
        void populationChanged(const int population);
        void dateChanged(const int year, const int month);
};

#endif // MAP_HPP
