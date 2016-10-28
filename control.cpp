#include "control.h"

#include <trikControl/brickFactory.h>
#include <trikControl/keysInterface.h>
#include <QDebug>
#include <qmath.h>

Control::Control()
{
    mBrick = trikControl::BrickFactory::create();
    mFilter = new Filter(mBrick, 500);

    mLeftMotor = mBrick->motor("M1");
    mRightMotor = mBrick->motor("M3");

    mEncLeft = mBrick->encoder("E1");
    mEncRight = mBrick->encoder("E3");

    mEncLeft->reset();
    mEncRight->reset();

    mState = none;

    mAngle = 0;
    mGoalAngle = 0;

    mX = 0;
    mY = 0;
    mStraightDist = 0;

    QObject::connect(mFilter, SIGNAL(navigationReady(double)), this, SLOT(control(double)));
    QObject::connect(mFilter, SIGNAL(inited()), this, SLOT(startMove()));
    QObject::connect(mBrick->keys(), SIGNAL(buttonPressed(int,int)),
                     this, SLOT(keyHadle(int,int)));
}

void Control::reverse()
{
    const double error = - mGoalAngle + mAngle * 180 / M_PI;

    if (abs(error) > 0.5) {

        mLeftMotor->setPower(-60);
        mRightMotor->setPower(60);

    } else {

        mState = State::comingBack;

        mX = 0;
        mY = 0;

        mLeftMotor->setPower(0);
        mRightMotor->setPower(0);
   }

}

void Control::control(double angle)
{
    mAngle = angle;

    const int left = mEncLeft->readRawData() ;
    const int right = -mEncRight->readRawData();

    mEncLeft->reset();
    mEncRight->reset();

    const int dc = (left + right) / 2;

    mX += dc * qCos(mAngle);
    mY += dc * qSin(mAngle);

//    qDebug() << mX << " " << mY << " " << left << " " << right << " " << dc;

    switch (mState) {
    case State::reversion: {
        reverse();
        break;
    }
    case State::comingBack:
        comeBack();
        break;
    default:
        break;
    }
}

void Control::comeBack()
{
    if (abs(sqrt(mX * mX + mY * mY) - mStraightDist) > 100) {

        qDebug() << mX << " " << mY;

        mLeftMotor->setPower(70);
        mRightMotor->setPower(69);

    } else {

        mState = State::none;
        mLeftMotor->setPower(0);
                mRightMotor->setPower(0);
    }
}

void Control::keyHadle(int x, int y)
{
    if (x == 103 && y == 0) {

        mState = State::reversion;

        mStraightDist = sqrt(mX * mX + mY * mY);

        mGoalAngle = qAtan2(mY, mX) * 180 / M_PI;

        qDebug() << mGoalAngle << " " << mStraightDist;

    } else if (x == 116) {

        mLeftMotor->setPower(0);
        mRightMotor->setPower(0);
    }
}

void Control::makeKeyHandle()
{
    keyHadle(103, 0);
    mTimer->stop();
    mLeftMotor->setPower(0);
    mRightMotor->setPower(0);
}

void Control::startMove()
{

    mTimer = new QTimer;
    mTimer->start(13000);
    mLeftMotor->setPower(-70);
    mRightMotor->setPower(-80);

    QObject::connect(mTimer, SIGNAL(timeout()),
                     this, SLOT(makeKeyHandle()));
}
