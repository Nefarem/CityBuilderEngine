#include "StudentCharacter.hpp"

#include "src/engine/element/dynamic/CharacterManagerInterface.hpp"
#include "src/engine/element/static/building/AbstractProcessableBuilding.hpp"



StudentCharacter::StudentCharacter(
    CharacterManagerInterface& characterManager,
    const PathGeneratorInterface& pathGenerator,
    const CharacterInformation& conf,
    const QSharedPointer<AbstractProcessableBuilding>& issuer,
    const QWeakPointer<AbstractProcessableBuilding>& target,
    owner<PathInterface*> path
) :
    Character(characterManager, pathGenerator, conf, issuer),
    target(target)
{
    motionHandler.takePath(path);
}



void StudentCharacter::process(const CycleDate& date)
{
    Character::process(date);

    if (motionHandler.isPathCompleted()) {
        auto target(this->target.toStrongRef());
        if (target) {
            target->processInteraction(date, *this);
        }
        characterManager.clearCharacter(*this);
    }
}
