#include "ProcessableBuilding.hpp"

#include "engine/element/static/behavior/AbstractActivityBehavior.hpp"
#include "engine/element/static/behavior/AbstractStaticElementBehavior.hpp"
#include "engine/element/static/behavior/AbstractWalkerGenerator.hpp"
#include "engine/element/static/behavior/BehaviorFactory.hpp"
#include "global/conf/StaticElementInformation.hpp"



ProcessableBuilding::ProcessableBuilding(
    QObject* parent,
    const BehaviorFactory* behaviorFactory,
    const StaticElementInformation* conf,
    const MapArea& area,
    const MapCoordinates& entryPoint
) :
    Building(parent, conf, area),
    behaviors(),
    entryPoint(entryPoint)
{
    for (auto behaviorConf : conf->getBehaviors()) {
        behaviors.append(behaviorFactory->generate(this, behaviorConf));
    }
}



const MapCoordinates& ProcessableBuilding::getEntryPoint() const
{
    return entryPoint;
}



void ProcessableBuilding::init(const CycleDate& date)
{
    for (auto behavior : behaviors) {
        behavior->init(date);
        auto activityBehavior(dynamic_cast<AbstractActivityBehavior*>(behavior));
        if (activityBehavior) {
            activityBehavior->setActivitySpeedRatio(1.0, date);
        }
    }
}



void ProcessableBuilding::process(const CycleDate& date)
{
    for (auto behavior : behaviors) {
        behavior->process(date);
    }
}



bool ProcessableBuilding::processInteraction(const CycleDate& date, Character* actor)
{
    for (auto behavior : behaviors) {
        if (behavior->processInteraction(date, actor)) {
            return true;
        }
    }

    return false;
}



void ProcessableBuilding::notifyWalkerDestruction()
{
    for (auto behavior : behaviors) {
        auto walkerBehavior(dynamic_cast<AbstractWalkerGenerator*>(behavior));
        if (walkerBehavior) {
            walkerBehavior->clean();
        }
    }
}



const QList<AbstractStaticElementBehavior*>& ProcessableBuilding::getBehaviors() const
{
    return behaviors;
}
