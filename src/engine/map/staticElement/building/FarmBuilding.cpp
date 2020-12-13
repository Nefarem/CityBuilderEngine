#include "FarmBuilding.hpp"

#include <QtCore/QtGlobal>

#include "src/engine/map/dynamicElement/character/DeliveryManCharacter.hpp"
#include "src/engine/map/dynamicElement/CharacterGeneratorInterface.hpp"
#include "src/engine/processing/CycleDate.hpp"
#include "src/global/conf/BuildingInformation.hpp"
#include "src/global/pointer/SmartPointerUtils.hpp"
#include "src/global/state/BuildingState.hpp"



FarmBuilding::FarmBuilding(
    CharacterGeneratorInterface& characterFactory,
    const BuildingInformation& conf,
    const MapArea& area,
    Direction orientation,
    const MapCoordinates& entryPoint
) :
    AbstractProcessableBuilding(conf, area, orientation, entryPoint),
    GROWING_INTERVAL(0.9 * (conf.getMaxWorkers() * CycleDate::getBuildingCyclesPerYear())),
    characterFactory(characterFactory),
    growingCountDown(GROWING_INTERVAL),
    deliveryMan()
{

}



QSharedPointer<AbstractProcessableBuilding> FarmBuilding::Create(
    CharacterGeneratorInterface& characterFactory,
    const BuildingInformation& conf,
    const MapArea& area,
    Direction orientation,
    const MapCoordinates& entryPoint
) {
    auto farm(new FarmBuilding(characterFactory, conf, area, orientation, entryPoint));
    QSharedPointer<AbstractProcessableBuilding> pointer(farm);
    farm->selfReference = pointer;

    return pointer;
}



void FarmBuilding::process(const CycleDate& date)
{
    if (!isActive()) {
        return;
    }

    if (growingCountDown > 0) {
        growingCountDown -= getCurrentWorkerQuantity();
        notifyViewDataChange();
    }

    if (date.isFirstCycleOfMonth() && date.getMonth() == conf.getFarmConf().harvestMonth) {
        auto deliveryMan(this->deliveryMan.toStrongRef());
        if (deliveryMan) {
            // The delivery man is outside. We do not harvest now.
            // Next harvest will occure as soon as the growing is complete and thedelivery man is back.
            return;
        }

        harvest();
    }
    else if (growingCountDown <= 0) {
        // Case where the delivery man were outside at the begining of the harvest month.
        auto deliveryMan(this->deliveryMan.toStrongRef());
        if (deliveryMan) {
            // The delivery man is still outside. We do not harvest now.
            return;
        }

        harvest();
    }
}



bool FarmBuilding::processInteraction(const CycleDate& /*date*/, Character& actor)
{
    if (matches(deliveryMan, actor)) {
        deliveryMan.clear();

        return true;
    }

    return false;
}



BuildingState FarmBuilding::getCurrentState() const
{
    return BuildingState::CreateFarmState(
        reinterpret_cast<qintptr>(this),
        conf,
        area,
        orientation,
        getCurrentStatus(),
        getCurrentWorkerQuantity(),
        stateVersion,
        static_cast<int>(100.0 * getGrowthRatio())
    );
}



qreal FarmBuilding::getGrowthRatio() const
{
    return qMin(
        1.0,
        static_cast<qreal>(GROWING_INTERVAL - growingCountDown) / static_cast<qreal>(GROWING_INTERVAL)
    );
}



void FarmBuilding::harvest()
{
    qreal productivityRatio(getGrowthRatio());
    int quantity(productivityRatio * conf.getFarmConf().maxQuantityHarvested);

    deliveryMan = characterFactory.generateDeliveryMan(
        conf.getFarmConf().deliveryManConf,
        selfReference,
        conf.getFarmConf().producedItemConf,
        quantity
    );

    growingCountDown = GROWING_INTERVAL;
    notifyViewDataChange();
}
