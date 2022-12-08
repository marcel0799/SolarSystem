#version 330
precision mediump float;

in vec3 v2f_normal;
in vec2 v2f_texcoord;
in vec3 v2f_light;
in vec3 v2f_view;

out vec4 f_color;

//wir haben hier vier verschiedene texturen, das ist krass
uniform sampler2D day_texture;
uniform sampler2D night_texture;
uniform sampler2D cloud_texture;
uniform sampler2D gloss_texture;
uniform bool greyscale;

const float shininess = 20.0;
const vec3  sunlight = vec3(1.0, 0.941, 0.898);

void main()
{
    /** \todo Implement a fancy earth shader.
    * - Copy your working code from the fragment shader of your Phong shader use it as starting point
    * - instead of using a single texture, use the four texures `day_texure`, `night_texure`,
    *   `cloud_texure` and `gloss_texture` and mix them for enhanced effects:
    *       * The `gloss_texture` defines how glossy (specular) a point on earth is
    *       * The `cloud_texture` defines how cloudy a point on earth is, and more clouds should produce less reflections at this point
    *       * `day_texture` and `night_texture` define the day and night color of the earth. Make sure that there is a soft transition between both.
    *       * Note that there is no phong shading at the night side.
    * Hints:
    *   - cloud and gloss textures are just greyscales. So you'll just need one color component.
    *   - The texture(texture, 2d_position) returns a 4-vector (rgba). You can use either `texture(...).r` to get just the red component
    *     or `texture(...).rgb` to get a vec3 color value
    *   - use mix(vec3 a,vec3 b, float s) = a*(1-s) + b*s for linear interpolation of two colors
    *   - Lookup the documentation of the function called `smoothstep(...)` it might be helpful.
    *   - There is not the one right way to get the desired results. Feel free to use some magic numbers or creative solutions.
     */
    
    //UEBERNOMMEN AUS PHONG.frag---------------------------------------------------------------

    vec3 color = vec3(0.0,0.0,0.0);


    //neu

    //das eines sind farben -> vec3 und das andere nur greyscales deswegen nur float
    //man kann es sich wie ein gewicht vorstellen, wie wolkig ist es dort
    //gloss = 0 no reflections, gloss = 1 reflections

    vec3 day = texture(day_texture, v2f_texcoord.st).rgb;
    vec3 night = texture(night_texture, v2f_texcoord.st).rgb;
    float gloss = texture(gloss_texture, v2f_texcoord.st).a;
    float cloudiness = texture(cloud_texture, v2f_texcoord.st).a;

    //clouds are not specular = wolken reflextieren nicht
    gloss *= (1.0 - cloudiness);

    //ende neu


    vec3 material = texture(day_texture, v2f_texcoord.st).rgb;
    float alpha = texture(day_texture, v2f_texcoord.st).a;

    vec3 N = normalize(v2f_normal);
    vec3 L = v2f_light;
    vec3 R = reflect(-L,N);         //aktuell ist L zur quelle, deswegen irgendwie ein minus
    vec3 V = normalize(v2f_view);   //eben schon bestimmt
    
    
    float diffuse = max(0.0, dot(N,L)); 

    float specular = 0.0;
    if(diffuse != 0.0) {
        specular = pow( max(0.0, dot(R,V)), shininess );
    }

    if (greyscale) color = vec3(0.299*color.r+0.587*color.g+0.114*color.b);
    color = sunlight * ((0.2 * material) + (material * diffuse));
    f_color = vec4(color, alpha);

    //ENDE UEBERNOMMEN AUS PHONG.FRAG----------------------------------------------------------------------

}
