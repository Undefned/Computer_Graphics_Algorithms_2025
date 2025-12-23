float4x4 WorldViewProj : WORLDVIEWPROJECTION;

// Текстура как параметр (она появится в Properties)
texture2D DiffuseMap <
    string UIName = "DiffuseMap";
>;

sampler2D DiffuseSampler = sampler_state
{
    Texture   = <DiffuseMap>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU  = Wrap;
    AddressV  = Wrap;
};

struct VS_INPUT {
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
};

struct VS_OUTPUT {
    float4 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
};

VS_OUTPUT mainVS(VS_INPUT i)
{
    VS_OUTPUT o;
    o.Position = mul(float4(i.Position, 1.0), WorldViewProj);
    o.TexCoord = i.TexCoord;
    return o;
}

float4 mainPS(VS_OUTPUT i) : COLOR0
{
    float4 c = tex2D(DiffuseSampler, i.TexCoord);
    c.a = 1;
    return c;
}

technique tech0
{
    pass p0
    {
        VertexShader = compile vs_2_0 mainVS();
        PixelShader  = compile ps_2_0 mainPS();
    }
}
