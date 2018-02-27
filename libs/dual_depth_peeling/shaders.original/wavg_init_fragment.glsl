//--------------------------------------------------------------------------------------
// Order Independent Transparency with Average Color
//
// Author: Louis Bavoil
// Email: sdkfeedback@nvidia.com
//
// Copyright (c) NVIDIA Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#extension ARB_draw_buffers : require

vec4 ShadeFragment();

varying in vec3 N;
varying in vec3 V;

varying in vec4 fcolor;

void main(void)
{
   vec3 L = normalize(gl_LightSource[0].position.xyz - V);
   vec3 E = normalize(-V); // we are in Eye Coordinates, so EyePos is (0,0,0)
   vec3 R = normalize(-reflect(L,N));
   vec4 Iamb = fcolor * gl_LightSource[0].ambient;
   vec4 Idiff = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse * max(dot(N,L), 0.0);
   Idiff = clamp(Idiff, 0.0, 1.0);

   vec4 Ispec = gl_FrontLightProduct[0].specular
                * pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);
   Ispec = clamp(Ispec, 0.0, 1.0);

   //vec4 emission = (0.0,0.0,0.0,1.0);
   vec4 color2 = gl_FrontMaterial.emission + fcolor*gl_LightModel.ambient+Iamb + Idiff + Ispec;

   //     vec4 color2 = gl_FrontLightModelProduct.sceneColor + Iamb + Idiff + Ispec;
        vec4 color = ShadeFragment();

        gl_FragData[0] = vec4(color2.rgb * color.a, color.a);
        gl_FragData[1] = vec4(1.0);


}
