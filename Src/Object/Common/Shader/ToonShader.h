#pragma once

#include "ShaderBase.h"

class ToonShader : public ShaderBase
{
public:

    ToonShader();

    ~ToonShader() override = default;

    void Init(void) override;
};