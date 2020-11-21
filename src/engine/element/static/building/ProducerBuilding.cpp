#include "ProducerBuilding.hpp"

#include "src/engine/element/dynamic/character/Character.hpp"
#include "src/engine/element/dynamic/character/DeliveryManCharacter.hpp"
#include "src/engine/element/dynamic/character/MinerCharacter.hpp"
#include "src/engine/element/dynamic/CharacterFactoryInterface.hpp"
#include "src/engine/element/static/natureElement/NatureElementSearchEngine.hpp"
#include "src/engine/element/static/path/PathInterface.hpp"
#include "src/engine/state/BuildingState.hpp"
#include "src/global/conf/BuildingInformation.hpp"
#include "src/global/pointer/SmartPointerUtils.hpp"



ProducerBuilding::ProducerBuilding(
    const NatureElementSearchEngine& searchEngine,
    CharacterFactoryInterface& characterFactory,
    const BuildingInformation& conf,
    const MapArea& area,
    const MapCoordinates& entryPoint
) :
    AbstractProcessableBuilding(conf, area, entryPoint),
    searchEngine(searchEngine),
    characterFactory(characterFactory),
    miners(),
    nextMinerGenerationDate(),
    rawMaterialStock(0),
    deliveryMan()
{

}



QSharedPointer<AbstractProcessableBuilding> ProducerBuilding::Create(
    const NatureElementSearchEngine& searchEngine,
    CharacterFactoryInterface& characterFactory,
    const BuildingInformation& conf,
    const MapArea& area,
    const MapCoordinates& entryPoint
) {
    auto producer(new ProducerBuilding(searchEngine, characterFactory, conf, area, entryPoint));
    QSharedPointer<AbstractProcessableBuilding> pointer(producer);
    producer->selfReference = pointer;

    return pointer;
}



void ProducerBuilding::init(const CycleDate& date)
{
    if (canGenerateNewMiner()) {
        setupNextMinerGenerationDate(date);
    }
}



void ProducerBuilding::process(const CycleDate& date)
{
    cleanInvalids(miners);
    handleMinerGeneration(date);
    handleProduction();
}



bool ProducerBuilding::processInteraction(const CycleDate& /*date*/, Character& actor)
{
    if (matches(deliveryMan, actor)) {
        deliveryMan.clear();

        return true;
    }

    if (miners.contains(&actor)) {
        rawMaterialStock += conf.getProducerConf().miningQuantity;
        miners.remove(&actor);

        return true;
    }

    return false;
}



BuildingState ProducerBuilding::getCurrentState() const
{
    return BuildingState::CreateProducerState(reinterpret_cast<qintptr>(this), conf, area, stateVersion, rawMaterialStock);
}



void ProducerBuilding::handleMinerGeneration(const CycleDate& date)
{
    if (nextMinerGenerationDate) {
        if (date < nextMinerGenerationDate) {
            return;
        }

        auto path(searchEngine.getPathToClosestRawMaterial(conf.getProducerConf().rawMaterialConf, getEntryPoint()));
        if (!path) {
            // We don't have any target free for now.
            setupNextMinerGenerationDate(date);
            return;
        }

        auto miner(characterFactory.generateMiner(conf.getProducerConf().miner.conf, selfReference, path));
        miners.insert(miner.toStrongRef().get(), miner);

        if (canGenerateNewMiner()) {
            setupNextMinerGenerationDate(date);
        }
        else {
            nextMinerGenerationDate.reset();
        }
    }
    else if (canGenerateNewMiner()) {
        setupNextMinerGenerationDate(date);
    }
}



bool ProducerBuilding::canGenerateNewMiner() const
{
    return miners.size() < conf.getProducerConf().miner.maxSimultaneous;
}



void ProducerBuilding::setupNextMinerGenerationDate(const CycleDate& date)
{
    nextMinerGenerationDate.reassign(date, conf.getProducerConf().miner.generationInterval);
}



void ProducerBuilding::handleProduction()
{
    if (!deliveryMan && rawMaterialStock >= conf.getProducerConf().rawMaterialQuantityToProduce) {
        deliveryMan = characterFactory.generateDeliveryMan(
            conf.getProducerConf().deliveryManConf,
            selfReference,
            conf.getProducerConf().producedItemConf,
            1
        );
        rawMaterialStock -= conf.getProducerConf().rawMaterialQuantityToProduce;
    }
}
