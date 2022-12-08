#version 330
precision mediump float;

// output from phong.vert --> input from phong.frag
in vec3 v2f_normal;
in vec2 v2f_texcoord;
in vec3 v2f_light;
in vec3 v2f_view;

//Ziel ist es die richtige farbe zu bestimmen
out vec4 f_color;

uniform sampler2D tex;
uniform bool greyscale;

const float shininess = 8.0;
const vec3  sunlight = vec3(1.0, 0.941, 0.898);

void main()
{
    /**
    *  Implement the Phong shading model (like in the first exercise)
    *  - Use the passed `in` variables to compute the resulting color and store it in `f_color`.
    *  - The texture's color value should be used as material parameter for ambient, diffuse and specular lighting.
    *  - You can copy the function to extract the texture's color from the `color.frag` fragmentshader.
    *  - Scale ambient and specular light component by 0.2 to get a more realistic result
    *  - You do not have to implement shadows. NIX SCHATTEN
    *
    *  Hint: Here, functions like reflect, dot, max, min, normalize can be used in the same way as in the raytracer.
     */

    vec3 color = vec3(0.0,0.0,0.0);

    //uebernommen aus color.frag
    // fetch color from texture
    vec3 material = texture(tex, v2f_texcoord.st).rgb;
    float alpha = texture(tex, v2f_texcoord.st).a;

    //erstellen von N, R und L aus der vorlesung 
    vec3 N = normalize(v2f_normal);
    vec3 L = v2f_light;
    vec3 R = reflect(-L,N);         //aktuell ist L zur quelle, deswegen irgendwie ein minus
    vec3 V = normalize(v2f_view);   //eben schon bestimmt
    
    
    //erstellen des diffusem lichtes, das licht das leicht ueberall ist
    //abfangen das es nicht negativ ist, sonst kommt das licht von innen, also mehr als 90 grad
    float diffuse = max(0.0, dot(N,L)); 

    //erstellen des specularen lichts
    //wir muessen aus irgendwelche gruenden sicherstellen, dass das dot produkt positiv ist
    float specular = 0.0;
    if(diffuse != 0.0) {
        //wir koennen mit diffuse vergleich weil das ja gerade dot(N,L) entspricht, nette randsache, keine doppelberechnung
        specular = pow( max(0.0, dot(R,V)), shininess );
        //hoch die shininess, wie in der vorlesung
    }



    // convert RGB color to YUV color and use only the luminance
    if (greyscale) color = vec3(0.299*color.r+0.587*color.g+0.114*color.b);

    /*
    -   material mal nehmen mit dem diffusen licht ist in diesem fall unsere Farbe
    -   material addieren entspricht unserem ambient licht
    -   diffuses licht = (material * diffuse)*sunlight
    -   ambient licht = (0.2 * material) * sunlight
    */
    color = sunlight * ((0.2 * material) + (material * diffuse) + 0.2*material*specular);
    
    
    // add required alpha value
    f_color = vec4(color, alpha);
}
