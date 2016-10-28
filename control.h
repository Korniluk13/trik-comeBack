#pragma once

#include <trikControl/brickInterface.h>
#include <trikKernel/timeVal.h>
#include <trikControl/motorInterface.h>
#include <trikControl/encoderInterface.h>

#include <QtCore/QObject>
#include <QTimer>
#include "filter.h"

class Control : public QObject
{
    Q_OBJECT
public:

    Control();

public slots:

    void reverse();

    void control(double angle);

    void comeBack();

    void keyHadle(int x, int y);

    void makeKeyHandle();

    void startMove();

private:

    enum State {
        none
        , comingBack
        , reversion
    };

    trikControl::BrickInterface *mBrick;
    Filter *mFilter;

    trikControl::MotorInterface *mLeftMotor;
    trikControl::MotorInterface *mRightMotor;

    trikControl::EncoderInterface *mEncLeft;
    trikControl::EncoderInterface *mEncRight;

    State mState;

    double mAngle;
    double mGoalAngle;

    int mX;
    int mY;

    QTimer *mTimer;

    int mStraightDist;
};
