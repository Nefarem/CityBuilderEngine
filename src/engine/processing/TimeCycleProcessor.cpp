#include "TimeCycleProcessor.hpp"

#include "src/engine/map/dynamicElement/character/Character.hpp"
#include "src/engine/map/staticElement/building/AbstractProcessableBuilding.hpp"
#include "src/global/conf/BuildingInformation.hpp"
#include "src/defines.hpp"

#ifdef DEBUG_TOOLS
#include <QtCore/QElapsedTimer>
#endif
#include <QDebug>

#include "src/defines.hpp"

const qreal MSEC_PER_SEC(1000);



TimeCycleProcessor::TimeCycleProcessor(const CycleDate& startingDate,
    const qreal speedRatio
) :
    QObject(),
    paused(true),
    speedRatio(speedRatio),
    clock(),
    currentCycleDate(startingDate)
{
    if (speedRatio < 0.1) {
        this->speedRatio = 0.1;
    }
    if (speedRatio > 2.0) {
        this->speedRatio = 2.0;
    }
    clock.start(MSEC_PER_SEC / (CYCLES_PER_SECOND * this->speedRatio), this);
}



qreal TimeCycleProcessor::getSpeedRatio() const
{
    return speedRatio;
}



const CycleDate& TimeCycleProcessor::getCurrentDate() const
{
    return currentCycleDate;
}



void TimeCycleProcessor::registerProcessable(AbstractProcessable& processable)
{
    processableElements.append(&processable);
}



void TimeCycleProcessor::pause(const bool pause)
{
    if (pause != paused) {
        paused = pause;
        if (pause) {
            clock.stop();
        } else {
            clock.start(MSEC_PER_SEC / (CYCLES_PER_SECOND * speedRatio), this);
        }
    }
}



void TimeCycleProcessor::setSpeedRatio(const qreal ratio)
{
    if (ratio != speedRatio && ratio >= 0.1 && ratio <= 2.0) {
        speedRatio = ratio;

        if (!paused) {
            // Re-launch the timer with the new speed.
            clock.stop();
            clock.start(MSEC_PER_SEC / (CYCLES_PER_SECOND * ratio), this);
        }
    }
}



void TimeCycleProcessor::forceNextProcess()
{
    if (paused) {
        processCycle();
    }
}



void TimeCycleProcessor::timerEvent(QTimerEvent* /*event*/)
{
    processCycle();
}



void TimeCycleProcessor::processCycle()
{
#ifdef DEBUG_TOOLS
    QElapsedTimer timer;
    timer.start();
#endif

    // Increment to cycle date.
    ++currentCycleDate;
    // qDebug() << "Process time-cycle" << currentCycleDate.toString();

    for (auto processableElement : processableElements) {
        processableElement->process(currentCycleDate);
    }

#ifdef DEBUG_TOOLS
    if (timer.hasExpired(10)) {
        qDebug() << "********** Long process cycle detected: took " << timer.elapsed() << "ms. **********";
    }
#endif

    emit processFinished();
}
