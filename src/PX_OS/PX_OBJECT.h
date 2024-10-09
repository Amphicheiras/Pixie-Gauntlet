#pragma once

class PX_OBJECT
{
public:
    virtual ~PX_OBJECT() {};

    virtual void setDrivers() = 0;
    virtual void begin() = 0;
    virtual void loop() = 0;
};