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

#pragma once

#define BBC_PI 3.14159265358979f

/**
 * \brief Class for angles
 */
class Angle
{
public:
    
    enum AngleType
    {
          kRadians
        , kDegrees
    };
    
    Angle()
    {
    }

    Angle(float iVal, AngleType iType)
    {
        if (iType == kDegrees)
        {
            fromDegrees(iVal);
        }
        else
        {
            fromRadians(iVal);
        }
    }
    
    virtual ~Angle()
    {
    }

    virtual void fromDegrees(float iDegrees)
    {
        angleInRadians_ = iDegrees * (BBC_PI / 180.0);
    }

    float inDegrees() const
    {
        return angleInRadians_ * (180.0 / BBC_PI);
    }

    virtual void fromRadians(float iRadians)
    {
        angleInRadians_ = iRadians;
    }

    float inRadians() const
    {
        return angleInRadians_;
    }

    friend bool operator== (const Angle& lhs, const Angle& rhs)
    {
        return lhs.angleInRadians_ == rhs.angleInRadians_;
    }

    friend bool operator< (const Angle& lhs, const Angle& rhs)
    {
        return lhs.angleInRadians_ < rhs.angleInRadians_;
    }
    
    friend bool operator> (const Angle& lhs, const Angle& rhs)
    {
        return rhs < lhs;
    }

    friend bool operator<=(const Angle& lhs, const Angle& rhs)
    {
        return !(lhs > rhs);
    }

    friend bool operator>=(const Angle& lhs, const Angle& rhs)
    {
        return !(lhs < rhs);
    }

    Angle operator +(const Angle& iObj)
    {
        return Angle(angleInRadians_ + iObj.angleInRadians_, kRadians);
    }
    
protected:
    
    float angleInRadians_ = 0.0f;
};

/**
 * \brief Class for angles with bounding values
 */
class BoundAngle : public Angle
{
    BoundAngle() = delete;
    
public:
    
    BoundAngle(float iVal, AngleType iType, float iMin, float iMax)
    {
        if (iType == kDegrees)
        {
            minDegrees_ = iMin;
            maxDegrees_ = iMax;
            
            minRadians_ = iMin * (BBC_PI / 180.0);
            maxRadians_ = iMax * (BBC_PI / 180.0);;
        }
        else
        {
            minRadians_ = iMin;
            maxRadians_ = iMax;

            minDegrees_ = iMin * (180.0f / BBC_PI);
            maxDegrees_ = iMax * (180.0f / BBC_PI);
        }
            
        if (iType == kDegrees)
        {
            fromDegrees(iVal);
        }
        else
        {
            fromRadians(iVal);
        }
    }
    
    virtual ~BoundAngle()
    {
    }
    
    void fromDegrees(float iDegrees) override
    {
        if (iDegrees < minDegrees_)
            iDegrees = minDegrees_;
        else
        if (iDegrees > maxDegrees_)
            iDegrees = maxDegrees_;

        angleInRadians_ = iDegrees * (BBC_PI / 180.0);
    }
    
    void fromRadians(float iRadians) override
    {
        if (iRadians < minRadians_)
            iRadians = minRadians_;
        else
        if (iRadians > maxRadians_)
            iRadians = maxRadians_;

        angleInRadians_ = iRadians;
    }

    BoundAngle operator +(const BoundAngle& iObj)
    {
        return BoundAngle(angleInRadians_ + iObj.angleInRadians_, kRadians, minRadians_, maxRadians_);
    }

private:
    
    float minDegrees_ = std::numeric_limits<float>::min();
    float maxDegrees_ = std::numeric_limits<float>::max();

    float minRadians_ = std::numeric_limits<float>::min();
    float maxRadians_ = std::numeric_limits<float>::max();
};

