/*
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#include "gtest/gtest.h"
#include "Coordinates.h"

TEST(CoordinatesTest, CoordinatesTest_TestAngle)
{
    Angle tmp;
    
    tmp.fromDegrees(0);
    EXPECT_EQ(tmp.inDegrees(), 0);

    tmp.fromDegrees(180);
    EXPECT_EQ(tmp.inDegrees(), 180);

    tmp.fromDegrees(360);
    EXPECT_EQ(tmp.inDegrees(), 360);

    tmp.fromDegrees(720);
    EXPECT_EQ(tmp.inDegrees(), 720);

    tmp.fromDegrees(-180);
    EXPECT_EQ(tmp.inDegrees(), -180);
    
    tmp.fromDegrees(-360);
    EXPECT_EQ(tmp.inDegrees(), -360);
    
    tmp.fromDegrees(-720);
    EXPECT_EQ(tmp.inDegrees(), -720);

    tmp.fromRadians(0);
    EXPECT_EQ(tmp.inRadians(), 0);

    tmp.fromRadians(BBC_PI);
    EXPECT_EQ(tmp.inRadians(), BBC_PI);

    tmp.fromRadians(2 * BBC_PI);
    EXPECT_EQ(tmp.inRadians(), 2 * BBC_PI);

    tmp.fromRadians(-BBC_PI);
    EXPECT_EQ(tmp.inRadians(), -BBC_PI);
    
    tmp.fromRadians(2 * -BBC_PI);
    EXPECT_EQ(tmp.inRadians(), 2 * -BBC_PI);
    
    tmp.fromDegrees(360);
    EXPECT_EQ(tmp.inRadians(), 2 * BBC_PI);

    Angle testConstructor(180, Angle::kDegrees);
    EXPECT_EQ(testConstructor.inDegrees(), 180);


    Angle smaller(0, Angle::kDegrees);
    Angle larger(180, Angle::kDegrees);
    
    EXPECT_EQ(smaller < larger, true);
    EXPECT_LT(smaller, larger);
    
    EXPECT_EQ(Angle(0, Angle::kDegrees), Angle(0, Angle::kDegrees));

    Angle addedValue = Angle(180, Angle::kDegrees) + Angle(180, Angle::kDegrees);
    EXPECT_EQ(addedValue.inDegrees(), 360);

}

class BoundAngle180To360 : public BoundAngle
{
public:
    BoundAngle180To360(float iVal, AngleType iType)
    : BoundAngle(
                 iVal
                 , iType
                 , BBC_PI
                 , (iType == kRadians) ? 2 * BBC_PI : 360
                 )
    {
    }
};

TEST(CoordinatesTest, CoordinatesTest_TestBoundAngle)
{
    BoundAngle tmp(0, Angle::kRadians, 0, BBC_PI);
    EXPECT_EQ(0, tmp.inRadians());

    BoundAngle tmp1(0, Angle::kRadians, 1, BBC_PI);
    EXPECT_EQ(1, tmp1.inRadians());

    BoundAngle180To360 tmp2(0, Angle::kRadians);
    EXPECT_EQ(180, tmp2.inDegrees());

    
    class IPersp3ctiveSDK
    {
    public:
        virtual ~IPersp3ctiveSDK()
        {
        }
        
        enum TouchEvent
        {
            kBegin
            , kEnd
            , kContinue
        };
        
        virtual bool configure(  const std::string& iPluginName
                              , const std::string& iPluginDisplayName
                              , const std::string& iIPAddress
                              , uint16_t iPort ) = 0;
        
        virtual bool registerControl(  const std::string& iID
                                    , const std::string& iName
                                    , float iMin
                                    , float iMax
                                    , int32_t iSteps
                                    , int32_t iDefault) = 0;
        
        virtual bool start() = 0;
        virtual void stop()  = 0;

        virtual bool sendEvent(  const std::string& iID
                               , float iValue
                               , TouchEvent iTouchState) = 0;
        
        virtual bool processIncomingEvent( std::string& iID
                                          , float &iValue
                                          , TouchEvent &iTouchState) = 0;
    };

    class Persp3ctive : public IPersp3ctiveSDK
    {
    public:
        Persp3ctive()
        {
        }

        virtual ~Persp3ctive()
        {
        }

        bool configure(  const std::string& iPluginName
                       , const std::string& iPluginDisplayName
                       , const std::string& iIPAddress
                       , uint16_t iPort ) override
        { return true; }
        
        bool registerControl(  const std::string& iID
                             , const std::string& iName
                             , float iMin
                             , float iMax
                             , int32_t iSteps
                             , int32_t iDefault) override
        { return true; }

        bool start() override { return true; }
        void stop() override {}
        
        bool sendEvent(   const std::string& iID
                         , float iValue
                         , TouchEvent iTouchState) override
        { return true; }

        bool processIncomingEvent( std::string& iID
                                  , float &iValue
                                  , TouchEvent &iTouchState) override
        { return true; }
    };

    Persp3ctive myPerspective;
}

