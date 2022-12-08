#version 330

layout (location = 0) in vec4 v_position; // input vertex position
layout (location = 1) in vec3 v_normal;   // input vertex normal
layout (location = 2) in vec2 v_texcoord; // input texture coordinates

out vec2 v2f_texcoord;  // output: texture coordinates
out vec3 v2f_normal;    // output: normal at current point
out vec3 v2f_light;     // output: normalized light vector from current vertex to light source
out vec3 v2f_view;      // output: normalized view vector from current vertex to the eye

uniform mat4 modelview_projection_matrix;
uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;
uniform vec4 light_position; //in eye space coordinates already

void main()
{
    /** \todo Implement the phong vertex shader.
     * - Setup all `out` variables defined above. They will be the input of the phong fragment shader.
     * - To do so you can use all other vectors and matrices above
     * - Don't forget to compute the vertex position in screen space and store it in the global variable gl_Position
     * - In `Solar_Viewer::draw_scene(...)` you have to set all uniforms from both the vertex AND the pixel shader
     *
     * Hints: - Compute the vertex position, normal and light vector in eye space.
     *        - Store the final vertex position in `gl_Position`
     *        - Make sure that your vector sizes match
     *        - Via .xyz you can access the first three components of a vec4
     */

    //transform normal to eye coordinates
    v2f_normal = normalize(normal_matrix * v_normal);
    
    // direction to light source (in eye space)
    //.xyz wird gemacht um nur auf die ersten drei wert des vec4 zuzugreifen
    v2f_light = normalize(light_position.xyz - (modelview_matrix * v_position).xyz); 
    
    //viewing vector (from vertex to eye in view_coordinates) siehe kommentar neben deklarierung der var
    // Eye position is origin
    v2f_view = normalize(-(modelview_matrix * v_position).xyz);

    //Rest uebernommen aus ich glaube color.frag ??
    // pass through texture coordinate
    v2f_texcoord = v_texcoord;
    
    // transform vertex by modelview and projection matrix
    gl_Position = modelview_projection_matrix * v_position;



} 
