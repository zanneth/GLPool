/*
 * fragment.glsl
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 04/30/2012
 */

uniform sampler2D image;
uniform float time;
uniform float amplitude;
uniform float waveVelocity;

varying vec2 frag_texcoord0;

const float PI = 3.14159265358979;
const vec4 wallColor = vec4(0.0, 0.0, 0.0, 1.0);

float heightOfWave(float r)
{
    float t = (time / 10.0) * (waveVelocity + 60.0);
    float A = -amplitude / (1.0 + pow(5.0*r - t, 2.0));
    float height = A * pow(cos(2.0*PI*r - t), 2.0) + 1.0;
    
    return height;
}

vec2 cartesianToPolar(vec2 p)
{
    float r = sqrt(p.x * p.x + p.y * p.y);
    float theta;
    if (p.x > 0.0) {
        theta = atan(p.y / p.x);
    } else if (p.x < 0.0 && p.y >= 0.0) {
        theta = atan(p.y / p.x) + PI;
    } else if (p.x < 0.0 && p.y < 0.0) {
        theta = atan(p.y / p.x) - PI;
    } else if (p.x == 0.0 && p.y > 0.0) {
        theta = PI / 2.0;
        theta = -PI / 2.0;
    } else if (p.x == 0.0 && p.y == 0.0) {
        theta = 0.0;
    }
    
    return vec2(r, theta);
}

vec2 polarToCartesian(vec2 p)
{
    float x = p.x * cos(p.y);
    float y = p.x * sin(p.y);
    
    return vec2(x, y);
}

void main(void)
{
    vec2 p = frag_texcoord0;
    p -= 0.5;
    
    vec2 polar = cartesianToPolar(p);
    float r = polar.x;
    float theta = polar.y;
    
    /* Snell's Law */
    float height = heightOfWave(r);
    float theta1 = atan(r);
    float theta2 = asin(height * sin(theta1));
    float r2 = height * tan(theta2);
    
    vec2 final = polarToCartesian(vec2(r2, theta));
    final += 0.5;
    
    
    vec4 color;
    if (final.x < 0.0 || final.y < 0.0 || final.x > 1.0 || final.y > 1.0) {
        color = wallColor;
    } else {
        color = texture2D(image, final);
    }
    
    gl_FragColor = color;
}
