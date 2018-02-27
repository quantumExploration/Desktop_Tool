//--------------------------------------------------------------------------------------
// Order Independent Transparency with Average Color
//
// Author: Louis Bavoil
// Email: sdkfeedback@nvidia.com
//
// Copyright (c) NVIDIA Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

vec3 ShadeVertex();
varying out vec4 fcolor;

varying out vec3 N;
varying out vec3 V;


void main(void)
{

        V = vec3(gl_ModelViewMatrix * gl_Vertex);
        N = normalize(gl_NormalMatrix * gl_Normal);

        fcolor = gl_Color;

        gl_Position = ftransform();
        gl_TexCoord[0].xyz = ShadeVertex();


}
