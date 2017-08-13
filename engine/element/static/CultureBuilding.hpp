#ifndef CULTUREBUILDING_HPP
#define CULTUREBUILDING_HPP

#include "engine/element/static/ServiceBuilding.hpp"
#include "engine/element/TargetedWalkerPool.hpp"

class CultureBuilding : public ServiceBuilding
{
        Q_OBJECT

    private:
        TargetedWalkerPool* targetedWalkers;

    public:
        CultureBuilding(
            QObject* parent,
            const SearchEngine* searchEngine,
            const StaticElementInformation* conf,
            const MapArea& area,
            const MapCoordinates& entryPoint
        );

        virtual void init(const CycleDate& date) override;

        virtual void process(const CycleDate& date) override;

        virtual bool processInteraction(const CycleDate& date, AbstractDynamicMapElement* actor) override;

        virtual void notifyWalkerDestruction(const CycleDate& date) override;

    private slots:
        void requestTargetedWalkerCreation(
            const DynamicElementInformation* elementConf,
            std::function<void(AbstractDynamicMapElement*)> afterCreation
        );
};

#endif // CULTUREBUILDING_HPP
