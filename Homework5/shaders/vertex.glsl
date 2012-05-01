/*
 * vertex.glsl 
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 04/30/2012
 */
 
attribute vec4 position;
attribute vec2 texcoord0;

uniform mat4 projMatrix;
uniform mat4 modelMatrix;

varying vec2 frag_texcoord0;

void main()
{
    mat4 modelViewProjectionMatrix = projMatrix * modelMatrix;
    gl_Position = modelViewProjectionMatrix * position;
    
    frag_texcoord0 = texcoord0;
}
