#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

// NOTE: Render size values must be passed from code
uniform float renderWidth;
uniform float renderHeight;

// Radius of the warp. Can be updated from the game.
uniform float radius = 50.0;
float angle = 3.14;

// Center of the warp. Can be updated from game.
uniform vec2 center = vec2(200.0, 200.0);

void main()
{
    vec2 texSize = vec2(renderWidth, renderHeight);
    vec2 tc = fragTexCoord*texSize;
    tc -= center; // tc is a distance vec2 from center to current fragment

    float dist = length(tc);

    if (dist < radius)
    {
        float percent = (radius - dist)/radius;
        float theta = percent*percent*angle;
        float s = sin(theta);
        float c = cos(theta);

        tc = vec2(dot(tc, vec2(c, -s)), dot(tc, vec2(s, c)));
    }

    tc += center; // Now tc is a pixel coordinate of fragment
    vec4 color = texture2D(texture0, tc/texSize)*colDiffuse*fragColor;

    finalColor = vec4(color.rgb, 1.0);
}